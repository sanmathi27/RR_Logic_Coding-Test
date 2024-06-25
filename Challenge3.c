/*
Approach:
Define constants:
   MAX_SEARCH_STRING_LEN: Maximum length for the search string.

 formHashTable(char *search_str, int search_Str_len)
   Allocate memory for a hash table `char_pos` with 26 slots(a-z).
   Appaend `char_pos` with positions of each character (a-z) in `search_str`.
   Return `char_pos`.

 printHashTable(int **char_pos)
   Print the hash table `char_pos`

 patternSearch(char *search_str, int search_Str_len, char *pattern, int pattern_length, int **char_pos)
   Allocate memory for `pattern_positions` to store positions where `pattern` matches in `search_str`.
   Check if `pattern` matches at each possible starting position using precomputed `char_pos`.
   Store starting positions of matches in `pattern_positions`.
   Print positions where `pattern` matches in `search_str`.

Main:
   Initialize `search_str` and `pattern`
   Form `char_pos` hash table from `search_str`
   Perform pattern search using `pattern` and `char_pos.`
   Free allocated memory for `char_pos`
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assume the maximum length of search string
#define MAX_SEARCH_STRING_LEN 1000

// Function to form the hash table char_pos from search_str
int **formHashTable(char *search_str, int search_str_len) {
    int **char_pos = (int **)malloc(26 * sizeof(int *)); // Array of 26 pointers (for 'a' to 'z')
    if (char_pos == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Initialize all pointers to NULL
    for (int i = 0; i < 26; ++i) {
        char_pos[i] = NULL;
    }

    // Fill char_pos with positions of each character in search_str
    for (int i = 0; i < search_str_len; ++i) {
        char current_char = search_str[i];

        //Considering alphabetic characters 'a' to 'z'
        if (current_char >= 'a' && current_char <= 'z') {
            int index = current_char - 'a';

            // Allocate memory if not already allocated
            if (char_pos[index] == NULL) {
                char_pos[index] = (int *)malloc(sizeof(int));
                char_pos[index][0] = 0; // Initialize the count of positions in the array
            } else {
                // Reallocate memory for additional position
                char_pos[index] = (int *)realloc(char_pos[index], (char_pos[index][0] + 2) * sizeof(int));
            }

            // Add current position to the list for current_char
            char_pos[index][char_pos[index][0] + 1] = i;
            char_pos[index][0]++; // Increment the count of positions
        }
    }

    return char_pos;
}

void printHashTable(int **char_pos) {
    printf("Hash Table (Character Positions):\n");
    for (int i = 0; i < 26; ++i) {
        if (char_pos[i] != NULL) {
            char current_char = 'a' + i;
            printf("%c : ", current_char);
            for (int j = 1; j <= char_pos[i][0]; ++j) {
                printf("%d ", char_pos[i][j]);
            }
            printf("\n");
        }
    }
}

// Function to search for pattern in search_str using precomputed char_pos
void patternSearch(char *search_str, int search_str_len, char *pattern, int pattern_length, int **char_pos) {
    int i, j, k;
    int match;
    int *pattern_positions = NULL;
    int num_matches = 0;

    pattern_positions = (int *)malloc(sizeof(int) * (search_str_len - pattern_length + 1));
    if (pattern_positions == NULL) {
        perror("Memory allocation failed");
        return;
    }

    // Iterate through possible starting positions in search_str
    for (i = 0; i <= search_str_len - pattern_length; ++i) {
        match = 1; // Assume match unless proven otherwise

        // Check each character in the pattern
        for (j = 0; j < pattern_length; ++j) {
            char current_char = pattern[j];
            int *positions = char_pos[current_char - 'a']; // Get positions for current_char

            // If positions is NULL or does not contain i + j
            if (positions == NULL || positions[0] == -1) {
                match = 0;
                break;
            }

            // Binary search for position i + j in positions array
            int left = 0, right = positions[0];
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (positions[mid] == i + j) {
                    break; // Found the position
                } else if (positions[mid] < i + j) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }

            if (left > right) { // Position i + j not found
                match = 0;
                break;
            }
        }

        if (match) {
            pattern_positions[num_matches++] = i; // Store starting position
        }
    }

    // Print results or further process pattern_positions as needed
    printf("Pattern found at positions: ");
    for (k = 0; k < num_matches; ++k) {
        printf("%d ", pattern_positions[k]);
    }
    printf("\n");

    free(pattern_positions);
}


int main() {
    char search_str[MAX_SEARCH_STRING_LEN] = "zvm is an all-powerful tool";
    char pattern[] = "tool";

    int search_str_len = strlen(search_str);
    int pattern_length = strlen(pattern);

    // Form the hash table (char_pos) from search_str
    int **char_pos = formHashTable(search_str, search_str_len);
    if (char_pos == NULL) {
        fprintf(stderr, "Error: Failed to form hash table.\n");
        return 1;
    }
    printHashTable(char_pos);
    // Perform pattern search using the formed hash table
    patternSearch(search_str, search_str_len, pattern, pattern_length, char_pos);

    // Free allocated memory for char_pos
    for (int i = 0; i < 26; ++i) {
        free(char_pos[i]);
    }
    free(char_pos);

    return 0;
}
