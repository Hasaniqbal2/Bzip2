# BZip2 Data Compression - Phase 1

## Overview
This repository contains the **Stage 1** implementation of a BZip2-style data compression pipeline.

### Phase 1 Features
1. **Block Division:** Reads arbitrarily large binary files and chunks them into memory-safe blocks based on a configurable `block_size`.
2. **RLE-1 (Run-Length Encoding):** Implements the strict `[Character][Count]` format to compress consecutive sequences of identical bytes.
3. **Burrows-Wheeler Transform (BWT):** Utilizes a highly optimized **Suffix Array** approach via C++ `std::sort` instead of the memory-heavy matrix rotation approach.
4. **Configuration Manager:** Parses a `config.ini` file to dynamically toggle RLE-1, choose the BWT type, and set chunk sizes without recompiling.

---

## Directory Structure
```text
project-bzip2/
├── Makefile                # Build automation (C++17)
├── config.ini              # Pipeline configuration
├── include/
│   └── bzip2.hpp           # Core data structures and function prototypes
└── src/
    ├── main.cpp            # Pipeline coordinator and binary I/O
    ├── block.cpp           # File stream slicing and chunking
    ├── config.cpp          # INI parser
    ├── rle.cpp             # Run-Length Encoding engine
    └── bwt.cpp             # Suffix Array BWT and Inverse BWT
