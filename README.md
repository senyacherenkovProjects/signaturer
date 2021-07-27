# signaturer
C++ console program for generating the signature of the specified file. The signature is generated as follows: the source file is divided into blocks of equal (fixed) length (if the file size is not a multiple of the block size, the last fragment can be smaller or padded with zeros to the size of the full block). For each block, the hash function value is calculated and added to the output signature file.

Interface: a command line that specifies:
* Path to the input file
* Path to the output file
* Block size (default, 1 MB)
