#include <iostream>
#include <vector>
#include "tag.hpp"

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

    std::vector<Tag> tags;

    std::istream& Read( std::istream& input );

    Chunk();
    ~Chunk();
};

struct Anvil {
    Chunk chunks[1024];

    unsigned short int NumChunks();
    unsigned long int ChunksTotalSize();
    unsigned short int ChunkAt(int x, int z);

    std::istream& Read( std::istream& input );
};

class tag_iterator :
    public std::iterator<std::input_iterator_tag, Tag>
{
    Anvil *anvil;
    unsigned short int at_chunk;
    std::vector<Tag>::iterator it;

public:
    tag_iterator() : anvil(NULL), at_chunk(0) {}
    tag_iterator(Anvil& a) : anvil(&a), at_chunk(0) { it = anvil->chunks[0].tags.begin(); ++*this; }
    tag_iterator(const tag_iterator& x)
        : anvil(x.anvil), at_chunk(x.at_chunk), it(x.it) {}
    ~tag_iterator() {}

    const Tag& operator*() const { return *it; }
    const Tag* operator->() const { return &*it; }
    tag_iterator& operator++() {
        // FIXME
//        if (in_stream && !(*in_stream >> value)) in_stream=0;

        std::cout << "1" << std::endl;

        // if we have reached the end of this chunk's tag vector
        if( anvil && ++it == anvil->chunks[at_chunk].tags.end() )
        {
            // find the next chunk which has tags in its vector
            while ( ++at_chunk < 1024 )
            {
                std::cout << "at_chunk: " << at_chunk << std::endl;

                it = anvil->chunks[at_chunk].tags.begin();

                // we have found a chunk with tags, stop searching
                if( it != anvil->chunks[at_chunk].tags.end() )
                    break;
            }

            // we have exchausted all the chunks, release anvil, and "it" should be NULL by now
            if ( at_chunk == 1024 )
                anvil = NULL;
        }

        std::cout << "2" << std::endl;

        return *this;
    }
    tag_iterator operator++(int) {
        tag_iterator tmp = *this;
        ++*this;
        return tmp;
    }
};
