#include <iostream>
using namespace std;

typedef unsigned char byte;

struct ChunkID {
    unsigned long int offset;
    byte sector_count;
    unsigned long int timestamp;
};

struct Chunk {
    ChunkID id;
};

struct Anvil {
    Chunk chunks[1024];

    unsigned short int NumChunks();
    unsigned long int ChunksTotalSize();
    unsigned short int ChunkAt(int x, int z);

	friend istream& operator>>( istream &input,  Anvil& obj );
};
