#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string loadFile(const string& filename);

void saveBinaryFile(const string& filename, const string& bitstream,
                    const unordered_map<string, char>& tokenMap,
                    const unordered_map<char, string>& codeMap);
bool readBinaryFileWithHeader(const string& filename, string& bitstream,
                              unordered_map<char, string>& reverseTokenMap,
                              unordered_map<string, char>& reverseCodeMap);

void countFrequencies(const string& data,
                      unordered_map<char, int>& charFreq,
                      unordered_map<string, int>& pairFreq);

template<typename K, typename V>
vector<pair<K, V>> mapToSortedVector(const unordered_map<K, V>& map) {
    vector<pair<K, V>> vec(map.begin(), map.end());
    sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
        return b.second < a.second;
    });
    return vec;
}

string applyTokenization(const string& data,
                        const vector<string>& topBigrams,
                        unordered_map<string, char>& tokenMap);

string reverseTokenization(const string& data,
                        const unordered_map<char, 
                        string>& reverseTokenMap);

struct HuffNode;
void generateCodes(HuffNode* node, 
                    const string& code,
                    unordered_map<char, string>& codeMap);

void generateReverseCodes(HuffNode* node, 
                            const string& code,
                            unordered_map<string, char>& reverseCodeMap);
                            
HuffNode* buildHuffmanWithSkiplist(const unordered_map<char, int>& freqMap);
