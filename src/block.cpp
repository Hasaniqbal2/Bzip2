#include "../include/bzip2.hpp"
#include <fstream>

vector<Block> divideIntoBlocks(const string& filename, size_t blockSize) {
    vector<Block> blocks;
    ifstream file(filename, ios::binary);
    if (!file) return blocks;

    while (file) {
        Block chunk(blockSize);
        file.read(reinterpret_cast<char*>(chunk.data()), blockSize);
        size_t n = file.gcount();
        if (n == 0) break;
        chunk.resize(n);
        blocks.push_back(std::move(chunk));
    }
    return blocks;
}