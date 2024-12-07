#include<bits/stdc++.h>
//wap to implement recursive descent parser


class Parser {
private:
    std::string input;
    size_t pos;

    char peek() {
        if (pos < input.length())
            return input[pos];
        return '\0';
    }

    char consume() {
        if (pos < input.length())
            return input[pos++];
        return '\0';
    }

    void skipWhitespace() {
        while (isspace(peek()))
            consume();
    }
    int expr() {
        int result = term();
        
        while (peek() == '+') {
            consume();
            result += term();
        }
        
        return result;
    }

    int term() {
        int result = factor();
        
        while (peek() == '*') {
            consume();
            result *= factor();
        }
        
        return result;
    }

    int factor() {
        skipWhitespace();
        
        if (isdigit(peek())) {
            return number();
        }
        else if (peek() == '(') {
            consume();
            int result = expr();
            
            if (peek() != ')')
                throw std::runtime_error("Expected ')'");
            
            consume();
            return result;
        }
        else {
            throw std::runtime_error("Unexpected character");
        }
    }

    int number() {
        int result = 0;
        
        while (isdigit(peek())) {
            result = result * 10 + (consume() - '0');
        }
        
        return result;
    }

public:
    Parser() : pos(0) {}

    int parse(const std::string& expression) {
        input = expression;
        pos = 0;
        return expr();
    }
};

int main(int argc, char const *argv[]) {
    Parser parser;
    
    // Test cases
    std::string expressions[] = {
        "2 + 3",
        "2 * 3",
        "2 + 3 * 4",
        "(2 + 3) * 4",
        "2 * (3 + 4)"
    };

    for (const auto& expr : expressions) {
        try {
            std::cout << "Expression: " << expr << " = " << parser.parse(expr) << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Error parsing '" << expr << "': " << e.what() << std::endl;
        }
    }

    return 0;
}