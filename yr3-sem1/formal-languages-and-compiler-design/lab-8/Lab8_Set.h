#ifndef SET_H
#define SET_H

#include <stdlib.h>
#include <stdbool.h>

// Max length for any symbol (Nonterminal, Terminal, etc.)
#define MAX_SYMBOL_LEN 128
// Initial capacity for dynamic string arrays (sets, sequences)
#define INITIAL_CAPACITY 10

// --- String Set/List Structure ---
typedef struct {
    char** items;
    size_t count;
    size_t capacity;
} StringCollection;

// --- Set/Collection Prototypes ---
// Allocates and initializes a new StringCollection (Set or Sequence)
StringCollection* Collection_Create();

// Adds a string to the collection if it doesn't already exist (Set behavior)
bool Collection_Add(StringCollection* coll, const char* item);

// Adds a string to the collection without checking for duplicates (List/Sequence behavior - PUSH)
bool Collection_Append(StringCollection* coll, const char* item);

// Removes the last item and returns it (Stack POP). Caller must free the returned string.
char* Collection_Pop(StringCollection* coll);

// Performs set union: adds all items from source to destination
bool Collection_Union(StringCollection* dest, const StringCollection* source);

// Removes a specific item from the collection (SLOW: Used for Queue pop or set removal)
bool Collection_Remove(StringCollection* coll, const char* item);

// Checks if an item exists in the collection
bool Collection_Contains(const StringCollection* coll, const char* item);

// Creates a deep copy of a StringCollection
StringCollection* Collection_Copy(const StringCollection* source);

// Frees the memory associated with the collection
void Collection_Destroy(StringCollection* coll);

#endif // SET_H