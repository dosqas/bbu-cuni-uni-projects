#include "Lab8_LL1_Parser.h"
#include "Lab8_Set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

// --- Helper Functions for StringCollection_Map (used for FIRST/FOLLOW) ---
#define MAP_INITIAL_CAPACITY 10
StringCollection_Map* StringCollectionMap_Create() {
    StringCollection_Map* map = malloc(sizeof(StringCollection_Map));
    if (!map) return NULL;
    map->count = 0; map->capacity = MAP_INITIAL_CAPACITY;
    map->entries = malloc(map->capacity * sizeof(StringCollectionMapEntry));
    if (!map->entries) { free(map); return NULL; }
    return map;
}
StringCollection* StringCollectionMap_Get(const StringCollection_Map* map, const char* key) {
    if (!map || !key) return NULL;
    for (size_t i = 0; i < map->count; i++) {
        if (strcmp(map->entries[i].key, key) == 0) { return map->entries[i].value; }
    }
    return NULL;
}
static bool StringCollectionMap_Resize(StringCollection_Map* map) {
    if (map->count == map->capacity) {
        size_t new_capacity = map->capacity * 2;
        StringCollectionMapEntry* new_entries = realloc(map->entries, new_capacity * sizeof(StringCollectionMapEntry));
        if (!new_entries) { perror("Map resize failed"); return false; }
        map->entries = new_entries; map->capacity = new_capacity;
    }
    return true;
}
bool StringCollectionMap_Set(StringCollection_Map* map, const char* key, StringCollection* value) {
    if (!map || !key) return false;
    for (size_t i = 0; i < map->count; i++) {
        if (strcmp(map->entries[i].key, key) == 0) { Collection_Destroy(map->entries[i].value); map->entries[i].value = value; return true; }
    }
    if (!StringCollectionMap_Resize(map)) return false;
    strncpy(map->entries[map->count].key, key, MAX_SYMBOL_LEN);
    map->entries[map->count].key[MAX_SYMBOL_LEN - 1] = '\0';
    map->entries[map->count].value = value; map->count++;
    return true;
}
void StringCollectionMap_Destroy(StringCollection_Map* map) {
    if (map) {
        for (size_t i = 0; i < map->count; i++) { Collection_Destroy(map->entries[i].value); }
        free(map->entries); free(map);
    }
}
// --- Grammar Management, File Reading, FIRST/FOLLOW, ParseTable, Tree Building (omitted for brevity) ---
void Grammar_Destroy(Grammar* g) {
    if (g) {
        Collection_Destroy(g->nonterminals); Collection_Destroy(g->terminals); free(g->start_symbol);
        for (size_t i = 0; i < g->prod_map_count; i++) {
            ProductionList* pl = g->productions[i].rules;
            if (pl) {
                for (size_t j = 0; j < pl->count; j++) { Collection_Destroy(pl->rules[j]); }
                free(pl->rules); free(pl);
            }
        }
        free(g->productions);
        for (size_t i = 0; i < g->ordered_count; i++) { Collection_Destroy(g->ordered_productions[i].rhs); }
        free(g->ordered_productions); free(g);
    }
}
static ProductionList* Grammar_GetOrAddLHS(Grammar* g, const char* lhs) {
    for (size_t i = 0; i < g->prod_map_count; i++) {
        if (strcmp(g->productions[i].lhs, lhs) == 0) { return g->productions[i].rules; }
    }
    if (g->prod_map_count == g->prod_map_capacity) {
        g->prod_map_capacity *= 2; g->productions = realloc(g->productions, g->prod_map_capacity * sizeof(*g->productions));
        if (!g->productions) { perror("Failed to resize productions map"); return NULL; }
    }
    ProductionList* pl = malloc(sizeof(ProductionList));
    if (!pl) return NULL; pl->count = 0; pl->capacity = INITIAL_CAPACITY; pl->rules = malloc(pl->capacity * sizeof(ProductionRHS*));
    if (!pl->rules) { free(pl); return NULL; }
    strncpy(g->productions[g->prod_map_count].lhs, lhs, MAX_SYMBOL_LEN);
    g->productions[g->prod_map_count].lhs[MAX_SYMBOL_LEN - 1] = '\0';
    g->productions[g->prod_map_count].rules = pl; g->prod_map_count++; return pl;
}
static bool Grammar_AddOrderedProduction(Grammar* g, const char* lhs, ProductionRHS* rhs) {
    if (g->ordered_count == g->ordered_capacity) {
        g->ordered_capacity *= 2; g->ordered_productions = realloc(g->ordered_productions, g->ordered_capacity * sizeof(OrderedProduction));
        if (!g->ordered_productions) { perror("Failed to resize ordered productions"); return false; }
    }
    strncpy(g->ordered_productions[g->ordered_count].lhs, lhs, MAX_SYMBOL_LEN);
    g->ordered_productions[g->ordered_count].lhs[MAX_SYMBOL_LEN - 1] = '\0';
    g->ordered_productions[g->ordered_count].rhs = rhs; g->ordered_count++; return true;
}
Grammar* load_grammar_from_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("Failed to open grammar file"); return NULL; } Grammar* g = malloc(sizeof(Grammar));
    if (!g) { fclose(f); return NULL; }
    g->nonterminals = Collection_Create(); g->terminals = Collection_Create(); g->start_symbol = NULL;
    g->prod_map_count = 0; g->prod_map_capacity = MAP_INITIAL_CAPACITY; g->productions = malloc(g->prod_map_capacity * sizeof(*g->productions));
    g->ordered_count = 0; g->ordered_capacity = INITIAL_CAPACITY; g->ordered_productions = malloc(g->ordered_capacity * sizeof(OrderedProduction));
    if (!g->nonterminals || !g->terminals || !g->productions || !g->ordered_productions) { fprintf(stderr, "Failed to initialize grammar structures.\n"); Grammar_Destroy(g); fclose(f); return NULL; }
    char line[1024]; bool reading_productions = false;
    while (fgets(line, sizeof(line), f)) {
        char* trimmed_line = line; while (*trimmed_line && (*trimmed_line == ' ' || *trimmed_line == '\t' || *trimmed_line == '\n' || *trimmed_line == '\r')) { trimmed_line++; }
        if (*trimmed_line == '\0') continue; char* newline = strchr(trimmed_line, '\n'); if (newline) *newline = '\0';
        if (strncmp(trimmed_line, "N =", 3) == 0) {
            char* symbols_str = trimmed_line + 3; while (*symbols_str == ' ') symbols_str++;
            char* token = strtok(symbols_str, " "); while (token) { Collection_Add(g->nonterminals, token); token = strtok(NULL, " "); } continue;
        }
        if (strncmp(trimmed_line, "E =", 3) == 0) {
            char* symbols_str = trimmed_line + 3; while (*symbols_str == ' ') symbols_str++;
            char* token = strtok(symbols_str, " "); while (token) { Collection_Add(g->terminals, token); token = strtok(NULL, " "); } continue;
        }
        if (strncmp(trimmed_line, "S =", 3) == 0) {
            char* symbol_str = trimmed_line + 3; while (*symbol_str == ' ') symbol_str++; g->start_symbol = strdup(symbol_str); continue;
        }
        if (strcmp(trimmed_line, "P =") == 0) { reading_productions = true; continue; }
        if (reading_productions && strstr(trimmed_line, "->")) {
            char* arrow = strstr(trimmed_line, "->"); *arrow = '\0'; char* lhs = trimmed_line; char* rhs_str = arrow + 2;
            while (*lhs && (*lhs == ' ' || *lhs == '\t')) lhs++;
            char* end_lhs = lhs + strlen(lhs) - 1; while (end_lhs > lhs && (*end_lhs == ' ' || *end_lhs == '\t')) *end_lhs-- = '\0';
            while (*rhs_str && (*rhs_str == ' ' || *rhs_str == '\t')) rhs_str++;
            ProductionRHS* rhs_sequence = Collection_Create();
            char* token = strtok(rhs_str, " ");
            while (token) { Collection_Append(rhs_sequence, token); token = strtok(NULL, " "); }
            ProductionList* pl = Grammar_GetOrAddLHS(g, lhs);
            if (pl->count == pl->capacity) {
                pl->capacity *= 2; pl->rules = realloc(pl->rules, pl->capacity * sizeof(ProductionRHS*));
                if (!pl->rules) { perror("Failed to resize production list"); Grammar_Destroy(g); fclose(f); return NULL; }
            }
            pl->rules[pl->count++] = rhs_sequence; Grammar_AddOrderedProduction(g, lhs, Collection_Copy(rhs_sequence));
        }
    }
    fclose(f);
    for (size_t i = 0; i < g->prod_map_count; i++) {
        ProductionList* pl = g->productions[i].rules;
        for (size_t j = 0; j < pl->count; j++) {
            ProductionRHS* rhs = pl->rules[j];
            for (size_t k = 0; k < rhs->count; k++) {
                const char* sym = rhs->items[k];
                if (strcmp(sym, "epsilon") != 0 && !Collection_Contains(g->nonterminals, sym) && !Collection_Contains(g->terminals, sym)) { Collection_Add(g->terminals, sym); }
            }
        }
    }
    return g;
}
StringCollection* read_sequence_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("Failed to open sequence file"); return NULL; }
    StringCollection* seq = Collection_Create(); char line[1024];
    while (fgets(line, sizeof(line), f)) {
        char* trimmed_line = line; while (*trimmed_line && (*trimmed_line == ' ' || *trimmed_line == '\t' || *trimmed_line == '\n' || *trimmed_line == '\r')) { trimmed_line++; }
        if (*trimmed_line == '\0') continue;
        char* token = strtok(trimmed_line, " \t\n\r"); while (token) { Collection_Append(seq, token); token = strtok(NULL, " \t\n\r"); }
    }
    fclose(f); return seq;
}

