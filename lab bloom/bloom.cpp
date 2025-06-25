#include "bloom.h"
#include <algorithm>
#include <bitset>

BF::BF(uint64_t size, std::vector<hashFunction> hv) : size(size), hv(hv) {
    bv.resize(size, false);
}

BF::BF(const BF& other) : size(other.size), hv(other.hv), bv(other.bv) {}

BF::~BF() {}

void BF::add(const int& key) {
    for (const auto& hash : hv) {
        uint64_t index = hash(key) % size;
        bv[index] = true;
    }
}

bool BF::contains(const int& key) const {
    for (const auto& hash : hv) {
        uint64_t index = hash(key) % size;
        if (!bv[index]) {
            return false;
        }
    }
    return true;
}

void BF::bit_union(const BF& other) {
    for (uint64_t i = 0; i < size; ++i) {
        bv[i] = bv[i] || other.bv[i];
    }
}

void BF::intersect(const BF& other) {
    for (uint64_t i = 0; i < size; ++i) {
        bv[i] = bv[i] && other.bv[i];
    }
}

std::ostream& operator<<(std::ostream& out, const BF& b) {
    for (bool bit : b.bv) {
        out << bit;
    }
    return out;
}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hv, unsigned max) {
    BF bf(size, hv);
    for (const auto& key : inList) {
        bf.add(key);
    }

    unsigned fp = 0, tn = 0;
    for (unsigned i = 0; i < max; ++i) {
        if (std::find(inList.begin(), inList.end(), i) == inList.end()) {
            if (bf.contains(i)) {
                ++fp;
            } else {
                ++tn;
            }
        }
    }

    return static_cast<float>(fp) / (fp + tn);
}

bool getBitFromArray(std::vector<char> bv, int index) {
    int byteIndex = index / 8;
    int bitIndex = index % 8;
    return (bv[byteIndex] & (1 << (7 - bitIndex))) != 0;
}

bool getBitFromByte(char in, int index) {
    return (in & (1 << (7 - index))) != 0;
}

int easy(int key) {
    return key;
}

int cpp(int key) {
    std::size_t h1 = std::hash<int>{}(key);
    return int(h1);
}

int simple(int key) {
    return 5 * key + 3;
}

int simple2(int key) {
    return 3 * key + 1;
}

int simple3(int key) {
    return 2654435769 * key;
}
