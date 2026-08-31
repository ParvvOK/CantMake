#pragma once
#include <string>
#include <vector>
using namespace std;

namespace cantmake {

enum class TokenType {
    Identifier,
    String,
    LeftBrace,
    RightBrace,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    string lexeme;
    int line;
    int column;
};

class Lexer {
public:
    explicit Lexer(const string& source);

    vector<Token> tokenize();

private:
    char peek() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();

    string source;
    size_t current = 0;
    int line = 1;
    int column = 1;
};

}