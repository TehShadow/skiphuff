// decoder.cpp
#include "compressor.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4 || string(argv[1]) != "--decode") {
        cerr << "Usage: " << argv[0] << " --decode <encoded_file> <output_file>\n";
        return 1;
    }

    string encodedFile = argv[2];
    string outputFile = argv[3];

    string bitstream;
    unordered_map<char, string> reverseTokenMap;
    unordered_map<string, char> reverseCodeMap;

    if (!readBinaryFileWithHeader(encodedFile, bitstream, reverseTokenMap, reverseCodeMap)) {
        cerr << "Failed to read encoded file.\n";
        return 1;
    }

    // Decode bitstream using Huffman code map
    string tokenized;
    string buffer;
    for (char bit : bitstream) {
        buffer += bit;
        if (reverseCodeMap.count(buffer)) {
            tokenized += reverseCodeMap[buffer];
            buffer.clear();
        }
    }

    // Reverse token replacement
    string original = reverseTokenization(tokenized, reverseTokenMap);

    ofstream out(outputFile);
    out << original;
    out.close();

    cout << "Decompression complete. Output saved to: " << outputFile << "\n";
    return 0;
}