StringCollection* read_pif_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Failed to open PIF file");
        return NULL;
    }
    
    StringCollection* tokens = Collection_Create();
    char line[1024];

    // Loop through each line in the PIF file
    while (fgets(line, sizeof(line), f)) {
        char* current_line = line;
        
        // --- 1. Trim leading whitespace/control characters ---
        while (*current_line && isspace((unsigned char)*current_line)) { 
            current_line++; 
        }
        
        // Skip empty or purely whitespace lines
        if (*current_line == '\0') continue;

        // --- 2. Remove Outer Parentheses ---
        // Expect format: (token_code, token_number)
        if (*current_line == '(') {
            current_line++; // Move past '('
            
            // Look for the last ')' to null-terminate the string before it
            char* end_paren = strrchr(current_line, ')');
            if (end_paren) {
                *end_paren = '\0';
            }
        }
        
        // --- 3. Find the Comma Delimiter ---
        // The token code is everything before the first comma
        char* comma_delimiter = strchr(current_line, ',');
        
        // Determine the length of the token code
        size_t token_len = 0;
        if (comma_delimiter) {
            // Token runs up to the comma
            token_len = comma_delimiter - current_line;
        } else {
            // If no comma, it's either an error or a malformed line, but we take the whole line
            token_len = strlen(current_line);
        }

        // --- 4. Extract and Trim the Token Code ---
        char token_name[MAX_SYMBOL_LEN];
        token_name[0] = '\0'; // Initialize to empty string

        // Fix: Handle the special case where the token itself is a comma (e.g., from line (,, 0))
        // This occurs when the comma_delimiter is the first character in the line.
        if (token_len == 0 && comma_delimiter == current_line && current_line[0] == ',') {
             // The token code is the single comma
            token_name[0] = ',';
            token_name[1] = '\0';
        }
        else if (token_len > 0) {
            // Standard case: token is non-empty string before the comma
            // Safely copy the token content
            if (token_len >= MAX_SYMBOL_LEN) token_len = MAX_SYMBOL_LEN - 1;
            strncpy(token_name, current_line, token_len);
            token_name[token_len] = '\0';
        }
        
        // --- 5. Trim and Append the Extracted Token Code ---
        char* trimmed_token = token_name; 
        
        // Only proceed if we have a string to process
        if (*trimmed_token != '\0') {
            // Trim leading whitespace on the copied token
            while (*trimmed_token && isspace((unsigned char)*trimmed_token)) {
                trimmed_token++;
            }
            
            // Trim trailing whitespace on the copied token
            char* end = trimmed_token + strlen(trimmed_token) - 1; 
            while (end >= trimmed_token && isspace((unsigned char)*end)) {
                *end-- = '\0';
            }

            if (*trimmed_token != '\0') {
                Collection_Append(tokens, trimmed_token);
            }
        }
    }

    fclose(f);
    return tokens;
}
StringCollection* get_first_set(const char* non_term, StringCollection_Map* first_sets) {
    return StringCollectionMap_Get(first_sets, non_term);
}
StringCollection* compute_first_seq(const ProductionRHS* sequence, const Grammar* grammar, const StringCollection_Map* first_sets) {
    StringCollection* result = Collection_Create();
    if (sequence->count == 0) { Collection_Add(result, "epsilon"); return result; }
    for (size_t i = 0; i < sequence->count; i++) {
        const char* s = sequence->items[i]; bool has_epsilon = false;
        if (strcmp(s, "epsilon") == 0) { if (i == 0 && sequence->count == 1) Collection_Add(result, "epsilon"); break; }
        if (Collection_Contains(grammar->terminals, s)) { Collection_Add(result, s); break; }
        if (Collection_Contains(grammar->nonterminals, s)) {
            StringCollection* first_s = StringCollectionMap_Get(first_sets, s);
            if (first_s) {
                for (size_t j = 0; j < first_s->count; j++) {
                    if (strcmp(first_s->items[j], "epsilon") == 0) { has_epsilon = true; } 
                    else { Collection_Add(result, first_s->items[j]); }
                }
            }
            if (!has_epsilon) { break; }
            if (i == sequence->count - 1) { Collection_Add(result, "epsilon"); }
        }
    }
    return result;
}
StringCollection_Map* generate_first(Grammar* grammar) {
    StringCollection_Map* first_sets = StringCollectionMap_Create();
    for (size_t i = 0; i < grammar->nonterminals->count; i++) { StringCollectionMap_Set(first_sets, grammar->nonterminals->items[i], Collection_Create()); }
    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 0; i < grammar->prod_map_count; i++) {
            const char* lhs = grammar->productions[i].lhs;
            ProductionList* rules = grammar->productions[i].rules;
            StringCollection* current_first_set = StringCollectionMap_Get(first_sets, lhs);
            for (size_t j = 0; j < rules->count; j++) {
                ProductionRHS* rhs = rules->rules[j];
                StringCollection* f_alpha = compute_first_seq(rhs, grammar, first_sets);
                if (Collection_Union(current_first_set, f_alpha)) { changed = true; }
                Collection_Destroy(f_alpha);
            }
        }
    }
    return first_sets;
}
StringCollection_Map* generate_follow(const Grammar* grammar, const StringCollection_Map* first_sets) {
    StringCollection_Map* follow_sets = StringCollectionMap_Create();
    for (size_t i = 0; i < grammar->nonterminals->count; i++) { StringCollection* f_set = Collection_Create(); StringCollectionMap_Set(follow_sets, grammar->nonterminals->items[i], f_set); }
    StringCollection* start_follow = StringCollectionMap_Get(follow_sets, grammar->start_symbol);
    if (start_follow) { Collection_Add(start_follow, "$"); }
    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 0; i < grammar->prod_map_count; i++) {
            const char* lhs = grammar->productions[i].lhs;
            ProductionList* rules = grammar->productions[i].rules;
            StringCollection* follow_lhs = StringCollectionMap_Get(follow_sets, lhs);
            for (size_t j = 0; j < rules->count; j++) {
                ProductionRHS* rhs = rules->rules[j];
                for (size_t k = 0; k < rhs->count; k++) {
                    const char* B = rhs->items[k];
                    if (!Collection_Contains(grammar->nonterminals, B)) { continue; }
                    ProductionRHS beta_seq; beta_seq.items = (char**)rhs->items + (k + 1); beta_seq.count = rhs->count - (k + 1); beta_seq.capacity = 0;
                    StringCollection* follow_B = StringCollectionMap_Get(follow_sets, B); if (!follow_B) continue;
                    StringCollection* beta_first = compute_first_seq(&beta_seq, grammar, first_sets);
                    for (size_t l = 0; l < beta_first->count; l++) {
                        if (strcmp(beta_first->items[l], "epsilon") != 0) {
                            if (Collection_Add(follow_B, beta_first->items[l])) { changed = true; }
                        }
                    }
                    if (beta_seq.count == 0 || Collection_Contains(beta_first, "epsilon")) {
                        if (Collection_Union(follow_B, follow_lhs)) { changed = true; }
                    }
                    Collection_Destroy(beta_first);
                }
            }
        }
    }
    return follow_sets;
}
static int ParseTable_Find(const ParseTable* table, const char* nt, const char* t) {
    if (!table) return 0;
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].non_term, nt) == 0 && strcmp(table->entries[i].terminal, t) == 0) { return table->entries[i].prod_idx; }
    }
    return 0;
}
static bool ParseTable_Add(ParseTable* table, const char* nt, const char* t, int prod_idx, bool* has_conflicts) {
    int existing_idx = ParseTable_Find(table, nt, t);
    if (existing_idx != 0) {
        if (existing_idx != prod_idx) { printf("[CONFLICT] Table[%s, %s] has %d vs %d\n", nt, t, existing_idx, prod_idx); *has_conflicts = true; return true; }
        return true;
    }
    if (table->count == table->capacity) {
        table->capacity *= 2;
        table->entries = realloc(table->entries, table->capacity * sizeof(ParseTableEntry));
        if (!table->entries) { perror("Failed to resize parse table"); return false; }
    }
    strncpy(table->entries[table->count].non_term, nt, MAX_SYMBOL_LEN); strncpy(table->entries[table->count].terminal, t, MAX_SYMBOL_LEN);
    table->entries[table->count].prod_idx = prod_idx; table->count++; return true;
}
ParseTable* build_parsing_table(const Grammar* grammar, const StringCollection_Map* first_set, const StringCollection_Map* follow_set, bool* has_conflicts) {
    ParseTable* table = malloc(sizeof(ParseTable));
    if (!table) return NULL; table->count = 0; table->capacity = MAP_INITIAL_CAPACITY; table->entries = malloc(table->capacity * sizeof(ParseTableEntry));
    if (!table->entries) { free(table); return NULL; } *has_conflicts = false;
    for (size_t idx = 0; idx < grammar->ordered_count; idx++) {
        int prod_idx = (int)idx + 1; const char* lhs = grammar->ordered_productions[idx].lhs; ProductionRHS* rhs = grammar->ordered_productions[idx].rhs;
        StringCollection* first_alpha = compute_first_seq(rhs, grammar, first_set); bool has_epsilon = Collection_Contains(first_alpha, "epsilon");
        for (size_t k = 0; k < first_alpha->count; k++) {
            const char* t = first_alpha->items[k];
            if (strcmp(t, "epsilon") != 0) { ParseTable_Add(table, lhs, t, prod_idx, has_conflicts); }
        }
        if (has_epsilon) {
            StringCollection* follow_lhs = StringCollectionMap_Get(follow_set, lhs);
            if (follow_lhs) {
                for (size_t k = 0; k < follow_lhs->count; k++) {
                    const char* t = follow_lhs->items[k]; ParseTable_Add(table, lhs, t, prod_idx, has_conflicts);
                }
            }
        }
        Collection_Destroy(first_alpha);
    }
    return table;
}
void ParseTable_Destroy(ParseTable* table) { if (table) { free(table->entries); free(table); } }
Node* Node_Create(const char* symbol) {
    Node* n = malloc(sizeof(Node));
    if (!n) return NULL; strncpy(n->symbol, symbol, MAX_SYMBOL_LEN); n->symbol[MAX_SYMBOL_LEN - 1] = '\0';
    n->id = 0; n->parent = NULL; n->child_count = 0; n->child_capacity = INITIAL_CAPACITY;
    n->children = malloc(n->child_capacity * sizeof(Node*));
    if (!n->children) { free(n); return NULL; }
    return n;
}
void Node_AddChild(Node* parent, Node* child) {
    if (parent->child_count == parent->child_capacity) {
        parent->child_capacity *= 2; parent->children = realloc(parent->children, parent->child_capacity * sizeof(Node*));
        if (!parent->children) { perror("Failed to resize node children"); return; }
    }
    parent->children[parent->child_count++] = child; child->parent = parent;
}
void Node_Destroy(Node* n) {
    if (n) {
        for (size_t i = 0; i < n->child_count; i++) { Node_Destroy(n->children[i]); }
        free(n->children); free(n);
    }
}
static const char** g_prod_iter = NULL; static size_t g_prod_count = 0;
static Node* build_rec(const Grammar* grammar, const char* symbol) {
    Node* node = Node_Create(symbol);
    if (Collection_Contains(grammar->terminals, symbol) || strcmp(symbol, "epsilon") == 0) { return node; }
    if (!g_prod_iter || g_prod_count == 0) { return node; }
    int prod_idx = atoi(*g_prod_iter); g_prod_iter++; g_prod_count--;
    if (prod_idx < 1 || prod_idx > grammar->ordered_count) { fprintf(stderr, "Invalid production index: %d\n", prod_idx); return node; }
    ProductionRHS* rhs = grammar->ordered_productions[prod_idx - 1].rhs;
    if (rhs->count == 1 && strcmp(rhs->items[0], "epsilon") == 0) {
        Node_AddChild(node, Node_Create("epsilon"));
    } else {
        for (size_t i = 0; i < rhs->count; i++) { Node* child = build_rec(grammar, rhs->items[i]); Node_AddChild(node, child); }
    }
    return node;
}
Node* build_tree(const Grammar* grammar, const StringCollection* prod_indices) {
    g_prod_iter = (const char**)prod_indices->items; g_prod_count = prod_indices->count;
    Node* root = build_rec(grammar, grammar->start_symbol);
    return root;
}
void print_tree_table(Node* root) {
    if (!root) return;
    Node** queue = malloc(100 * sizeof(Node*)); int head = 0, tail = 0; int max_size = 100;
    queue[tail++] = root; int counter = 1; root->id = counter++;
    while (head < tail) {
        Node* n = queue[head++];
        for (size_t i = 0; i < n->child_count; i++) {
            Node* child = n->children[i]; child->id = counter++;
            if (tail == max_size) { max_size *= 2; queue = realloc(queue, max_size * sizeof(Node*)); }
            queue[tail++] = child;
        }
    }
    printf("%-10s%-20s%-10s%-10s\n", "Index", "Symbol", "Father", "Sibling");
    printf("-----------------------------------------------------------\n");
    head = 0; tail = 0; queue[tail++] = root;
    while (head < tail) {
        Node* n = queue[head++]; int father_id = n->parent ? n->parent->id : 0; int sibling_id = 0;
        if (n->parent) {
            for(size_t i=0; i < n->parent->child_count; i++) {
                if (n->parent->children[i] == n && i + 1 < n->parent->child_count) { sibling_id = n->parent->children[i+1]->id; break; }
            }
        }
        printf("%-10d%-20s%-10d%-10d\n", n->id, n->symbol, father_id, sibling_id);
        for (size_t i = 0; i < n->child_count; i++) { queue[tail++] = n->children[i]; }
    }
    free(queue);
}

