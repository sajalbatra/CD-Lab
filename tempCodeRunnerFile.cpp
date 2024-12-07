#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const int MAX_LABEL = 10;
const int MAX_OPCODE = 10;
const int MAX_OPERAND = 10;

struct Instruction {
    char label[MAX_LABEL];
    char opcode[MAX_OPCODE];
    char operand[MAX_OPERAND];
    int locctr;
};

void assemble(Instruction& instruction, ifstream& input, ofstream& output) {
    // Read instruction from input file
    input >> instruction.label >> instruction(opcode) >> instruction.operand;

    // Initialize location counter
    instruction.locctr = 0;

    // Check for START instruction
    if (strcmp(instruction.opcode, "START") == 0) {
        instruction.locctr = atoi(instruction.operand);
        output << hex << setw(4) << setfill('0') << instruction.locctr << endl;
    } else {
        // Check for WORD, RESW, BYTE, RESB instructions
        if (strcmp(instruction.opcode, "WORD") == 0) {
            instruction.locctr += 3;
        } else if (strcmp(instruction.opcode, "RESW") == 0) {
            instruction.locctr += 3 * atoi(instruction.operand);
        } else if (strcmp(instruction.opcode, "BYTE") == 0) {
            instruction.locctr += 1;
        } else if (strcmp(instruction.opcode, "RESB") == 0) {
            instruction.locctr += atoi(instruction.operand);
        }

        // Write assembled machine code to output file
        output << hex << setw(4) << setfill('0') << instruction.locctr << endl;
    }
}

int main() {
    ifstream input("input.txt");
    ofstream output("output.txt");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Error opening files." << endl;
        return 1;
    }

    Instruction instruction;
    while (input >> instruction.label >> instruction.opcode >> instruction.operand) {
        assemble(instruction, input, output);
    }

    input.close();
    output.close();

    return 0;
}