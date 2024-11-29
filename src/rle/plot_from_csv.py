import pandas as pd
import matplotlib.pyplot as plt
from io import StringIO

# Input your data as a string
data = """
File,Algorithm Sequence,Input Size (bytes),Output Size (bytes),Compression Ratio (%)
rle_good,RLE,     185,      25,87.00
rle_good,ARI,     185,     131,30.00
rle_good,Huffman,     185,    1105,-497.00
rle_good,BWT + MTF + RLE,     185,      55,71.00
rle_good,BWT + MTF + ARI,     185,      83,56.00
rle_good,BWT + MTF + Huffman,     185,    1064,-475.00
"""

# Load the data into a pandas DataFrame
df = pd.read_csv(StringIO(data))

# Convert 'Compression Ratio (%)' to numeric to handle plotting
df['Compression Ratio (%)'] = pd.to_numeric(df['Compression Ratio (%)'], errors='coerce')

# Plot the data
plt.figure(figsize=(10, 6))
bars = plt.bar(df['Algorithm Sequence'], df['Compression Ratio (%)'], color=plt.cm.tab10.colors)

# Add text labels above the bars (Input Size and Output Size)
for bar, ratio, input_size, output_size in zip(
    bars, df['Compression Ratio (%)'], df['Input Size (bytes)'], df['Output Size (bytes)']
):
    if ratio < 0:
        # Place the negative compression ratio text at the 40% mark
        plt.text(bar.get_x() + bar.get_width() / 2,
                 40,  # Fixed position for negative values
                 f"-{abs(ratio):.1f}%\nIn: {int(input_size)}\nOut: {int(output_size)}",
                 ha='center', fontsize=10, color='red')
    else:
        # Add input and output size above each bar
        plt.text(bar.get_x() + bar.get_width() / 2,
                 bar.get_height() + 15,
                 f"In: {int(input_size)}\nOut: {int(output_size)}",
                 ha='center', fontsize=10)
        # Add the compression ratio above the bar
        plt.text(bar.get_x() + bar.get_width() / 2,
                 bar.get_height() + 5,
                 f"{ratio:.1f}%",
                 ha='center', fontsize=10, color='blue')

# Set Y-axis limit
plt.ylim(0, 100)

# Title and labels
plt.title("Compression Ratios for rle_good", fontsize=16)
plt.xlabel("Algorithm Sequence", fontsize=14)
plt.ylabel("Compression Ratio (%)", fontsize=14)
plt.xticks(rotation=45)
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()

# Show the plot
plt.show()
