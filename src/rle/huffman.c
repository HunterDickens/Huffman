#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

// A Huffman Tree node
struct MinHeapNode {
    char data;                 // Input character
    unsigned freq;             // Frequency of the character
    struct MinHeapNode *left, *right; // Left and right child
};

// A Min Heap: Collection of nodes
struct MinHeap {
    unsigned size;             // Current size of min heap
    unsigned capacity;         // Capacity of min heap
    struct MinHeapNode** array; // Array of min heap node pointers
};

struct MinHeapNode* newNode(char data, unsigned freq);
struct MinHeap* createMinHeap(unsigned capacity);
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void generateCodes(struct MinHeapNode* root, int arr[], int top, char codes[256][MAX_TREE_HT]);
void HuffmanCodes(char data[], int freq[], int size, char codes[256][MAX_TREE_HT]);
void writeCompressedData(FILE* inputFile, FILE* outputFile, char codes[256][MAX_TREE_HT], int freq[]);

// Create a new min heap node
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->data = data;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

// Create a Min Heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Min-Heapify
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Extract the minimum value node from the heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node into the Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Build the Min Heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Check if the size of the heap is one
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Build the Huffman Tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// Generate Huffman codes and store them in the codes array
void generateCodes(struct MinHeapNode* root, int arr[], int top, char codes[256][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        generateCodes(root->left, arr, top + 1, codes);
    }

    if (root->right) {
        arr[top] = 1;
        generateCodes(root->right, arr, top + 1, codes);
    }

    if (!(root->left) && !(root->right)) {
        for (int i = 0; i < top; ++i) {
            codes[(unsigned char)root->data][i] = '0' + arr[i];
        }
        codes[(unsigned char)root->data][top] = '\0';
    }
}

// Write compressed data in binary format
void writeCompressedData(FILE* inputFile, FILE* outputFile, char codes[256][MAX_TREE_HT], int freq[]) {
    rewind(inputFile);
    unsigned char buffer = 0;
    int bitsFilled = 0;

    // Write frequency table (header)
    for (int i = 0; i < 256; i++) {
        fwrite(&freq[i], sizeof(int), 1, outputFile);
    }

    // Write compressed data
    char ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        char* code = codes[(unsigned char)ch];
        for (int i = 0; code[i] != '\0'; i++) {
            buffer = (buffer << 1) | (code[i] - '0');
            bitsFilled++;
            if (bitsFilled == 8) {
                fputc(buffer, outputFile);
                buffer = 0;
                bitsFilled = 0;
            }
        }
    }

    // Write remaining bits, if any
    if (bitsFilled > 0) {
        buffer = buffer << (8 - bitsFilled); // Pad remaining bits with 0
        fputc(buffer, outputFile);
    }
}

// Generate Huffman codes
void HuffmanCodes(char data[], int freq[], int size, char codes[256][MAX_TREE_HT]) {
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    generateCodes(root, arr, top, codes);
}

// Main program
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "wb");

    if (!inputFile || !outputFile) {
        fprintf(stderr, "Error: Could not open file(s).\n");
        return 1;
    }

    int freq[256] = {0};
    char ch;

    // Calculate character frequencies
    while ((ch = fgetc(inputFile)) != EOF) {
        freq[(unsigned char)ch]++;
    }

    char data[256];
    int nonZeroFreq[256];
    int size = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = i;
            nonZeroFreq[size] = freq[i];
            size++;
        }
    }

    char codes[256][MAX_TREE_HT] = {{0}};
    HuffmanCodes(data, nonZeroFreq, size, codes);

    rewind(inputFile);
    writeCompressedData(inputFile, outputFile, codes, freq);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
