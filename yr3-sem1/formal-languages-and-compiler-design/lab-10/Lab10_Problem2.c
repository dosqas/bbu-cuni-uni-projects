#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int main() {

struct People {
    char* name;
};

struct People people[] = {
    {.name = "Alex"},
    {.name = "Ioan"},
    {.name = "Andreea"},
    {.name = "Stefan"}
};
int people_count = 4;
struct People* people_work = people;
int people_work_count = people_count;

// Sort people DESC
// Simple bubble sort
for (int i = 0; i < people_work_count - 1; i++) {
    for (int j = 0; j < people_work_count - i - 1; j++) {
        if (strcmp(people_work[j].name, people_work[j+1].name) < 0) {
            struct People temp = people_work[j];
            people_work[j] = people_work[j+1];
            people_work[j+1] = temp;
        }
    }
}

// Select fields from people: name

// Output cities
for (int i = 0; i < people_work_count; i++) {
    printf("%s", people_work[i].name);
    printf("\n");
}

    return 0;
}