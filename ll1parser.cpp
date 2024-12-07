//WAP to implement the LL1 parser 
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

// Grammar rules and parsing table
class LL1Parser {
private:
    // Production rules
    map<char, vector<string>> productions;
    // Parsing table
    map<char, map<char, string>> parsingTable;
    // Sets of terminals and non-terminals
    set<char> terminals;
    set<char> nonTerminals;
    char startSymbol;
    
    // Initialize the grammar and parsing table
    void initializeGrammar() {
        // Example grammar:
        // E → TE'
        // E'→ +TE' | ε
        // T → FT'
        // T'→ *FT' | ε
        // F → (E) | id
        
        // Add productions
        productions['E'] = {"TE'"};
        productions['E\''] = {"+TE'", "ε"};
        productions['T'] = {"FT'"};
        productions['T\''] = {"*FT'", "ε"};
        productions['F'] = {"(E)", "id"};
        
        // Set start symbol
        startSymbol = 'E';
        
        // Initialize terminals and non-terminals
        nonTerminals = {'E', 'E\'', 'T', 'T\'', 'F'};
        terminals = {'+', '*', '(', ')','id', '$'};
        
        // Initialize parsing table
        parsingTable['E']['id'] = "TE'";
        parsingTable['E']['('] = "TE'";
        
        parsingTable['E\'']['+'] = "+TE'";
        parsingTable['E\''][')'] = "ε";
        parsingTable['E\'']['$'] = "ε";
        
        parsingTable['T']['id'] = "FT'";
        parsingTable['T']['('] = "FT'";
        
        parsingTable['T\'']['+'] = "ε";
        parsingTable['T\'']['*'] = "*FT'";
        parsingTable['T\''][')'] = "ε";
        parsingTable['T\'']['$'] = "ε";
        
        parsingTable['F']['id'] = "id";
        parsingTable['F']['('] = "(E)";
    }

public:
    LL1Parser() {
        initializeGrammar();
    }
    
    bool parse(string input) {
        // Add end marker to input
        input += "$";
        
        // Initialize stack with start symbol and end marker
        stack<char> parseStack;
        parseStack.push('$');
        parseStack.push(startSymbol);
        
        // Current position in input
        size_t index = 0;
        
        // Parsing process
        while (!parseStack.empty()) {
            char top = parseStack.top();
            char currentInput = input[index];
            
            cout << "Stack: ";
            stack<char> tempStack = parseStack;
            while (!tempStack.empty()) {
                cout << tempStack.top();
                tempStack.pop();
            }
            cout << "\tInput: " << input.substr(index) << endl;
            
            if (top == currentInput) {
                parseStack.pop();
                index++;
                continue;
            }
            
            if (terminals.find(top) != terminals.end()) {
                cout << "Error: Terminal mismatch" << endl;
                return false;
            }
            
            if (parsingTable[top].find(currentInput) == parsingTable[top].end()) {
                cout << "Error: No production rule found" << endl;
                return false;
            }
            
            string production = parsingTable[top][currentInput];
            parseStack.pop();
            
            if (production != "ε") {
                for (int i = production.length() - 1; i >= 0; i--) {
                    parseStack.push(production[i]);
                }
            }
        }
        
        return index == input.length();
    }
};

int main() {
    LL1Parser parser;
    
    // Test cases
    vector<string> testInputs = {
        "id+id*id",
        "id*id",
        "(id)",
        "id+id)",  // Invalid input
    };
    
    for (const string& input : testInputs) {
        cout << "\nParsing input: " << input << endl;
        cout << "-------------------" << endl;
        bool result = parser.parse(input);
        cout << "Parse result: " << (result ? "Success" : "Failure") << endl;
        cout << "-------------------" << endl;
    }
    
    return 0;
}