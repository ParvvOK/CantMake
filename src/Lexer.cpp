#include "Lexer.h"
#include <cctype>
using namespace std;

namespace cantmake {

Lexer::Lexer(const string& source) : source(source) {}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;

        char c = peek();
        int startColumn = column;

        if (c == '{') {
            tokens.push_back({TokenType::LeftBrace, "{", line, startColumn});
            advance();
        } else if (c == '}') {
            tokens.push_back({TokenType::RightBrace, "}", line, startColumn});
            advance();
        } else if (c == '"') {
            advance();
            string str;
            while (!isAtEnd() && peek() != '"' && peek() != '\n') {
                str += advance();
            }
            
            if (isAtEnd() || peek() == '\n') {
                tokens.push_back({TokenType::Unknown, str, line, startColumn});
            } else {
                advance();
                tokens.push_back({TokenType::String, str, line, startColumn});
            }
        } else if (isalpha(c) || c == '_') {
            string ident;
            while (!isAtEnd() && (isalnum(peek()) || peek() == '_')) {
                ident += advance();
            }
            tokens.push_back({TokenType::Identifier, ident, line, startColumn});
        } else {
            string unk;
            unk += advance();
            tokens.push_back({TokenType::Unknown, unk, line, startColumn});
        }
    }

    tokens.push_back({TokenType::EndOfFile, "", line, column});
    return tokens;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::advance() {
    char c = source[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
            advance();
        } else if (c == '#') {
            while (!isAtEnd() && peek() != '\n') {
                advance();
            }
        } else {
            break;
        }
    }
}

}