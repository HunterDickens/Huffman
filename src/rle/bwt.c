#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

// Block size depends on system's integer size
#if (INT_MAX == 32767)
#define BLOCK_SIZE 20000
#else
#define BLOCK_SIZE 200000
#endif

long length;
unsigned char buffer[BLOCK_SIZE];
int indices[BLOCK_SIZE + 1];

int memcmp_signed;

int unsigned_memcmp(const void *p1, const void *p2, unsigned int i) {
    const unsigned char *pc1 = (const unsigned char *)p1;
    const unsigned char *pc2 = (const unsigned char *)p2;
    while (i--) {
        if (*pc1 < *pc2)
            return -1;
        else if (*pc1++ > *pc2++)
            return 1;
    }
    return 0;
}

 // qsort
int bounded_compare(const void *a, const void *b) {
    const unsigned int *i1 = (const unsigned int *)a;
    const unsigned int *i2 = (const unsigned int *)b;
    unsigned int l1 = (unsigned int)(length - *i1);
    unsigned int l2 = (unsigned int)(length - *i2);
    int result;

    if (memcmp_signed)
        result = unsigned_memcmp(buffer + *i1, buffer + *i2, l1 < l2 ? l1 : l2);
    else
        result = memcmp(buffer + *i1, buffer + *i2, l1 < l2 ? l1 : l2);

    if (result == 0)
        return l2 - l1;
    else
        return result;
}

void print_transformation() {
    printf("\n--- BWT Transformation ---\n");
    for (int i = 0; i <= length; i++) {
        if (indices[i] == 0) {
            printf("Index %d: ? (End-of-buffer marker)\n", i);
        } else {
            unsigned char prefix = buffer[indices[i] - 1];
            printf("Index %d: %c\n", i, isprint(prefix) ? prefix : '?');
        }
    }
    printf("--------------------------\n");
}

int main(int argc, char *argv[]) {
    int debug = 0;
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        debug = 1;
        argv++;
        argc--;
    }

    fprintf(stderr, "Performing BWT on ");
    if (argc > 1) {
        if (freopen(argv[1], "rb", stdin) == NULL) {
            fprintf(stderr, "Error: Could not open input file '%s'\n", argv[1]);
            return 1;
        }
        fprintf(stderr, "%s", argv[1]);
    } else {
        fprintf(stderr, "stdin");
    }

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

    // Deter memcmp behavior
    if (memcmp("\x070", "\x080", 1) < 0) {
        memcmp_signed = 0;
        fprintf(stderr, "memcmp() treats character data as unsigned\n");
    } else {
        memcmp_signed = 1;
        fprintf(stderr, "memcmp() treats character data as signed\n");
    }

    
    while ((length = fread(buffer, 1, BLOCK_SIZE, stdin)) > 0) {
        fprintf(stderr, "Performing BWT on %ld bytes\n", length);

        long l = length + 1;
        fwrite(&l, sizeof(long), 1, stdout);

        for (int i = 0; i <= length; i++)
            indices[i] = i;

        qsort(indices, length + 1, sizeof(int), bounded_compare);
        fprintf(stderr, "\n");

        long first = -1, last = -1;
        for (int i = 0; i <= length; i++) {
            if (indices[i] == 1)
                first = i;
            if (indices[i] == 0) {
                last = i;
                fputc('?', stdout);
            } else
                fputc(buffer[indices[i] - 1], stdout);
        }

        // Print
        if (debug) {
            print_transformation();
        }

        fprintf(stderr, "first = %ld  last = %ld\n", first, last);
        fwrite(&first, sizeof(long), 1, stdout);
        fwrite(&last, sizeof(long), 1, stdout);
    }

    return 0;
}
