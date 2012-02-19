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
    // a vector of iterators to vectors of tags, used for walking the tree
    // the iterator at the back is the tip
    std::vector<std::vector<Tag>::iterator> it;
    std::vector<std::vector<Tag>::iterator> it_end;

public:
    tag_iterator() : anvil(NULL) {}
    tag_iterator(Anvil& a) : anvil(&a)
        {
            for( at_chunk = 0 ; at_chunk < 1024 ; at_chunk++ )
                if( !anvil->chunks[at_chunk].tags.empty() )
                {
                    // the order of the push backs is CRITICAL
                    it_end.push_back( anvil->chunks[at_chunk].tags.end() );
                    it.push_back( anvil->chunks[at_chunk].tags.begin() );

                    std::cout << "at_chunk: " << at_chunk << std::endl;
                    std::cout << "depth: " << it.size() << std::endl;
                    break;
                }

            if( at_chunk == 1024 )
                anvil = NULL;

            *this;
        }
    tag_iterator(const tag_iterator& x)
        : anvil(x.anvil), at_chunk(x.at_chunk), it(x.it), it_end(x.it_end) {}
    ~tag_iterator() {}

    bool valid() { return anvil != NULL; }

    const Tag& operator*() const { return *(it.back()); }
    const Tag* operator->() const { return &*(it.back()); }
    tag_iterator& operator++() {

        if( anvil )
        {
            // the first place to look for the next tag is in THIS tag's tag list (walk down the tree)
            if( !it.back()->tags.empty() )
            {
                // the order of the push backs is CRITICAL
                it_end.push_back( it.back()->tags.end() );
                it.push_back( it.back()->tags.begin() );
            }
            // THIS tag's tag list is empty, which means we have arrived at a leaf of the tree
            else
            {
                // if we have reached the end of this tag's tag vector
                while ( ++(it.back()) == it_end.back() )
                {
                    // get back one level
                    it.pop_back();
                    it_end.pop_back();

                    // if the depth is now zero, this chunk has no more tags, go to the next chunk
                    if ( it.size() == 0 )
                    {
                        // find the next chunk which has tags in its vector
                        while ( ++at_chunk < 1024 )
                        {
                            if( !anvil->chunks[at_chunk].tags.empty() )
                            {
                                std::cout << "at_chunk: " << at_chunk << std::endl;

                                // the order of the push backs is CRITICAL
                                it_end.push_back( anvil->chunks[at_chunk].tags.end() );
                                it.push_back( anvil->chunks[at_chunk].tags.begin() );
                                break;
                            }
                        }

                        // we have exchausted all the chunks, set anvil to NULL so valid() can return false
                        if ( at_chunk == 1024 )
                            anvil = NULL;

                        std::cout << "depth: " << it.size() << std::endl;
                        return *this;
                    }
                }
            }
        }

        std::cout << "depth: " << it.size() << std::endl;
        return *this;
    }
    tag_iterator operator++(int) {
        tag_iterator tmp = *this;
        ++*this;
        return tmp;
    }
};
