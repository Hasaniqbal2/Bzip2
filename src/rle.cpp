#include "../include/bzip2.hpp"

Block rle1_encode(const Block& input) {
    Block output;
    size_t i = 0;
    while (i < input.size()) {
        uint8_t cur = input[i];
        size_t run = 1;
        
        while (i + run < input.size() && input[i + run] == cur && run < 255) {
            run++;
        }
        output.push_back(cur);          
        output.push_back((uint8_t)run); 
        i += run;
    }
    return output;
}

Block rle1_decode(const Block& input) {
    Block output;
    for (size_t i = 0; i + 1 < input.size(); i += 2) {
        uint8_t cur = input[i];
        uint8_t run = input[i + 1];
        for (int k = 0; k < run; k++) {
            output.push_back(cur);
        }
    }
    return output;
}