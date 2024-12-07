#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTIONS 10
#define MAX_SYMBOLS 20
#define MAX_STATES 20
#define MAX_LEN 20

typedef struct {
    char lhs[MAX_LEN];
    char rhs[MAX_LEN][MAX_LEN];
    int rhs_count;
} Production;

typedef struct {
    char action;     // 's' for shift, 'r' for reduce, 'a' for accept
    int number;      // state number for shift, production number for reduce
} Action;

Production productions[MAX_PRODUCTIONS];
char symbols[MAX_SYMBOLS][MAX_LEN];  // Both terminals and non-terminals
Action parsing_table[MAX_STATES][MAX_SYMBOLS];
int num_productions = 0;
int num_symbols = 0;
int num_states = 0;

int is_terminal(const char* symbol) {
    // Terminals are lowercase letters
    return symbol[0] >= 'a' && symbol[0] <= 'z';
}

int is_non_terminal(const char* symbol) {
    // Non-terminals are uppercase letters
    return symbol[0] >= 'A' && symbol[0] <= 'Z';
}

int get_symbol_index(const char* symbol) {
    for (int i = 0; i < num_symbols; i++) {
        if (strcmp(symbols[i], symbol) == 0) {
            return i;
        }
    }
    strcpy(symbols[num_symbols], symbol);
    return num_symbols++;
}

void add_action(int state, const char* symbol, char action, int number) {
    int symbol_index = get_symbol_index(symbol);
    parsing_table[state][symbol_index].action = action;
    parsing_table[state][symbol_index].number = number;
}

void build_parsing_table() {
    // Initialize parsing table with empty actions
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            parsing_table[i][j].action = ' ';
            parsing_table[i][j].number = -1;
        }
    }

    // Add actions based on the grammar
    // This is where you would implement the CLR parsing table construction algorithm
    // For this example, we'll manually add some actions
    add_action(0, "id", 's', 5);
    add_action(0, "E", ' ', 1);
    add_action(0, "T", ' ', 2);
    add_action(0, "F", ' ', 3);
    
    add_action(1, "+", 's', 6);
    add_action(1, "$", 'a', 0);
    
    // Add more actions as needed
}

void print_parsing_table() {
    printf("\nCLR Parsing Table:\n");
    printf("----------------\n\n");

    // Print header
    printf("State |");
    for (int i = 0; i < num_symbols; i++) {
        printf(" %4s |", symbols[i]);
    }
    printf("\n");

    // Print separator
    for (int i = 0; i < (num_symbols + 1) * 7; i++) {
        printf("-");
    }
    printf("\n");

    // Print table contents
    for (int i = 0; i < num_states; i++) {
        printf("  %2d  |", i);
        for (int j = 0; j < num_symbols; j++) {
            if (parsing_table[i][j].action != ' ') {
                printf(" %c%3d |", 
                    parsing_table[i][j].action, 
                    parsing_table[i][j].number);
            } else {
                printf("     |");
            }
        }
        printf("\n");
    }
}

int main() {
    // Example grammar: E -> E+T | T
    //                  T -> T*F | F
    //                  F -> (E) | id
    
    printf("Enter number of productions: ");
    scanf("%d", &num_productions);
    getchar(); // consume newline

    printf("\nEnter productions (e.g., 'E->E+T' or 'F->id'):\n");
    for (int i = 0; i < num_productions; i++) {
        char input[MAX_LEN];
        fgets(input, MAX_LEN, stdin);
        input[strcspn(input, "\n")] = 0;  // remove newline

        // Parse the input
        char* lhs = strtok(input, "->");
        strcpy(productions[i].lhs, lhs);
        
        // Add LHS to symbols if not present
        get_symbol_index(lhs);
        
        // Parse RHS
        char* rhs = strtok(NULL, "|");
        productions[i].rhs_count = 0;
        while (rhs != NULL) {
            // Remove leading/trailing spaces
            while (*rhs == ' ') rhs++;
            strcpy(productions[i].rhs[productions[i].rhs_count], rhs);
            
            // Add terminals and non-terminals to symbols
            char* symbol = strtok(rhs, " ");
            while (symbol != NULL) {
                get_symbol_index(symbol);
                symbol = strtok(NULL, " ");
            }
            
            productions[i].rhs_count++;
            rhs = strtok(NULL, "|");
        }
    }

    // Add end marker to symbols
    get_symbol_index("$");
    
    // Set number of states (this would normally be calculated)
    num_states = 12;  // Example value

    build_parsing_table();
    print_parsing_table();

    return 0;
}