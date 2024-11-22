#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "huffman.h"


int main() {
    char sentence[1024];
    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0'; // Remove the trailing newline

    int freq[CHAR_COUNT] = {0};
    calculateFrequencies(sentence, freq);

    // Optional: Print the frequencies
    printf("\nCharacter Frequencies:\n");
    printFrequencies(freq);

    // Prepare data arrays for Huffman tree
    char data[CHAR_COUNT];
    int dataFreq[CHAR_COUNT];
    int size;

    buildDataFromFrequencies(freq, data, dataFreq, &size);

    // Build Huffman Codes
    printf("\nHuffman Codes:\n");
    huffmanCodes(data, dataFreq, size);

    return 0;
}
