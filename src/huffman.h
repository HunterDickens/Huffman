#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TREE_HT 100
#define CHAR_COUNT 256 // Total possible characters (ASCII)

// Structure for Huffman Tree Node
typedef struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
} MinHeapNode;

// Structure for MinHeap
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode **array;
} MinHeap;

// Function Declarations
void calculateFrequencies(const char* sentence, int freq[]);
void printFrequencies(int freq[]);
void buildDataFromFrequencies(int freq[], char data[], int dataFreq[], int* size);
void huffmanCodes(char data[], int freq[], int size);

#endif // HUFFMAN_H
