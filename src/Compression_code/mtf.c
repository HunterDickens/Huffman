#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    fprintf(stderr, "Performing MTF encoding on ");

    // input file
    if (argc > 1) {
        if (freopen(argv[1], "rb", stdin) == NULL) {
            fprintf(stderr, "Error: Could not open input file '%s'\n", argv[1]);
            return 1;
        }
        fprintf(stderr, "%s", argv[1]);
    } else {
        fprintf(stderr, "stdin");
    }

    // output file
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

    
    unsigned char order[256];
    for (int i = 0; i < 256; i++) {
        order[i] = (unsigned char) i;
    }

    int c;
    while ((c = getc(stdin)) != EOF) {
        int i;

        
        for (i = 0; i < 256; i++) {
            if (order[i] == (c & 0xff)) {
                break;
            }
        }

        putc((char) i, stdout);

        // Shuffle the order array
        for (int j = i; j > 0; j--) {
            order[j] = order[j - 1];
        }
        order[0] = (unsigned char) c;
    }

    return 0;
}
