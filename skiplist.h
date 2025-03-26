#pragma once
#include <vector>
#include <random>

const int MAX_LEVEL = 6;

struct HuffNode {
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;
    HuffNode(char c, int f);
    HuffNode(HuffNode* l, HuffNode* r);
};

struct SkipNode {
    HuffNode* huff;
    vector<SkipNode*> forward;
    SkipNode(HuffNode* huff, int level);
};

class SkipList {
    SkipNode* head;
    int level;
    mt19937 rng;
    uniform_real_distribution<> dist;
    int randomLevel();

public:
    SkipList();
    void insert(HuffNode* huff);
    HuffNode* popMin();
    bool empty();
};