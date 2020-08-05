----------------------HUFFMAN COMPRESSOR/DECOMPRESSOR--------------------------

all copyrights goes to © Mohammed Osama Abuelwafa

A c++ object oriented code that compresses a text file consisting of 128-ascii characters and is able to decompress it.

# HOW TO USE?

- the console prompts to the user inputting 1 for compression and 2 for decompression.
in case of compression: The Program calculates the compression ratio and the coding efficency and outputs them in the console.

passing of the parameters is as the following:

# compression:

source_file : whole path + .txt   (if the file is in the project directory, mentioning the name only is enough)
target file : whole path + .huf   (if the file is in the project directory, mentioning the name only is enough)


# Decompression:

source_file : whole path + .huf   (if the file is in the project directory, mentioning the name only is enough)
target file : whole path + .txt   (if the file is in the project directory, mentioning the name only is enough)

# implementation details:

compression:

1. Open input file, count and store the frequencies of different characters
2. Construct Priority Queue and then Huffman merge Tree
3. Calculate Huffman Encode Table
4. Encode the file, store Huffman Encode Table and encoded to the target file.
5. calculate the compression ratio and the coding efficency and outputs them in the console.


Decompression:

1. Open file, recreate Huffman Tree based on Huffman Encode Table;
2. Decode the file based on the tree and store it to the target file.
