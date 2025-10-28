// -------------------------------------------------------------
// CONFIGURATION CONSTANTS
// -------------------------------------------------------------
#define MAX_STATES 3      // Maximum number of states allowed in an automaton
#define MAX_SYMBOLS 65   // Maximum number of input symbols (alphabet size)

// -------------------------------------------------------------
// FINITE AUTOMATON STRUCTURE
// -------------------------------------------------------------
typedef struct {
    int num_states;                      // Number of states in the automaton
    char alphabet[MAX_SYMBOLS];          // List of input symbols (the alphabet)
    int transitions[MAX_STATES][MAX_SYMBOLS];  // Transition table: [state][symbol] = next state
    int start_state;                     // Starting state (usually q0)
    int final_states[MAX_STATES];        // Array of final/accepting states
    int num_finals;                      // Number of final states
} FiniteAutomaton;

// -------------------------------------------------------------
// FUNCTION DECLARATIONS
// -------------------------------------------------------------
int symbol_index(FiniteAutomaton *fa, char symbol);
void printFA(FiniteAutomaton *fa, const char *name);
void FA_to_RG(FiniteAutomaton *fa, const char *name);
FiniteAutomaton buildIdentifierFA();
FiniteAutomaton buildNumberFA();