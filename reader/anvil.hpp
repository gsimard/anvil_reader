#include <iostream>
using namespace std;

typedef unsigned char byte;

struct ChunkID {
    unsigned long int offset;
    byte sector_count;
    unsigned long int timestamp;
};

enum compression_e {gzip = 1, zlib = 2};

union compression_t {
    compression_e e;
    byte b;
};

struct Chunk {
    ChunkID id;

    unsigned long int length;
    compression_t compression_type;
    byte *data;

	friend istream& operator>>( istream &input,  Chunk& obj );
};

struct Anvil {
    Chunk chunks[1024];

    unsigned short int NumChunks();
    unsigned long int ChunksTotalSize();
    unsigned short int ChunkAt(int x, int z);

	friend istream& operator>>( istream &input,  Anvil& obj );
};
