#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lab5_FA_RG_Convertor.h"
#include "Lab5_Recognition.h"

int is_identifier(const char *lexeme) {
    FiniteAutomaton fa = buildIdentifierFA();
    int state = fa.start_state;
    for (int i = 0; i < strlen(lexeme); i++) {
        char c = lexeme[i];
        int idx = symbol_index(&fa, c);
        if (idx == -1) return 0; // invalid symbol
        state = fa.transitions[state][idx];
        if (state == -1) return 0; // dead transition
    }
    // accept if current state is final
    for (int i = 0; i < fa.num_finals; i++)
        if (fa.final_states[i] == state)
            return 1;
    return 0;
}

int is_number(const char *lexeme) {
    FiniteAutomaton fa = buildNumberFA();
    int state = fa.start_state;
    for (int i = 0; i < strlen(lexeme); i++) {
        char c = lexeme[i];
        int idx = symbol_index(&fa, c);
        if (idx == -1) return 0;
        state = fa.transitions[state][idx];
        if (state == -1) return 0;
    }
    for (int i = 0; i < fa.num_finals; i++)
        if (fa.final_states[i] == state)
            return 1;
    return 0;
}
