// main.cpp
#include "compressor.h"
#include "skiplist.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4 || string(argv[1]) != "--encode") {
        cerr << "Usage: " << argv[0] << " --encode <input_file> <output_file>\n";
        return 1;
    }

    string inputFile = argv[2];
    string outputFile = argv[3];

    string content = loadFile(inputFile);
    if (content.empty()) return 1;

    unordered_map<char, int> charFreq;
    unordered_map<string, int> pairFreq;
    countFrequencies(content, charFreq, pairFreq);

    auto sortedPairs = mapToSortedVector(pairFreq);
    const int MAX_TOKENS = 128;
    vector<string> topBigrams;
    unordered_map<string, char> tokenMap;
    unordered_map<char, string> reverseTokenMap;

    int tokenIndex = 0;
    for (const auto& [bigram, freq] : sortedPairs) {
        if (tokenIndex >= MAX_TOKENS) break;
        char token = static_cast<char>(128 + tokenIndex);
        tokenMap[bigram] = token;
        reverseTokenMap[token] = bigram;
        topBigrams.push_back(bigram);
        ++tokenIndex;
    }

    string tokenized = applyTokenization(content, topBigrams, tokenMap);

    unordered_map<char, int> tokenFreq;
    for (char c : tokenized) ++tokenFreq[c];

    HuffNode* root = buildHuffmanWithSkiplist(tokenFreq);

    unordered_map<char, string> codeMap;
    unordered_map<string, char> reverseCodeMap;
    generateCodes(root, "", codeMap);
    generateReverseCodes(root, "", reverseCodeMap);

    string bitstream;
    for (char c : tokenized) bitstream += codeMap[c];

    saveBinaryFile(outputFile, bitstream, tokenMap, codeMap);

    cout << "Compression complete. Output saved to: " << outputFile << "\n";
    cout << "Original size:   " << content.size() << " bytes\n";
    cout << "Tokenized size: " << tokenized.size() << " bytes\n";
    cout << "Huffman-encoded: " << bitstream.size() << " bits (~" << (bitstream.size() + 7) / 8 << " bytes)\n";
    return 0;
}