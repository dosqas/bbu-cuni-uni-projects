#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int main() {

struct Cities {
    int population;
    char* name;
};

struct Cities cities[] = {
    {.population = 30000, .name = "Falticeni"},
    {.population = 17000, .name = "Ludus"},
    {.population = 5000, .name = "Boholt"},
    {.population = 32000, .name = "Tandarei"}
};
int cities_count = 4;
struct Cities* cities_work = cities;
int cities_work_count = cities_count;

// Filter cities where (population > 20000)
struct Cities cities_filtered[100];
int cities_filtered_count = 0;
for (int i = 0; i < cities_count; i++) {
    if (cities[i].population > 20000) {
        cities_filtered[cities_filtered_count++] = cities[i];
    }
}
// Update working dataset
cities_work = cities_filtered;
cities_work_count = cities_filtered_count;

// Sort cities DESC
// Simple bubble sort
for (int i = 0; i < cities_work_count - 1; i++) {
    for (int j = 0; j < cities_work_count - i - 1; j++) {
        if (cities_work[j].population < cities_work[j+1].population) {
            struct Cities temp = cities_work[j];
            cities_work[j] = cities_work[j+1];
            cities_work[j+1] = temp;
        }
    }
}

// Select fields from cities: name

// Output cities
for (int i = 0; i < cities_work_count; i++) {
    printf("%s", cities_work[i].name);
    printf("\n");
}

    return 0;
}