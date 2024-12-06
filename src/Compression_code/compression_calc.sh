#!/bin/bash

# Ensure the script is used correctly
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <input file> <output file>"
    exit 1
fi

# Assign input and output file paths
INPUT_FILE="$1"
OUTPUT_FILE="$2"
CSV_FILE="compression_results.csv"  # Name of the CSV file to store results

# Check if the input file exists
if [[ ! -f "$INPUT_FILE" ]]; then
    echo "Error: Input file '$INPUT_FILE' does not exist!"
    exit 1
fi

# Check if the output file exists
if [[ ! -f "$OUTPUT_FILE" ]]; then
    echo "Error: Output file '$OUTPUT_FILE' does not exist!"
    exit 1
fi

# Get the sizes of the files
input_size=$(stat -f%z "$INPUT_FILE" 2>/dev/null || stat --printf="%s" "$INPUT_FILE")
output_size=$(stat -f%z "$OUTPUT_FILE" 2>/dev/null || stat --printf="%s" "$OUTPUT_FILE")

# Validate that file sizes are numeric
if [[ -z "$input_size" || -z "$output_size" ]]; then
    echo "Error: Unable to determine file sizes."
    exit 1
fi

# Calculate compression ratio and percentage
compression_ratio=$(echo "scale=2; $input_size / $output_size" | bc)
compression_percentage=$(echo "scale=2; (1 - $output_size / $input_size) * 100" | bc)

# Print the results to the terminal
echo "Input Size: $input_size bytes"
echo "Output Size: $output_size bytes"
echo "Compression Ratio: $compression_ratio"
echo "Compression Percentage: $compression_percentage%"

# Write the results to the CSV file
if [[ ! -f "$CSV_FILE" ]]; then
    # Create the CSV file and add the header if it doesn't exist
    echo "Input File,Output File,Input Size (bytes),Output Size (bytes),Compression Ratio,Compression Percentage (%)" > "$CSV_FILE"
fi

# Append the results to the CSV file
echo "$INPUT_FILE,$OUTPUT_FILE,$input_size,$output_size,$compression_ratio,$compression_percentage" >> "$CSV_FILE"

echo "Results appended to $CSV_FILE"
