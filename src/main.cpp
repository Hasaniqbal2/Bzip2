#include "../include/bzip2.hpp"
#include <fstream>

void compress(const string& in, const string& out, Config& cfg) {
    auto blocks = divideIntoBlocks(in, cfg.block_size);
    if (blocks.empty()) return;

    ofstream outFile(out, ios::binary);
    outFile.write("BZ1\0", 4);
    int numBlocks = blocks.size();
    outFile.write(reinterpret_cast<char*>(&numBlocks), sizeof(int));

    size_t total_orig = 0, total_comp = 4 + sizeof(int);

    for (auto& block : blocks) {
        size_t origSize = block.size();
        total_orig += origSize;

        if (cfg.rle1_enabled) block = rle1_encode(block);
        
        BwtResult res = bwt_encode(block, cfg.bwt_type);
        size_t compSize = res.data.size();

        outFile.write(reinterpret_cast<char*>(&res.primaryIndex), sizeof(int));
        outFile.write(reinterpret_cast<char*>(&origSize), sizeof(size_t));
        outFile.write(reinterpret_cast<char*>(&compSize), sizeof(size_t));
        outFile.write(reinterpret_cast<char*>(res.data.data()), compSize);

        total_comp += sizeof(int) + (sizeof(size_t) * 2) + compSize;
    }

    if (cfg.output_metrics) {
        cout << "\n--- Phase 1 Compression Results ---\n";
        cout << "Original Size   : " << total_orig << " bytes\n";
        cout << "Compressed Size : " << total_comp << " bytes\n";
        cout << "BWT Method Used : " << (cfg.bwt_type == 1 ? "Suffix Array" : "Matrix") << "\n";
    }
}

void decompress(const string& in, const string& out, Config& cfg) {
    ifstream inFile(in, ios::binary);
    if (!inFile) return;

    char magic[4];
    inFile.read(magic, 4);
    if (string(magic, 4) != string("BZ1\0", 4)) return;

    int numBlocks = 0;
    inFile.read(reinterpret_cast<char*>(&numBlocks), sizeof(int));

    ofstream outFile(out, ios::binary);

    for (int i = 0; i < numBlocks; i++) {
        int primary = 0;
        size_t origSize = 0, storSize = 0;

        inFile.read(reinterpret_cast<char*>(&primary), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&origSize), sizeof(size_t));
        inFile.read(reinterpret_cast<char*>(&storSize), sizeof(size_t));

        Block data(storSize);
        inFile.read(reinterpret_cast<char*>(data.data()), storSize);

        if (primary >= 0) data = bwt_decode(data, primary);
        if (cfg.rle1_enabled) data = rle1_decode(data);

        outFile.write(reinterpret_cast<char*>(data.data()), data.size());
    }
}


int main(int argc, char** argv) {
    if (argc < 3) return 1;

    string mode = argv[1];
    string file1 = argv[2];
    string file2 = (argc >= 4) ? argv[3] : "";
    string cfgfile = (argc >= 5) ? argv[4] : "config.ini";

    if (mode == "test" && argc == 4) cfgfile = argv[3];

    Config cfg = loadConfig(cfgfile);

    if (mode == "compress" && argc >= 4) {
        compress(file1, file2, cfg);
    } else if (mode == "decompress" && argc >= 4) {
        decompress(file1, file2, cfg);
    } else if (mode == "test") {
        compress(file1, "__test.bz1", cfg);
        decompress("__test.bz1", "__test.out", cfg);
        cout << "Test complete. Check __test.out against original.\n";
    }
    return 0;
}