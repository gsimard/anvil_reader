#include <iostream>
#include <vector>
#include "tag.hpp"
using namespace std;

typedef unsigned char byte;

struct ChunkID {
    unsigned long int offset;
    byte sector_count;
    unsigned long int timestamp;

    ChunkID();
};

enum compression_e {none = 0, gzip = 1, zlib = 2};

union compression_t {
    compression_e e;
    byte b;
};

struct Chunk {
    ChunkID id;

    unsigned long int length;
    compression_t compression_type;
    byte *data;

    vector<Tag> tags;

    istream& Read( istream& input );

    Chunk();
    ~Chunk();
};

struct Anvil {
    Chunk chunks[1024];

    unsigned short int NumChunks();
    unsigned long int ChunksTotalSize();
    unsigned short int ChunkAt(int x, int z);

    istream& Read( istream& input );
};
