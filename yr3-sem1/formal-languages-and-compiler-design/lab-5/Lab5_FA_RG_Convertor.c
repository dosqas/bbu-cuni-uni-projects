#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lab5_FA_RG_Convertor.h"

// -------------------------------------------------------------
// UTILITY FUNCTION
// -------------------------------------------------------------
// Finds the index of a symbol in the automaton's alphabet
int symbol_index(FiniteAutomaton *fa, char symbol) {
    for (int i = 0; i < strlen(fa->alphabet); i++) {
        if (fa->alphabet[i] == symbol) return i;
    }
    return -1; // Symbol not found
}

int is_final_state(FiniteAutomaton *fa, int state_number) {
    for (int f = 0; f < fa->num_finals; f++) {
        if (fa->final_states[f] == state_number) return 1;
    }
    return 0;
}

// -------------------------------------------------------------
// DEFINE FA FOR IDENTIFIERS
// -------------------------------------------------------------
// An identifier starts with a letter (a-z or A-Z) and may be followed
// by any combination of letters or digits.
FiniteAutomaton buildIdentifierFA() {
    FiniteAutomaton fa;

    fa.num_states = 2; // q0 = start, q1 = accepting state
    strcpy(fa.alphabet, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    // Initialize all transitions to -1 (meaning "no transition")
    for (int i = 0; i < MAX_STATES; i++)
        for (int j = 0; j < MAX_SYMBOLS; j++)
            fa.transitions[i][j] = -1;

    // Define start and final states
    fa.start_state = 0;
    fa.final_states[0] = 1; // 1 is the only final state
    fa.num_finals = 1;

    // Transitions:
    // q0 --LETTER--> q1
    // q1 --LETTER or DIGIT--> q1
    for (char c = 'a'; c <= 'z'; c++) {
        int idx = symbol_index(&fa, c);
        fa.transitions[0][idx] = 1;
        fa.transitions[1][idx] = 1;
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        int idx = symbol_index(&fa, c);
        fa.transitions[0][idx] = 1;
        fa.transitions[1][idx] = 1;
    }

    for (char c = '0'; c <= '9'; c++) {
        int idx = symbol_index(&fa, c);
        fa.transitions[1][idx] = 1;
    }

    return fa;
}

// -------------------------------------------------------------
// DEFINE FA FOR CONSTANTS (NUMBERS)
// -------------------------------------------------------------
// A constant (number) can be:
// - "0"
// - or a nonzero digit followed by digits (e.g., 25, 903, etc.)
FiniteAutomaton buildNumberFA() {
    FiniteAutomaton fa;

    fa.num_states = 3; // q0 = start, q1 = accepts "0", q2 = accepts multi-digit numbers
    strcpy(fa.alphabet, "0123456789");

    // Initialize all transitions to -1
    for (int i = 0; i < MAX_STATES; i++)
        for (int j = 0; j < MAX_SYMBOLS; j++)
            fa.transitions[i][j] = -1;

    // Define start and final states
    fa.start_state = 0;
    fa.final_states[0] = 1; // Accept "0"
    fa.final_states[1] = 2; // Accept numbers starting from 1-9
    fa.num_finals = 2;

    // Transitions:
    // q0 --'0'--> q1
    int idx0 = symbol_index(&fa, '0');
    if (idx0 != -1)
        fa.transitions[0][idx0] = 1;

    // q0 --[1-9]--> q2
    // q2 --[0-9]--> q2 (loop for multiple digits)
    for (char c = '1'; c <= '9'; c++) {
        int idx = symbol_index(&fa, c);
        if (idx != -1) {
            fa.transitions[0][idx] = 2;
            fa.transitions[2][idx] = 2;
        }
    }
    // add '0' loop at q2
    if (idx0 != -1)
        fa.transitions[2][idx0] = 2;

    return fa;
}

// -------------------------------------------------------------
// PRINT FINITE AUTOMATON
// -------------------------------------------------------------
// Displays all states, transitions, and final states of the automaton.
void printFA(FiniteAutomaton *fa, const char *name) {
    int hasTransition;

    printf("\n=== Finite Automaton for %s ===\n", name);
    printf("States: { ");
    for (int i = 0; i < fa->num_states; i++) printf("q%d ", i);
    printf("}\nAlphabet: %s\nStart state: q%d\nFinal states: { ", fa->alphabet, fa->start_state);
    for (int i = 0; i < fa->num_finals; i++) printf("q%d ", fa->final_states[i]);
    printf("}\nTransitions:\n");

    // Print all transitions in a readable format
    for (int i = 0; i < fa->num_states; i++) {
        hasTransition = 0;
        for (int j = 0; j < strlen(fa->alphabet); j++) {
            int next = fa->transitions[i][j];
            if (next != -1) {
                hasTransition = 1;
                printf("  q%d --%c--> q%d\n", i, fa->alphabet[j], next);
            }
        }
        if (hasTransition)
            printf("\n");
    }
}

// -------------------------------------------------------------
// TRANSFORMATION: FINITE AUTOMATON → REGULAR GRAMMAR
// -------------------------------------------------------------
// Converts the finite automaton into an equivalent regular grammar.
void FA_to_RG(FiniteAutomaton *fa, const char *name) {
    printf("\n=== Regular Grammar for %s ===\n", name);
    for (int i = 0; i < fa->num_states; i++) {
        printf("A%d -> ", i);

        // For every possible symbol, if there’s a transition, print a rule
        for (int j = 0; j < strlen(fa->alphabet); j++) {
            int next = fa->transitions[i][j];
            if (next != -1) {
                int nextnext = fa->transitions[i][j + 1];
                if (nextnext != -1 || is_final_state(fa, i))
                    printf("%cA%d | ", fa->alphabet[j], next);
                else printf("%cA%d", fa->alphabet[j], next);
            }
        }

        // If the current state is final, add an epsilon-like rule (“_” used for readability)
        if (is_final_state(fa, i)) printf("_");

        printf("\n\n");
    }
}