// --- Parser ---

StringCollection* parse(const Grammar* grammar, const ParseTable* table, const StringCollection* seq) {
    // Input Stack (Queue-like: FIFO)
    StringCollection* input_stack = Collection_Copy(seq);
    Collection_Append(input_stack, "$"); 

    // Work Stack (Stack-like: LIFO)
    StringCollection* work_stack = Collection_Create();
    Collection_Append(work_stack, "$");
    Collection_Append(work_stack, grammar->start_symbol);

    char a[MAX_SYMBOL_LEN];
    char X[MAX_SYMBOL_LEN];
    StringCollection* output = Collection_Create();

    while (input_stack->count > 0 && work_stack->count > 0) {
        // Current input token 'a' (always items[0], queue-like)
        if (input_stack->count == 0) break;
        strncpy(a, input_stack->items[0], MAX_SYMBOL_LEN);
        a[MAX_SYMBOL_LEN - 1] = '\0';
        
        // Top of work stack 'X' (always items[count - 1], stack-like)
        if (work_stack->count == 0) break;
        strncpy(X, work_stack->items[work_stack->count - 1], MAX_SYMBOL_LEN);
        X[MAX_SYMBOL_LEN - 1] = '\0';
        
        // Acceptance
        if (strcmp(a, "$") == 0 && strcmp(X, "$") == 0) {
            Collection_Destroy(input_stack);
            Collection_Destroy(work_stack);
            return output;
        }

        // Match Terminal (X is terminal or $)
        if (Collection_Contains(grammar->terminals, X) || strcmp(X, "$") == 0) {
            if (strcmp(X, a) == 0) {
                // Match: consume input 'a' (Queue Pop - SLOW, but unavoidable)
                Collection_Remove(input_stack, a);
                
                // Match: consume stack 'X' (LIFO Pop - FAST)
                char* popped_X = Collection_Pop(work_stack); 
                free(popped_X); 
            } else {
                printf("ERROR: Terminal mismatch. Expected '%s', got '%s'.\n", X, a);
                break;
            }
        } 
        
        // Non-terminal (X is non-terminal)
        else {
            int prod_idx = ParseTable_Find(table, X, a);
            
            if (prod_idx != 0) {
                // 1. Add production index to output
                char prod_idx_str[10];
                sprintf(prod_idx_str, "%d", prod_idx);
                Collection_Append(output, prod_idx_str);

                // 2. Pop X from work stack (FAST LIFO Pop)
                char* popped_X = Collection_Pop(work_stack); 
                free(popped_X); 

                // 3. Get RHS
                ProductionRHS* rhs = grammar->ordered_productions[prod_idx - 1].rhs;
                
                // 4. Push RHS (in reverse order, fast LIFO push)
                // --- FIX: Check for Epsilon Production ---
                if (!(rhs->count == 1 && strcmp(rhs->items[0], "epsilon") == 0)) {
                    for (size_t k = rhs->count; k > 0; k--) {
                         Collection_Append(work_stack, rhs->items[k - 1]);
                    }
                }
                // --- END FIX ---
            } else {
                // FAILED LOOKUP 
                printf("ERROR: No rule found for M[%s, %s]\n", X, a);
                printf("Expected from %s: ", X);
                for(size_t i=0; i<table->count; i++) {
                    if(strcmp(table->entries[i].non_term, X) == 0) {
                        printf("'%s' ", table->entries[i].terminal);
                    }
                }
                printf("\n");
                break;
            }
        }
    }
    
    bool success = (input_stack->count == 1 && strcmp(input_stack->items[0], "$") == 0 &&
                    work_stack->count == 1 && strcmp(work_stack->items[0], "$") == 0);

    Collection_Destroy(input_stack);
    Collection_Destroy(work_stack);
    
    if (success) {
        return output;
    } else {
        Collection_Destroy(output);
        return NULL;
    }
}

