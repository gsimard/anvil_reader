#include <iostream>
#include <fstream>
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

    for( tag_iterator it(anvil) ; it.valid() ; ++it )
    {
        if( it->tag_type.e == TAG_Byte_Array && it->name == "Blocks" )
        {
            //cout << TAG_NAMES[it->tag_type.b] << ": " << (it->name == "" ? "____" : it->name)
            //     << " size: " << it->tag_byte_array_size << endl;

            byte *tag_byte_array = it->tag_byte_array;
            unsigned long int tag_byte_array_size = it->tag_byte_array_size;

            // Verify that we have a valid sized block
            if( tag_byte_array_size != 4096 )
                continue;

            Tag a_tag = *it;

            if( a_tag.parent )
            {
                std::cout << a_tag.parent << endl;

                for( std::vector<Tag*>::iterator tag_it = it->parent->tags.begin() ; tag_it < it->parent->tags.end() ; ++tag_it )
                {
                    std::cout << "has2 a parent" << endl;

                    if( (*tag_it)->tag_type.e == TAG_Byte && (*tag_it)->name == "Y" )
                    {
                        std::cout << "this block has a parent, and an Y entry: " << (unsigned long int)(*tag_it)->tag_byte << endl;
                    }
                }
            }
        }
    }

    // DEBUG: save the first valid chunk in a file
    for ( int i = 0 ; i < 1024 ; i++ )
    {
        if (anvil.chunks[i].data != NULL)
        {
            ofstream mc_chunk_file;
            mc_chunk_file.open( "chunk.dat", ios::binary );
            mc_chunk_file.write( (char*)anvil.chunks[i].data, anvil.chunks[i].length );
            mc_chunk_file.close();

            break;
        }
    }

    cout << "Number of chunks: " << anvil.NumChunks() << "/1024" << endl;
    cout << "Total space used by chunks: " << anvil.ChunksTotalSize() << endl;

    return 0;
}
