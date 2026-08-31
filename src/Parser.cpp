#include "Parser.h"
#include <iostream>
using namespace std;

namespace cantmake {

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

ProjectConfig Parser::parse() {
    ProjectConfig config;

    while (!isAtEnd() && peek().type != TokenType::EndOfFile) {
        parseStatement(config);
    }

    return config;
}

void Parser::parseStatement(ProjectConfig& config) {
    Token ident = consume(TokenType::Identifier, "Expected identifier.");
    string keyword = ident.lexeme;

    if (keyword == "project") {
        Token str = consume(TokenType::String, "Expected string after 'project'.");
        config.projectName = str.lexeme;
    } else if (keyword == "compiler") {
        Token str = consume(TokenType::String, "Expected string after 'compiler'.");
        config.compiler = str.lexeme;
    } else if (keyword == "standard") {
        Token str = consume(TokenType::String, "Expected string after 'standard'.");
        config.cppStandard = str.lexeme;
    } else if (keyword == "output") {
        Token str = consume(TokenType::String, "Expected string after 'output'.");
        config.outputExecutable = str.lexeme;
    } else if (keyword == "sources") {
        parseList(config.sourceFiles);
    } else if (keyword == "include_dirs") {
        parseList(config.includeDirectories);
    } else {
        throw runtime_error("Unknown keyword '" + keyword + "' at line " + to_string(ident.line) + ":" + to_string(ident.column));
    }
}

void Parser::parseList(vector<string>& list) {
    consume(TokenType::LeftBrace, "Expected '{' before list.");

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        Token str = consume(TokenType::String, "Expected string inside list.");
        list.push_back(str.lexeme);
    }

    consume(TokenType::RightBrace, "Expected '}' after list.");
}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::EndOfFile;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const string& errorMessage) {
    if (check(type)) return advance();
    throw runtime_error(errorMessage + " (line " + to_string(peek().line) + ":" + to_string(peek().column) + ")");
}
}