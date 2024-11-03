#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cctype>
#include <algorithm>


enum TokenType {
    IDENTIFIER,
    CONSTANT,
    KEYWORD,
    PARENTHESIS,
    PUNCTUATION,
    ARITHMETIC_OPERATOR,
    LOGICAL_OPERATOR
};

struct Token {
    TokenType type;
    std::string value;
    
    bool operator==(const Token& other) const {
        return type == other.type && value == other.value;
    }
};

const std::set<std::string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};


bool isKeyword(const std::string& str) {
    return keywords.find(str) != keywords.end();
}

// Function to check if a character is a parenthesis
bool isParenthesis(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}

// Function to check if a character is a punctuation
bool isPunctuation(char c) {
    return c == ';' || c == ':' || c == ',';
}

// Function to check if a character is an arithmetic operator
bool isArithmeticOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Function to check if a string is a logical operator
bool isLogicalOperator(const std::string& str) {
    return str == ">" || str == ">=" || str == "<" || str == "<=" || str == "==" || str == "!=";
}

class LexicalAnalyzer {
private:
    std::string input;
    std::vector<Token> tokens;
    std::set<std::string> identifiers;

    void skipWhitespaceAndComments() {
        size_t pos = 0;
        while (pos < input.length()) {
            if (std::isspace(input[pos])) {
                pos++;
            } else if (input[pos] == '/' && pos + 1 < input.length() && input[pos + 1] == '/') {
                pos = input.find('\n', pos);
                if (pos == std::string::npos) break;
            } else {
                break;
            }
        }
        input = input.substr(pos);
    }

    Token getNextToken() {
        if (input.empty()) return Token{IDENTIFIER, ""};

        char c = input[0];
        if (std::isalpha(c) || c == '_') {
            size_t end = 1;
            while (end < input.length() && (std::isalnum(input[end]) || input[end] == '_')) {
                end++;
            }
            std::string word = input.substr(0, end);
            input = input.substr(end);

            if (isKeyword(word)) {
                return Token{KEYWORD, word};
            } else {
                identifiers.insert(word);
                return Token{IDENTIFIER, word};
            }
        } else if (std::isdigit(c)) {
            size_t end = 1;
            while (end < input.length() && std::isdigit(input[end])) {
                end++;
            }
            std::string number = input.substr(0, end);
            input = input.substr(end);
            return Token{CONSTANT, number};
        } else if (isParenthesis(c)) {
            input = input.substr(1);
            return Token{PARENTHESIS, std::string(1, c)};
        } else if (isPunctuation(c)) {
            input = input.substr(1);
            return Token{PUNCTUATION, std::string(1, c)};
        } else if (isArithmeticOperator(c)) {
            input = input.substr(1);
            return Token{ARITHMETIC_OPERATOR, std::string(1, c)};
        } else if (c == '>' || c == '<' || c == '=' || c == '!') {
            if (input.length() > 1 && input[1] == '=') {
                std::string op = input.substr(0, 2);
                input = input.substr(2);
                return Token{LOGICAL_OPERATOR, op};
            } else {
                input = input.substr(1);
                return Token{LOGICAL_OPERATOR, std::string(1, c)};
            }
        }
        input = input.substr(1);
        return getNextToken();
    }

public:
    void analyze(const std::string& source) {
        input = source;
        tokens.clear();
        identifiers.clear();

        while (!input.empty()) {
            skipWhitespaceAndComments();
            if (!input.empty()) {
                tokens.push_back(getNextToken());
            }
        }
    }

    void printAnalysis() {
        std::map<TokenType, std::string> typeNames;
        typeNames[KEYWORD] = "Keyword";
        typeNames[IDENTIFIER] = "Identifier";
        typeNames[CONSTANT] = "Constant";
        typeNames[PARENTHESIS] = "Parenthesis";
        typeNames[PUNCTUATION] = "Punctuation";
        typeNames[ARITHMETIC_OPERATOR] = "Arithmetic Operator";
        typeNames[LOGICAL_OPERATOR] = "Logical Operator";

        std::map<TokenType, std::vector<std::string> > groupedTokens;

        for (size_t i = 0; i < tokens.size(); ++i) {
            groupedTokens[tokens[i].type].push_back(tokens[i].value);
        }

        std::map<TokenType, std::vector<std::string> >::iterator it;
        for (it = groupedTokens.begin(); it != groupedTokens.end(); ++it) {
            std::cout << typeNames[it->first] << " (" << it->second.size() << "): ";
            for (size_t i = 0; i < it->second.size(); ++i) {
                std::cout << it->second[i] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Duplicate Identifiers: ";
        std::set<std::string>::iterator idIt;
        for (idIt = identifiers.begin(); idIt != identifiers.end(); ++idIt) {
            int count = 0;
            for (size_t i = 0; i < tokens.size(); ++i) {
                if (tokens[i].type == IDENTIFIER && tokens[i].value == *idIt) {
                    count++;
                }
            }
            if (count > 1) {
                std::cout << *idIt << " ";
            }
        }
        std::cout << std::endl;
    }
};

int main() {
    LexicalAnalyzer analyzer;
    std::string input;
    std::string line;

    std::cout << "Enter C/C++ code (press Ctrl+Z on a new line to finish input):\n";
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }

    analyzer.analyze(input);
    analyzer.printAnalysis();

    return 0;
}