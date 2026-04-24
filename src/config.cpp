#include "../include/bzip2.hpp"
#include <fstream>
#include <algorithm>

Config loadConfig(const string& filename) {
    Config cfg;
    ifstream f(filename);
    if (!f) return cfg;

    string line;
    while (getline(f, line)) {
        if (line.empty() || line[0] == '[' || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq == string::npos) continue;

        string key = line.substr(0, eq);
        string val = line.substr(eq + 1);
        
        if (key.find("block_size") != string::npos) cfg.block_size = stoul(val);
        if (val.find("suffix_array") != string::npos) cfg.bwt_type = 1;
    }
    return cfg;
}