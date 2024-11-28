#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Print status message
    fprintf(stderr, "Run Length Encoding ");

    // Handle input file
    if (argc > 1) {
        if (freopen(argv[1], "rb", stdin) == NULL) {
            fprintf(stderr, "Error: Could not open input file '%s'\n", argv[1]);
            return 1;
        }
        fprintf(stderr, "%s", argv[1]);
    } else {
        fprintf(stderr, "stdin");
    }

    // Handle output file
    fprintf(stderr, " to ");
    if (argc > 2) {
        if (freopen(argv[2], "wb", stdout) == NULL) {
            fprintf(stderr, "Error: Could not open output file '%s'\n", argv[2]);
            return 1;
        }
        fprintf(stderr, "%s", argv[2]);
    } else {
        fprintf(stderr, "stdout");
    }
    fprintf(stderr, "\n");

    // RLE compression logic
    int last = -1;  // Track the last character (-1 indicates no previous character)
    int c;          // Current character
    int count = 0;  // Counter for repeated characters

    while ((c = getc(stdin)) != EOF) {
        if (c == last) {
            // Increment the count for repeated characters
            count++;
            if (count == 255) {
                // Write the maximum count and reset
                putc(count, stdout);
                count = 0;
            }
        } else {
            // Write the previous character and its count if applicable
            if (last != -1) {
                putc(last, stdout);
                if (count > 0) {
                    putc(count, stdout);
                }
            }
            // Reset for the new character
            last = c;
            count = 0;
        }
    }

    // Write the last character and its count
    if (last != -1) {
        putc(last, stdout);
        if (count > 0) {
            putc(count, stdout);
        }
    }

    return 0;
}
