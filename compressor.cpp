#include "compressor.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <set>
#include <sstream>
#include "skiplist.h"

using namespace std;

string loadFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file.\n";
        return "";
    }
    return string((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
}

void saveBinaryFile(const string& filename, const string& bitstream,const unordered_map<string, char>& tokenMap,const unordered_map<char, string>& codeMap) {
    ofstream out(filename, ios::binary);
    out << "CPS1"; // Magic header and version

    // Token map size
    uint8_t tokenCount = tokenMap.size();
    out.put(tokenCount);
    for (const auto& [pair, token] : tokenMap) {
        out.write(pair.c_str(), 2);
        out.put(token);
    }

    // Code map size
    uint16_t codeCount = codeMap.size();
    out.put(codeCount >> 8);
    out.put(codeCount & 0xFF);
    for (const auto& [ch, code] : codeMap) {
        out.put(ch);
        uint8_t len = code.length();
        out.put(len);
        out.write(code.c_str(), len);
    }

    // Write bitstream
    for (size_t i = 0; i < bitstream.size(); i += 8) {
        bitset<8> byte(bitstream.substr(i, 8));
        out.put(static_cast<unsigned char>(byte.to_ulong()));
    }
    out.close();
}

bool readBinaryFileWithHeader(const string& filename, string& bitstream,unordered_map<char, string>& reverseTokenMap,unordered_map<string, char>& reverseCodeMap) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    char header[4];
    in.read(header, 4);
    if (string(header, 4) != "CPS1") {
        cerr << "Invalid file format.\n";
        return false;
    }

    // Read token map
    uint8_t tokenCount = in.get();
    for (int i = 0; i < tokenCount; ++i) {
        char bigram[2], token;
        in.read(bigram, 2);
        token = in.get();
        reverseTokenMap[token] = string(bigram, 2);
    }

    // Read code map
    uint16_t codeCount = (in.get() << 8) | in.get();
    for (int i = 0; i < codeCount; ++i) {
        char ch = in.get();
        uint8_t len = in.get();
        string code(len, '\0');
        in.read(&code[0], len);
        reverseCodeMap[code] = ch;
    }

    // Read bitstream
    char byte;
    while (in.get(byte)) {
        bitset<8> b(static_cast<unsigned char>(byte));
        bitstream += b.to_string();
    }
    return true;
}
void countFrequencies(const string& data,unordered_map<char, int>& charFreq,unordered_map<string, int>& pairFreq) {
for (size_t i = 0; i < data.size(); ++i) {
++charFreq[data[i]];
if (i + 1 < data.size()) {
string pair = data.substr(i, 2);
++pairFreq[pair];
}
}
}

string applyTokenization(const string& data,const vector<string>& topBigrams,unordered_map<string, char>& tokenMap) {
string result;
size_t i = 0;
set<string> bigramSet(topBigrams.begin(), topBigrams.end());

while (i < data.size()) {
    if (i + 1 < data.size()) {
        string bigram = data.substr(i, 2);
    if (bigramSet.count(bigram)) {
        result += tokenMap[bigram];
        i += 2;
    continue;
}
}
result += data[i];
++i;
}

return result;
}

string reverseTokenization(const string& data,const unordered_map<char, string>& reverseTokenMap) {
string result;
for (char c : data) {
    if (reverseTokenMap.count(c)) {
        result += reverseTokenMap.at(c);
    } else {
    result += c;
    }   
}
    return result;
}

HuffNode* buildHuffmanWithSkiplist(const unordered_map<char, int>& freqMap) {
    SkipList skiplist;
    for (const auto& [c, freq] : freqMap) {
        skiplist.insert(new HuffNode(c, freq));
    }

    while (!skiplist.empty()) {
        HuffNode* left = skiplist.popMin();
        HuffNode* right = skiplist.popMin();
        if (!right) {
            return left;
        }
        HuffNode* merged = new HuffNode(left, right);
        skiplist.insert(merged);
    }

    return nullptr;
}

void generateCodes(HuffNode* node, const string& code,unordered_map<char, string>& codeMap) {
if (!node) return;
if (!node->left && !node->right) {
codeMap[node->ch] = code;
return;
}
generateCodes(node->left, code + "0", codeMap);
generateCodes(node->right, code + "1", codeMap);
}

void generateReverseCodes(HuffNode* node, const string& code,
           unordered_map<string, char>& reverseCodeMap) {
if (!node) return;
    if (!node->left && !node->right) {
    reverseCodeMap[code] = node->ch;
    return;
    }
generateReverseCodes(node->left, code + "0", reverseCodeMap);
generateReverseCodes(node->right, code + "1", reverseCodeMap);
}