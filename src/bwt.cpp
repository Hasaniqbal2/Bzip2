#include "../include/bzip2.hpp"
#include <algorithm>

BwtResult bwt_encode(const Block& input, int type) {
    (void)type; 
    
    size_t n = input.size();
    vector<int> sa(n);
    for (size_t i = 0; i < n; i++) sa[i] = i; 

    sort(sa.begin(), sa.end(), [&](int a, int b) {
        for (size_t k = 0; k < n; k++) {
            uint8_t c1 = input[(a + k) % n];
            uint8_t c2 = input[(b + k) % n];
            if (c1 != c2) return c1 < c2;
        }
        return false;
    });

    Block output(n);
    int primary = -1;
    for (size_t i = 0; i < n; i++) {
        output[i] = input[(sa[i] + n - 1) % n];
        if (sa[i] == 0) primary = i;
    }
    return {output, primary};
}

Block bwt_decode(const Block& input, int primaryIndex) {
    size_t n = input.size();
    vector<int> freq(256, 0);
    for (uint8_t b : input) freq[b]++;

    vector<int> first(256, 0);
    for (int i = 1; i < 256; i++) first[i] = first[i - 1] + freq[i - 1];

    vector<int> lf(n);
    vector<int> count(256, 0);
    for (size_t i = 0; i < n; i++) {
        uint8_t c = input[i];
        lf[i] = first[c] + count[c]++;
    }

    Block output(n);
    int row = primaryIndex;
    for (size_t i = n; i > 0; i--) {
        output[i - 1] = input[row];
        row = lf[row];
    }
    return output;
}