#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char* concatenateWords(char** sentences, int n);

int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);
    getchar(); // to consume newline after scanf

    // Allocate memory for an array of character pointers
    char** sentences = (char**)malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        // Allocate memory for each sentence
        sentences[i] = (char*)malloc(100 * sizeof(char));

        printf("Enter sentence %d: ", i + 1);
        fgets(sentences[i], 100, stdin);
        sentences[i][strcspn(sentences[i], "\n")] = 0; // remove trailing newline
    }

    // Call the NASM subprogram
    char *result = concatenateWords(sentences, n);

    printf("The concatenated string is: %s\n", result);

    // Free the allocated memory
    for (int i = 0; i < n; i++) {
        free(sentences[i]);
    }
    free(sentences);

    // Don't forget to free the memory!
    free(result);
    getchar();

    return 0;
}
