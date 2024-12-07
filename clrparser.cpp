#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTIONS 10
#define MAX_SYMBOLS 10
#define MAX_STATES 10

typedef struct {
    char lhs[20], rhs[20];
} Production;

Production productions[MAX_PRODUCTIONS];
char symbols[MAX_SYMBOLS][20];
char parsing_table[MAX_STATES][MAX_SYMBOLS][20];
int num_productions = 0, num_symbols = 0, num_states = 0;

int is_terminal(char* symbol) {
    return symbol[0] >= 'a' && symbol[0] <= 'z';
}

int is_non_terminal(char* symbol) {
    return symbol[0] >= 'A' && symbol[0] <= 'Z';
}

void add_symbol(char* symbol) {
    for (int i = 0; i < num_symbols; i++) {
        if (strcmp(symbol, symbols[i]) == 0) return;
    }
    strcpy(symbols[num_symbols++], symbol);
}

void add_state(char* symbol, char* action, int state) {
    int symbol_index = -1;
    for (int i = 0; i < num_symbols; i++) {
        if (strcmp(symbol, symbols[i]) == 0) {
            symbol_index = i;
            break;
        }
    }
    if (symbol_index == -1) {
        strcpy(symbols[num_symbols], symbol);
        symbol_index = num_symbols++;
    }
    strcpy(parsing_table[state][symbol_index], action);
}

void build_parsing_table() {
    num_states = num_productions;
    for (int i = 0; i < num_productions; i++) {
        char *lhs = productions[i].lhs, *rhs = productions[i].rhs;
        if (is_terminal(&rhs[0])) {
            add_state(lhs, rhs, i);
        } else if (is_non_terminal(&rhs[0])) {
            add_state(lhs, rhs, i);
        }
    }
}

void print_parsing_table() {
    printf("Parsing Table:\n--------------\n\n  |");
    for (int i = 0; i < num_symbols; i++) {
        printf("   %s  |", symbols[i]);
    }
    printf("\n");
    for (int i = 0; i < num_states; i++) {
        printf(" %d|", i);
        for (int j = 0; j < num_symbols; j++) {
            printf(" %s |", parsing_table[i][j]);
        }
        printf("\n");
    }
}

int main() {
    printf("Enter the number of productions: ");
    scanf("%d", &num_productions);
    printf("Enter the productions (in the form A->X):\n");
    for (int i = 0; i < num_productions; i++) {
        scanf("%s %s", productions[i].lhs, productions[i].rhs);
        add_symbol(productions[i].lhs);
        add_symbol(productions[i].rhs);
    }
    build_parsing_table();
    print_parsing_table();
    return 0;
}