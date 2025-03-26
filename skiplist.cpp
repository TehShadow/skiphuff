#include "skiplist.h"

using namespace std;

HuffNode::HuffNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
HuffNode::HuffNode(HuffNode* l, HuffNode* r) : ch(0), freq(l->freq + r->freq), left(l), right(r) {}

SkipNode::SkipNode(HuffNode* huff, int level) : huff(huff), forward(level + 1, nullptr) {}

SkipList::SkipList() : level(0), rng(random_device{}()), dist(0.0, 1.0) {
    head = new SkipNode(nullptr, MAX_LEVEL);
}

int SkipList::randomLevel() {
    int lvl = 0;
    while (dist(rng) < 0.5 && lvl < MAX_LEVEL) ++lvl;
    return lvl;
}

void SkipList::insert(HuffNode* huff) {
    vector<SkipNode*> update(MAX_LEVEL + 1);
    SkipNode* x = head;
    for (int i = level; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->huff->freq < huff->freq) {
            x = x->forward[i];
        }
        update[i] = x;
    }

    int newLevel = randomLevel();
    if (newLevel > level) {
        for (int i = level + 1; i <= newLevel; ++i) {
            update[i] = head;
        }
        level = newLevel;
    }

    SkipNode* newNode = new SkipNode(huff, newLevel);
    for (int i = 0; i <= newLevel; ++i) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

HuffNode* SkipList::popMin() {
    if (!head->forward[0]) return nullptr;
    SkipNode* minNode = head->forward[0];
    for (int i = 0; i <= level; ++i) {
        if (head->forward[i] == minNode) {
            head->forward[i] = minNode->forward[i];
        }
    }
    while (level > 0 && !head->forward[level]) --level;
    HuffNode* huff = minNode->huff;
    delete minNode;
    return huff;
}

bool SkipList::empty() {
    return head->forward[0] == nullptr;
}