#include "Lab8_Set.h"
#include <stdio.h>
#include <string.h>

// Internal function to resize the array *only if* it is currently full (used by append/add)
static bool Collection_Resize(StringCollection* coll) {
    if (coll->count == coll->capacity) {
        size_t new_capacity = coll->capacity * 2;
        char** new_items = realloc(coll->items, new_capacity * sizeof(char*));
        if (new_items == NULL) {
            perror("Failed to resize collection");
            return false;
        }
        coll->items = new_items;
        coll->capacity = new_capacity;
    }
    return true;
}

StringCollection* Collection_Create() {
    StringCollection* coll = malloc(sizeof(StringCollection));
    if (coll == NULL) {
        perror("Failed to allocate collection");
        return NULL;
    }
    coll->count = 0;
    coll->capacity = INITIAL_CAPACITY;
    coll->items = malloc(coll->capacity * sizeof(char*));
    if (coll->items == NULL) {
        perror("Failed to allocate collection items");
        free(coll);
        return NULL;
    }
    return coll;
}

bool Collection_Contains(const StringCollection* coll, const char* item) {
    if (!coll || !item) return false;
    for (size_t i = 0; i < coll->count; i++) {
        if (strcmp(coll->items[i], item) == 0) {
            return true;
        }
    }
    return false;
}

bool Collection_Add(StringCollection* coll, const char* item) {
    if (Collection_Contains(coll, item)) {
        return false; // Already exists, Set property enforced
    }
    return Collection_Append(coll, item); // Append without duplicate check
}

bool Collection_Append(StringCollection* coll, const char* item) {
    if (!Collection_Resize(coll)) {
        return false;
    }
    coll->items[coll->count] = strdup(item);
    if (coll->items[coll->count] == NULL) {
        perror("Failed to duplicate string");
        return false;
    }
    coll->count++;
    return true;
}

char* Collection_Pop(StringCollection* coll) {
    if (coll->count == 0) {
        return NULL;
    }
    coll->count--;
    char* item = coll->items[coll->count];
    return item;
}

bool Collection_Union(StringCollection* dest, const StringCollection* source) {
    bool changed = false;
    if (!dest || !source) return false;

    for (size_t i = 0; i < source->count; i++) {
        if (!Collection_Contains(dest, source->items[i])) {
            if (Collection_Append(dest, source->items[i])) {
                changed = true;
            } else {
                return false; // Error during append
            }
        }
    }
    return changed;
}

bool Collection_Remove(StringCollection* coll, const char* item) {
    if (!coll || !item) return false;
    for (size_t i = 0; i < coll->count; i++) {
        if (strcmp(coll->items[i], item) == 0) {
            free(coll->items[i]);
            // Shift remaining elements left
            for (size_t j = i; j < coll->count - 1; j++) {
                coll->items[j] = coll->items[j + 1];
            }
            coll->count--;
            return true;
        }
    }
    return false; // Item not found
}

StringCollection* Collection_Copy(const StringCollection* source) {
    if (!source) return NULL;
    StringCollection* dest = Collection_Create();
    if (!dest) return NULL;

    // --- FIX: Ensure capacity is sufficient directly ---
    size_t needed_capacity = source->count > dest->capacity ? source->count : dest->capacity;
    
    if (dest->capacity < needed_capacity) {
        char** new_items = realloc(dest->items, needed_capacity * sizeof(char*));
        if (new_items == NULL) {
            perror("Failed to realloc in Collection_Copy");
            Collection_Destroy(dest);
            return NULL;
        }
        dest->items = new_items;
        dest->capacity = needed_capacity;
    }
    // --------------------------------------------------

    for (size_t i = 0; i < source->count; i++) {
        // We rely on the sufficient capacity allocated above.
        dest->items[i] = strdup(source->items[i]);
        if (dest->items[i] == NULL) {
            // Cleanup on failure
            for(size_t j=0; j<i; j++) free(dest->items[j]);
            free(dest->items);
            free(dest);
            return NULL;
        }
        dest->count++; // Manually increment count
    }
    return dest;
}

void Collection_Destroy(StringCollection* coll) {
    if (coll) {
        for (size_t i = 0; i < coll->count; i++) {
            free(coll->items[i]);
        }
        free(coll->items);
        free(coll);
    }
}