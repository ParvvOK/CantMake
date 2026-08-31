# cantmake - C/C++ Build System & Orchestrator

**cantmake** is a fast, lightweight C/C++ build system designed to handle incremental builds, file caching, dependency scanning, and multi-threaded compilation efficiently. Which sometimes simply does not work (hence cantmake) . This project was made by me to learn and shit.

---

## 🛠️ Key Features

- **Custom Configuration Parsing**: Simple syntax powered by built-in `Lexer` and `Parser` modules (`makefile.cant`).
- **Dependency Tracking**: Automatic header inclusion scanning (`DependencyScanner`) to build a Directed Acyclic Graph (`DependencyGraph`), ensuring only modified modules and their dependents recompile.
- **Smart Caching**: Uses file content hashing (`HashManager` & `CacheManager`) rather than relying purely on timestamps (`mtime`) to safely skip unnecessary compilation steps.
- **Parallel Compilation**: Executes build tasks across multiple CPU threads simultaneously via a custom `ThreadPool` and `TaskScheduler`.
- **Compiler Abstraction**: Seamlessly invokes backend compiler toolchains (such as `g++` or `clang++`) through `Compiler` and `BuildSystem`.

---

## 📁 Repository Structure

```text
.
├── build.sh                 # Bootstrap script to compile the project
├── include/                 # Header files
│   ├── BuildSystem.h
│   ├── CacheManager.h
│   ├── Compiler.h
│   ├── DependencyGraph.h
│   ├── DependencyScanner.h
│   ├── HashManager.h
│   ├── Lexer.h
│   ├── Parser.h
│   ├── ProjectConfig.h
│   ├── TaskScheduler.h
│   ├── ThreadPool.h
│   └── Utils.h
├── src/                     # Source implementations
│   ├── BuildSystem.cpp
│   ├── CacheManager.cpp
│   ├── Compiler.cpp
│   ├── DependencyGraph.cpp
│   ├── DependencyScanner.cpp
│   ├── HashManager.cpp
│   ├── Lexer.cpp
│   ├── Main.cpp
│   ├── Parser.cpp
│   ├── ProjectConfig.cpp
│   ├── TaskScheduler.cpp
│   ├── ThreadPool.cpp
│   └── Utils.cpp
└── test/                    # Test suite & sample project
    ├── makefile.cant        # Sample build configuration
    └── examples/            # Example C++ project
        ├── include/
        └── src/
```

---

## ⚡ Quick Start

### 1. Build the Binary
Make the build script executable and compile **cant**:
```bash
chmod +x build.sh
./build.sh
```

### 2. Test with the Example Project
Navigate into the `test/` directory and run the build configuration:
```bash
cd test
../cant makefile.cant
```
