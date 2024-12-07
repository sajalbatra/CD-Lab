#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

struct Opcode {
    string mnemonic;
    int opcode;
    int length;
};

struct Symbol {
    string name;
    int address;
};

struct Literal {
    string literal;
    int address;
};

// Mnemonic Opcode Table (MOT)
unordered_map<string, Opcode> MOT = {
    {"MOVER", {"MOVER", 1, 2}},
    {"ADD", {"ADD", 2, 2}},
    {"STOP", {"STOP", 3, 1}},
};

// Pseudo Opcode Table (POT)
unordered_map<string, string> POT = {
    {"START", "START"},
    {"END", "END"},
    {"ORIGIN", "ORIGIN"},
    {"LTORG", "LTORG"},
    {"DC", "DC"},
};

// Symbol Table (ST) and Literal Table (LT)
unordered_map<string, Symbol> symbolTable;
vector<Literal> literalTable;
int locationCounter = 0;

// Function to handle labels
void handleLabel(const string& label) {
    if (symbolTable.find(label) == symbolTable.end()) {
        symbolTable[label] = {label, locationCounter};
    } else {
        cout << "Error: Duplicate label found: " << label << endl;
    }
}

// Function to handle literals
void handleLiteral(const string& literal) {
    for (const auto& lit : literalTable) {
        if (lit.literal == literal) return; // Literal already exists
    }
    literalTable.push_back({literal, -1}); // Address will be resolved later
}

// Function to resolve literals (Assign addresses to literals)
void resolveLiterals() {
    for (auto& lit : literalTable) {
        if (lit.address == -1) {
            lit.address = locationCounter;
            locationCounter += 1;  // Assuming 1-byte literals
        }
    }
}

// Function to write the tables to files
template<typename T>
void writeTableToFile(const string& filename, const unordered_map<string, T>& table) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& entry : table) {
            outFile << entry.second.name << " " << entry.second.address << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
    }
}

void writeLiteralTableToFile(const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& entry : literalTable) {
            outFile << entry.literal << " " << entry.address << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
    }
}

int main() {
    string line;
    ifstream sourceFile("source.asm");

    if (sourceFile.is_open()) {
        while (getline(sourceFile, line)) {
            istringstream iss(line);
            string label, opcode, operand;

            // Handle label (if present)
            if (line.find(':') != string::npos) {
                iss >> label;
                label.pop_back();  // Remove ':' from label
                handleLabel(label);  // Process label
                iss >> opcode;
            } else {
                iss >> opcode;
            }

            // Process pseudo-ops (POT) like START, END, ORIGIN, LTORG
            if (POT.find(opcode) != POT.end()) {
                if (opcode == "START") {
                    iss >> operand;
                    locationCounter = stoi(operand);  // Initialize location counter
                } else if (opcode == "ORIGIN") {
                    iss >> operand;
                    if (operand.find('+') != string::npos) {
                        string sym = operand.substr(0, operand.find('+'));
                        int offset = stoi(operand.substr(operand.find('+') + 1));
                        locationCounter = symbolTable[sym].address + offset;
                    }
                } else if (opcode == "LTORG") {
                    resolveLiterals();  // Assign addresses to unresolved literals
                } else if (opcode == "END") {
                    resolveLiterals();  // Assign addresses to unresolved literals
                    break;  // End of source code
                }
            }
            // Process mnemonic opcodes (MOT)
            else if (MOT.find(opcode) != MOT.end()) {
                iss >> operand;

                // If operand is a literal, process it
                if (!operand.empty() && operand[0] == '=') {
                    handleLiteral(operand);
                } else if (!operand.empty()) {
                    // If operand is a symbol, handle it
                    handleLabel(operand);
                }

                locationCounter += MOT[opcode].length;  // Update location counter
            }
            // Handle direct assignment in DC
            else if (opcode == "DC") {
                handleLabel(label);
                locationCounter += 1;  // Assuming DC occupies 1 byte
            }
        }
        sourceFile.close();
    } else {
        cout << "Unable to open source file." << endl;
        return 1;
    }

    // Write tables to respective files
    writeTableToFile("SymbolTable.txt", symbolTable);
    writeLiteralTableToFile("LiteralTable.txt");

    cout << "Assembler has processed the source file and generated Symbol Table and Literal Table." << endl;

    return 0;
}
