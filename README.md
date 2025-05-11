# Huffman Compression with Skip List Priority Queue

A custom-built lossless file compression and decompression tool in C++, using:
- **Huffman encoding** for efficient symbol compression
- **Skip list** as a priority queue for building the Huffman tree
- Custom binary file format with headers for metadata and compression details

## 📦 Features

- Compresses text or binary files using Huffman coding
- Uses a skip list data structure instead of a traditional heap for node sorting
- Custom serialization format with headers to store frequency maps
- Decompressor restores original file exactly
- Modular code with clean class structure
