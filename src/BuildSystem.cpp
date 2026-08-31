#include "BuildSystem.h"
#include "Lexer.h"
#include "Parser.h"
#include "ProjectConfig.h"
#include "DependencyScanner.h"
#include "DependencyGraph.h"
#include "HashManager.h"
#include "CacheManager.h"
#include "TaskScheduler.h"
#include "ThreadPool.h"
#include "Compiler.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <filesystem>
using namespace std;

namespace cantmake {

BuildSystem::BuildSystem(const string& makefileTarget)
    : makefileTarget(makefileTarget) {}

bool BuildSystem::checkFileNeedsCompilation(const string& filename, 
                                            DependencyGraph& graph, 
                                            HashManager& hashManager, 
                                            CacheManager& cacheManager) {
    FileNode* node = graph.getOrCreateNode(filename);
    
    if (node->needsCompilation) return true;

    string currentHash = hashManager.computeHash(filename);
    if (currentHash.empty()) {
        cerr << "Warning: Could not compute hash for " << filename << endl;
        node->needsCompilation = true;
        return true;
    }

    bool changed = cacheManager.hasChanged(filename, currentHash);
    
    for (FileNode* dep : node->dependencies) {
        if (checkFileNeedsCompilation(dep->filename, graph, hashManager, cacheManager)) {
            changed = true;
        }
    }

    node->needsCompilation = changed;
    return changed;
}

bool BuildSystem::run() {
    ifstream file(makefileTarget);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << makefileTarget << endl;
        return false;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    ProjectConfig config;
    try {
        config = parser.parse();
    } catch (const exception& e) {
        cerr << "Parse Error: " << e.what() << endl;
        return false;
    }

    config.projectRoot = utils::projectRootFromMakefile(makefileTarget);
    config.resolvePaths();

    DependencyGraph graph;
    DependencyScanner scanner(config, graph);
    scanner.scan();

    filesystem::path cacheFile = config.projectRoot / "build_cache" / "cache.txt";
    CacheManager cacheManager(cacheFile.string());
    cacheManager.load();

    HashManager hashManager;
    vector<FileNode*> filesToCompile;
    vector<FileNode*> allSourceNodes;

    for (const auto& sourceFile : config.sourceFiles) {
        FileNode* node = graph.getOrCreateNode(sourceFile);
        allSourceNodes.push_back(node);
        
        if (checkFileNeedsCompilation(sourceFile, graph, hashManager, cacheManager)) {
            filesToCompile.push_back(node);
        }
    }

    bool executableExists = filesystem::exists(config.outputExecutable);
    
    if (filesToCompile.empty() && executableExists) {
        cout << "Project is up to date." << endl;
        return true;
    }
    
    if (!executableExists && filesToCompile.empty()) {
        cout << "Executable missing, forcing relink." << endl;
    }

    TaskScheduler scheduler;
    for (FileNode* node : filesToCompile) {
        scheduler.addTask({node});
    }
    scheduler.setFinished();

    Compiler compiler(config);
    size_t numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    cout << "Using " << numThreads << " threads." << endl;
    ThreadPool pool(numThreads, scheduler, compiler);
    pool.start();

    pool.wait();

    if (pool.hasFailed()) {
        cerr << "Build failed due to compilation errors." << endl;
        return false;
    }

    if (!compiler.link(allSourceNodes)) {
        return false;
    }

    filesystem::create_directories(config.projectRoot / "build_cache");
    cacheManager.save(hashManager.getComputedHashes());

    cout << "Build complete." << endl;
    return true;
}

}