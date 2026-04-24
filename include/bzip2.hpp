#ifndef BZIP2_HPP
#define BZIP2_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

using Block = vector<uint8_t>;

struct Config {
    size_t block_size = 500000;
    bool rle1_enabled = true;
    int bwt_type = 0; 
    bool output_metrics = true;
};

Config loadConfig(const string& filename);
vector<Block> divideIntoBlocks(const string& filename, size_t blockSize);

Block rle1_encode(const Block& input);
Block rle1_decode(const Block& input);

struct BwtResult {
    Block data;
    int primaryIndex;
};

BwtResult bwt_encode(const Block& input, int type);
Block bwt_decode(const Block& input, int primaryIndex);

#endif