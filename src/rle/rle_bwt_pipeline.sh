#!/bin/bash

# Check for input file
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <input file>"
    exit 1
fi

INPUT_FILE=$1
RLE1_OUTPUT="rle1_output.txt"
BWT_OUTPUT="bwt_output.txt"
FINAL_OUTPUT="final_output.rle"

# Step 1: Run the first RLE
echo "Running first RLE on $INPUT_FILE..."
./rle "$INPUT_FILE" "$RLE1_OUTPUT"
if [[ $? -ne 0 ]]; then
    echo "First RLE failed."
    exit 1
fi

# Step 2: Run the BWT
echo "Running BWT on $RLE1_OUTPUT..."
./bwt "$RLE1_OUTPUT" "$BWT_OUTPUT"
if [[ $? -ne 0 ]]; then
    echo "BWT failed."
    exit 1
fi

# Step 3: Run the second RLE
echo "Running second RLE on $BWT_OUTPUT..."
./rle "$BWT_OUTPUT" "$FINAL_OUTPUT"
if [[ $? -ne 0 ]]; then
    echo "Second RLE failed."
    exit 1
fi

echo "Pipeline completed successfully. Final output: $FINAL_OUTPUT"

