#include <iostream>
#include <fstream>
#include <cstring>
#include "anvil.hpp"
using namespace std;


int main( int argc, char* argv[] )
{
    if( argc != 2 )
    {
        cerr << "Usage: " << argv[0] << " file" << endl;
        return 1;
    }

    ifstream mc_anvil_file;
    mc_anvil_file.open( argv[1], ios::binary );

    Anvil anvil;
    anvil.Read( mc_anvil_file );
    mc_anvil_file.close();

    // statistics
    unsigned long int block_count[256][256];
    memset( block_count, 0, sizeof(block_count)*256*256 );

    for( tag_iterator it(anvil) ; it.valid() ; ++it )
    {
        if( it->tag_type.e == TAG_Byte_Array && it->name == "Blocks" )
        {
            byte *tag_byte_array = it->tag_byte_array;
            unsigned long int tag_byte_array_size = it->tag_byte_array_size;

            // Verify that we have a valid sized block
            if( tag_byte_array_size != 4096 )
                continue;

            Tag a_tag = *it;

            if( a_tag.parent )
            {
                for( std::vector<Tag*>::iterator tag_it = it->parent->tags.begin() ; tag_it < it->parent->tags.end() ; ++tag_it )
                {
                    if( (*tag_it)->tag_type.e == TAG_Byte && (*tag_it)->name == "Y" )
                    {
                        // Y is the vertical position of the 16x16x16 block in 16 increments from bottom to top
                        int Y = (int)(*tag_it)->tag_byte;

                        for( int pos = 0 ; pos < 4096 ; pos++ )
                        {
                            int x = pos & 0xf;
                            int z = (pos & 0xf0) >> 4;
                            int y = ((pos & 0xf00) >> 8) + Y*16;

                            // increment count for this type of block at height y
                            block_count[tag_byte_array[pos]][y]++;
                        }
                    }
                }
            }
        }
    }

    ofstream mc_block_file;
    mc_block_file.open( "block_count.mat", ios::out );

    mc_block_file << "[";

    for( int i = 0 ; i < 256 ; i++ )
    {
        mc_block_file << "[";

        for( int j = 0 ; j < 256 ; j++ )
            mc_block_file << block_count[i][j] << " ";

        mc_block_file << "]" << std::endl;
    }

    mc_block_file << "]" << std::endl;
    mc_block_file.close();

    // Debug: save the first valid chunk in a file
    // for ( int i = 0 ; i < 1024 ; i++ )
    // {
    //     if (anvil.chunks[i].data != NULL)
    //     {
    //         ofstream mc_chunk_file;
    //         mc_chunk_file.open( "chunk.dat", ios::binary );
    //         mc_chunk_file.write( (char*)anvil.chunks[i].data, anvil.chunks[i].length );
    //         mc_chunk_file.close();

    //         break;
    //     }
    // }

    cout << "Number of chunks: " << anvil.NumChunks() << "/1024" << endl;
    cout << "Total space used by chunks: " << anvil.ChunksTotalSize() << endl;

    return 0;
}
