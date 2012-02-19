#include <iostream>
#include <fstream>
#include "anvil.hpp"
using namespace std;


int main( int argc, char* argv[] )
{
    if( argc != 2 )
    {
        cout << "Usage: " << argv[0] << " file" << endl;
        return 1;
    }

    ifstream mc_anvil_file;
    mc_anvil_file.open( argv[1], ios::binary );

    Anvil anvil;
    anvil.Read( mc_anvil_file );
    mc_anvil_file.close();

    cout << "Number of chunks: " << anvil.NumChunks() << endl;
    cout << "Total space used by chunks: " << anvil.ChunksTotalSize() << endl;

    tag_iterator it(anvil);

    while( true )
    {
        cout << it++->name << endl;
    }

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        if (anvil.chunks[i].data != NULL)
        {
            ofstream mc_chunk_file;
            mc_chunk_file.open( "chunk.dat", ios::binary );
            mc_chunk_file.write( (char*)anvil.chunks[i].data, anvil.chunks[i].length );
            mc_chunk_file.close();
        }
    }

    return 0;
}
