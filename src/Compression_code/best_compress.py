import pandas as pd
from io import StringIO

# Input the data as a string
data = """
File,Algorithm Sequence,Input Size (bytes),Output Size (bytes),Compression Ratio (%)
Capital-Volume-I,RLE, 2077155, 2074398,1.00
Capital-Volume-I,ARI, 2077155, 1162932,45.00
Capital-Volume-I,Huffman, 2077155, 1174080,44.00
Capital-Volume-I,BWT + MTF + RLE, 2077155, 1258083,40.00
Capital-Volume-I,BWT + MTF + ARI, 2077155,  646812,69.00
Capital-Volume-I,BWT + MTF + Huffman, 2077155,  672069,68.00
Capital-Volume-II,RLE, 1286856, 1285221,1.00
Capital-Volume-II,ARI, 1286856,  711006,45.00
Capital-Volume-II,Huffman, 1286856,  718743,45.00
Capital-Volume-II,BWT + MTF + RLE, 1286856,  647842,50.00
Capital-Volume-II,BWT + MTF + ARI, 1286856,  332646,75.00
Capital-Volume-II,BWT + MTF + Huffman, 1286856,  359028,73.00
Capital-Volume-III,RLE, 2251248, 2248779,1.00
Capital-Volume-III,ARI, 2251248, 1243698,45.00
Capital-Volume-III,Huffman, 2251248, 1255716,45.00
Capital-Volume-III,BWT + MTF + RLE, 2251248, 1205856,47.00
Capital-Volume-III,BWT + MTF + ARI, 2251248,  615795,73.00
Capital-Volume-III,BWT + MTF + Huffman, 2251248,  655057,71.00
ari_good,RLE,     125,     125,0
ari_good,ARI,     125,      69,45.00
ari_good,Huffman,     125,    1054,-743.00
ari_good,BWT + MTF + RLE,     125,      87,31.00
ari_good,BWT + MTF + ARI,     125,      75,40.00
ari_good,BWT + MTF + Huffman,     125,    1055,-744.00
bib,RLE,  111261,  111140,1.00
bib,ARI,  111261,   72789,35.00
bib,Huffman,  111261,   73785,34.00
bib,BWT + MTF + RLE,  111261,   55132,51.00
bib,BWT + MTF + ARI,  111261,   31981,72.00
bib,BWT + MTF + Huffman,  111261,   34090,70.00
book1,RLE,  768771,  768569,1.00
book1,ARI,  768771,  436883,44.00
book1,Huffman,  768771,  439398,43.00
book1,BWT + MTF + RLE,  768771,  560282,28.00
book1,BWT + MTF + ARI,  768771,  278301,64.00
book1,BWT + MTF + Huffman,  768771,  285879,63.00
book2,RLE,  610856,  610199,1.00
book2,ARI,  610856,  364720,41.00
book2,Huffman,  610856,  369324,40.00
book2,BWT + MTF + RLE,  610856,  369744,40.00
book2,BWT + MTF + ARI,  610856,  190860,69.00
book2,BWT + MTF + Huffman,  610856,  198325,68.00
huffman_good,RLE,      76,      44,43.00
huffman_good,ARI,      76,      54,29.00
huffman_good,Huffman,      76,    1047,-1277.00
huffman_good,BWT + MTF + RLE,      76,      38,50.00
huffman_good,BWT + MTF + ARI,      76,      52,32.00
huffman_good,BWT + MTF + Huffman,      76,    1044,-1273.00
paper1,RLE,   53161,   52925,1.00
paper1,ARI,   53161,   33120,38.00
paper1,Huffman,   53161,   34361,36.00
paper1,BWT + MTF + RLE,   53161,   33528,37.00
paper1,BWT + MTF + ARI,   53161,   17981,67.00
paper1,BWT + MTF + Huffman,   53161,   19101,65.00
paper2,RLE,   82199,   82172,1.00
paper2,ARI,   82199,   47535,43.00
paper2,Huffman,   82199,   48639,41.00
paper2,BWT + MTF + RLE,   82199,   52889,36.00
paper2,BWT + MTF + ARI,   82199,   27569,67.00
paper2,BWT + MTF + Huffman,   82199,   29007,65.00
paper3,RLE,   46526,   46525,1.00
paper3,ARI,   46526,   27393,42.00
paper3,Huffman,   46526,   28299,40.00
paper3,BWT + MTF + RLE,   46526,   32530,31.00
paper3,BWT + MTF + ARI,   46526,   17251,63.00
paper3,BWT + MTF + Huffman,   46526,   18140,62.00
paper4,RLE,   13286,   13284,1.00
paper4,ARI,   13286,    7998,40.00
paper4,Huffman,   13286,    8884,34.00
paper4,BWT + MTF + RLE,   13286,   10019,25.00
paper4,BWT + MTF + ARI,   13286,    5519,59.00
paper4,BWT + MTF + Huffman,   13286,    6372,53.00
paper5,RLE,   11954,   11936,1.00
paper5,ARI,   11954,    7559,37.00
paper5,Huffman,   11954,    8455,30.00
paper5,BWT + MTF + RLE,   11954,    8889,26.00
paper5,BWT + MTF + ARI,   11954,    5082,58.00
paper5,BWT + MTF + Huffman,   11954,    5940,51.00
paper6,RLE,   38105,   38035,1.00
paper6,ARI,   38105,   23833,38.00
paper6,Huffman,   38105,   25047,35.00
paper6,BWT + MTF + RLE,   38105,   24742,36.00
paper6,BWT + MTF + ARI,   38105,   13223,66.00
paper6,BWT + MTF + Huffman,   38105,   14188,63.00
rle_good,RLE,     185,      25,87.00
rle_good,ARI,     185,     131,30.00
rle_good,Huffman,     185,    1105,-497.00
rle_good,BWT + MTF + RLE,     185,      55,71.00
rle_good,BWT + MTF + ARI,     185,      83,56.00
rle_good,BWT + MTF + Huffman,     185,    1064,-475.00
"""


# Load the data into a pandas DataFrame
df = pd.read_csv(StringIO(data))

# Group by 'Algorithm Sequence' and find the best compression ratio for each category
best_per_algorithm = df.loc[df.groupby('Algorithm Sequence')['Compression Ratio (%)'].idxmax()]

# Display the best compression results
print(best_per_algorithm)

# Save the results to a CSV file (optional)
best_per_algorithm.to_csv('best_per_algorithm.csv', index=False)