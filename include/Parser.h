#pragma once
#include "Lexer.h"
#include "ProjectConfig.h"
#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

namespace cantmake {

class Parser {
public:
    explicit Parser(const vector<Token>& tokens);

    ProjectConfig parse();

private:
    const Token& peek() const;
    const Token& previous() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const string& errorMessage);

    void parseStatement(ProjectConfig& config);
    void parseList(vector<string>& list);

    vector<Token> tokens;
    size_t current = 0;
};

}