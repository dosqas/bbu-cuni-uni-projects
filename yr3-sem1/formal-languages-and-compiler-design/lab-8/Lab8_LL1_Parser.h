#ifndef LL1_PARSER_H
#define LL1_PARSER_H

#include "Lab8_Set.h"
#include <stdbool.h>

// --- Data Structures for FIRST/FOLLOW Maps ---
// Internal map structure used for FIRST/FOLLOW sets (Map: NonTerminal -> Set)
typedef struct {
    char key[MAX_SYMBOL_LEN];
    StringCollection* value; // The actual set
} StringCollectionMapEntry;

typedef struct StringCollection_Map {
    StringCollectionMapEntry* entries;
    size_t count;
    size_t capacity;
} StringCollection_Map;

// --- Grammar Structures ---

// Represents the RHS (Right Hand Side) of a production (a sequence of symbols)
typedef StringCollection ProductionRHS;

// Represents a list of ProductionRHS (e.g., A -> alpha | beta)
typedef struct {
    ProductionRHS** rules;
    size_t count;
    size_t capacity;
} ProductionList;

// Represents a single production for the ORDEREDPRODUCTIONS list
typedef struct {
    char lhs[MAX_SYMBOL_LEN];
    ProductionRHS* rhs;
} OrderedProduction;

// The main Grammar structure
typedef struct {
    // StringCollection serves as a Set of strings for terminals/nonterminals
    StringCollection* nonterminals;
    StringCollection* terminals;
    
    char* start_symbol;

    // Map: Nonterminal (char*) -> ProductionList*
    struct {
        char lhs[MAX_SYMBOL_LEN]; // <-- FIX IS HERE: added 'char'
        ProductionList* rules;
    } *productions;
    size_t prod_map_count;
    size_t prod_map_capacity;


    // List of all productions in order (1-indexed for the table)
    OrderedProduction* ordered_productions;
    size_t ordered_count;
    size_t ordered_capacity;

} Grammar;

// A single entry in the parsing table: (NonTerminal, Terminal) -> ProductionIndex
typedef struct {
    char non_term[MAX_SYMBOL_LEN];
    char terminal[MAX_SYMBOL_LEN];
    int prod_idx;
} ParseTableEntry;

// The parsing table
typedef struct {
    ParseTableEntry* entries;
    size_t count;
    size_t capacity;
} ParseTable;

// --- Parse Tree Node ---
typedef struct Node {
    char symbol[MAX_SYMBOL_LEN];
    int id; // For table printing
    struct Node* parent;
    
    // Children list (using StringCollection structure for simplicity of management)
    struct Node** children;
    size_t child_count;
    size_t child_capacity;

} Node;

// --- Function Prototypes ---
// General
void Grammar_Destroy(Grammar* g);
void ParseTable_Destroy(ParseTable* table);

// Map Helpers
StringCollection_Map* StringCollectionMap_Create();
StringCollection* StringCollectionMap_Get(const StringCollection_Map* map, const char* key);
bool StringCollectionMap_Set(StringCollection_Map* map, const char* key, StringCollection* value);
void StringCollectionMap_Destroy(StringCollection_Map* map);

// File Reading
Grammar* load_grammar_from_file(const char* filename);
StringCollection* read_sequence_file(const char* filename);
StringCollection* read_pif_file(const char* filename);

// FIRST Set
StringCollection* get_first_set(const char* non_term, StringCollection_Map* first_sets);
StringCollection* compute_first_seq(const ProductionRHS* sequence, const Grammar* grammar, const StringCollection_Map* first_sets);
StringCollection_Map* generate_first(Grammar* grammar);

// FOLLOW Set
StringCollection_Map* generate_follow(const Grammar* grammar, const StringCollection_Map* first_sets);

// Parsing Table
ParseTable* build_parsing_table(const Grammar* grammar, const StringCollection_Map* first_set, const StringCollection_Map* follow_set, bool* has_conflicts);

// Parser
StringCollection* parse(const Grammar* grammar, const ParseTable* table, const StringCollection* seq);

// Tree
Node* build_tree(const Grammar* grammar, const StringCollection* prod_indices);
void print_tree_table(Node* root);
void Node_Destroy(Node* n);

// Utility for printing collection (added for debugging)
void Collection_Print(const StringCollection* coll);


#endif // LL1_PARSER_H