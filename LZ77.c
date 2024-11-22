#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WINDOW_SIZE 20
#define BUFFER_SIZE 10

typedef struct {
    int offset;
    int length;
    char next_char;
} Token;

// Function to compress a file using LZ77
void lz77_compress_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        perror("Failed to open input file");
        exit(1);
    }

    FILE *out = fopen(output_file, "wb");
    if (!out) {
        perror("Failed to open output file");
        fclose(in);
        exit(1);
    }

    // Load file into memory
    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    rewind(in);

    char *text = malloc(file_size + 1);
    fread(text, 1, file_size, in);
    text[file_size] = '\0';
    fclose(in);

    Token tokens[100000];
    int token_count = 0;

    clock_t start = clock();

    int i = 0;
    while (i < file_size) {
        int best_offset = 0, best_length = 0;

        for (int j = i - WINDOW_SIZE; j < i; j++) {
            if (j < 0) continue;

            int k = 0;
            while (j + k < i && i + k < file_size && text[j + k] == text[i + k]) {
                k++;
            }

            if (k > best_length) {
                best_length = k;
                best_offset = i - j;
            }
        }

        tokens[token_count].offset = best_offset;
        tokens[token_count].length = best_length;
        tokens[token_count].next_char = i + best_length < file_size ? text[i + best_length] : '\0';
        token_count++;
        i += best_length + 1;
    }

    clock_t end = clock();
    double elapsed_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    fwrite(tokens, sizeof(Token), token_count, out);
    fclose(out);

    free(text);

    printf("Compression completed in %.2f milliseconds.\n", elapsed_ms);
}

// Function to decompress a file compressed using LZ77
void lz77_decompress_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        perror("Failed to open input file");
        exit(1);
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Failed to open output file");
        fclose(in);
        exit(1);
    }

    Token tokens[100000];
    int token_count = fread(tokens, sizeof(Token), 100000, in);
    fclose(in);

    char *output = malloc(1000000);
    int output_length = 0;

    clock_t start = clock();

    for (int i = 0; i < token_count; i++) {
        Token token = tokens[i];

        if (token.offset > 0) {
            int start_idx = output_length - token.offset;
            for (int j = 0; j < token.length; j++) {
                output[output_length++] = output[start_idx++];
            }
        }

        if (token.next_char != '\0') {
            output[output_length++] = token.next_char;
        }
    }

    clock_t end = clock();
    double elapsed_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    fwrite(output, 1, output_length, out);
    fclose(out);

    free(output);

    printf("Decompression completed in %.2f milliseconds.\n", elapsed_ms);
}

// Main function to compress and decompress a file
int main() {
    const char *input_file = "SECTION_1.txt";
    const char *compressed_file = "Section_1_compressed.bin";
    const char *decompressed_file = "Section_1_decompressed.txt";

    // Compress the file
    printf("Compressing '%s'...\n", input_file);
    lz77_compress_file(input_file, compressed_file);

    // Decompress the file
    printf("Decompressing '%s'...\n", compressed_file);
    lz77_decompress_file(compressed_file, decompressed_file);

    printf("Done! Compressed to '%s' and decompressed to '%s'.\n", compressed_file, decompressed_file);
    return 0;
}