// --- Utility for printing collection ---

void Collection_Print(const StringCollection* coll) {
    if (!coll) {
        printf("NULL\n");
        return;
    }
    printf("{ ");
    for(size_t i = 0; i < coll->count; i++) {
        printf("'%s'%s", coll->items[i], (i == coll->count - 1) ? "" : ", ");
    }
    printf(" }\n");
}


// --- Main Function (Example Usage with Diagnostics) ---

int main(int argc, char* argv[]) {
    
    // === REQUIREMENT 1 (Seminar Grammar) ===
    printf("\n=== REQUIREMENT 1 (Seminar Grammar) ===\n");

    Grammar* g1 = load_grammar_from_file("Lab8_SeminarGrammar.txt");
    if (!g1) return 1;

    StringCollection_Map* first1 = generate_first(g1);
    StringCollection_Map* follow1 = generate_follow(g1, first1);
    
    bool conflict1;
    ParseTable* table1 = build_parsing_table(g1, first1, follow1, &conflict1);

    printf("Conflicts? %s\n", conflict1 ? "true" : "false");

    StringCollection* seq1 = read_sequence_file("Lab8_SeminarSequence.txt");
    if (!seq1) {
        Grammar_Destroy(g1); StringCollectionMap_Destroy(first1); StringCollectionMap_Destroy(follow1); ParseTable_Destroy(table1);
        return 1;
    }
    
    StringCollection* res1 = parse(g1, table1, seq1);
    printf("Parsed productions: ");
    if (res1) {
        for(size_t i=0; i<res1->count; i++) {
            printf("%s ", res1->items[i]);
        }
        printf("\n");
        Collection_Destroy(res1);
    } else {
        printf("Parse failed.\n");
    }

    Collection_Destroy(seq1);
    Grammar_Destroy(g1);
    StringCollectionMap_Destroy(first1);
    StringCollectionMap_Destroy(follow1);
    ParseTable_Destroy(table1);


    // === REQUIREMENT 2 (Mini DSL Grammar) ===
    printf("\n=== REQUIREMENT 2 (Mini DSL Grammar) ===\n");
    
    Grammar* g2 = load_grammar_from_file("Lab8_MiniDSLGrammar.txt");
    if (!g2) return 1;

    StringCollection_Map* first2 = generate_first(g2);
    StringCollection_Map* follow2 = generate_follow(g2, first2);

    bool conflict2;
    ParseTable* table2 = build_parsing_table(g2, first2, follow2, &conflict2);

    printf("Conflicts? %s\n", conflict2 ? "true" : "false");

    StringCollection* seq2 = read_pif_file("Lab8_MiniDSLPIF.txt");
    if (!seq2) {
        Grammar_Destroy(g2); StringCollectionMap_Destroy(first2); StringCollectionMap_Destroy(follow2); ParseTable_Destroy(table2);
        return 1;
    }

    StringCollection* res2 = parse(g2, table2, seq2);

    if (res2) {
        printf("Parse successful. Building parse tree...\n");
        Node* root = build_tree(g2, res2);
        print_tree_table(root);
        Node_Destroy(root);
        Collection_Destroy(res2);
    } else {
        printf("Parse failed.\n");
    }

    Collection_Destroy(seq2);
    Grammar_Destroy(g2);
    StringCollectionMap_Destroy(first2);
    StringCollectionMap_Destroy(follow2);
    ParseTable_Destroy(table2);


    return 0;
}