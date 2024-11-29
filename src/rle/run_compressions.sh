#!/bin/bash

# CSV output file
OUTPUT_CSV="compression_results.csv"

# Write CSV header
echo "File,Algorithm Sequence,Input Size (bytes),Output Size (bytes),Compression Ratio (%)" > "$OUTPUT_CSV"

# Function to calculate file size and compression ratio
calculate_stats() {
    local input_file=$1
    local output_file=$2
    local sequence=$3

    base_name=$(basename "$input_file") # Extract the file name without the path
    input_size=$(wc -c < "$input_file") # Get input file size in bytes
    output_size=$(wc -c < "$output_file") # Get output file size in bytes
    compression_ratio=$(echo "scale=2; (1 - $output_size / $input_size) * 100" | bc) # Calculate compression ratio
    echo "$base_name,$sequence,$input_size,$output_size,$compression_ratio" >> "$OUTPUT_CSV"
}


# Base directory for input files
CALGARY_DIR="/Users/hunter/Documents/GitHub/Huffman/src/calgary"

# Check if all executables exist
for prog in rle ari huffman bwt mtf; do
    if ! [ -x "./$prog" ]; then
        echo "Error: $prog executable not found or not executable!"
        exit 1
    fi
done

# Loop through all files in the Calgary directory
for file in "$CALGARY_DIR"/*; do
    if [[ -f "$file" ]]; then
        base_name=$(basename "$file") # Get the file name without the path

        # RLE
        ./rle "$file" "$CALGARY_DIR/$base_name.rle"
        calculate_stats "$file" "$CALGARY_DIR/$base_name.rle" "RLE"

        # ARI
        ./ari "$file" "$CALGARY_DIR/$base_name.ari"
        calculate_stats "$file" "$CALGARY_DIR/$base_name.ari" "ARI"

        # Huffman
        ./huffman "$file" "$CALGARY_DIR/$base_name.huf"
        calculate_stats "$file" "$CALGARY_DIR/$base_name.huf" "Huffman"

        # BWT
        ./bwt "$file" "$CALGARY_DIR/$base_name.bwt"

        # BWT + MTF
        ./mtf "$CALGARY_DIR/$base_name.bwt" "$CALGARY_DIR/$base_name.mtf"

        # BWT + MTF + RLE
        ./rle "$CALGARY_DIR/$base_name.mtf" "$CALGARY_DIR/$base_name.bwt_mtf_rle"
        calculate_stats "$file" "$CALGARY_DIR/$base_name.bwt_mtf_rle" "BWT + MTF + RLE"

        # BWT + MTF + ARI
        ./ari "$CALGARY_DIR/$base_name.mtf" "$CALGARY_DIR/$base_name.bwt_mtf_ari"
        calculate_stats "$file" "$CALGARY_DIR/$base_name.bwt_mtf_ari" "BWT + MTF + ARI"

        # BWT + MTF + Huffman
        ./huffman "$CALGARY_DIR/$base_name.mtf" "$CALGARY_DIR/$base_name.bwt_mtf_huf"
        calculate_stats "$file" "$CALGARY_DIR/$base_name.bwt_mtf_huf" "BWT + MTF + Huffman"
    fi
done
# Cleanup: Remove all files with a '.' in the Calgary folder
echo "Cleaning up generated files..."
find "$CALGARY_DIR" -type f -name "*.*" -exec rm {} \;
echo "Cleanup complete!"
