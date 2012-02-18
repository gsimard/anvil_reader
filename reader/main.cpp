#include <iostream>
#include <fstream>
#include "anvil.hpp"
using namespace std;


int main( int argc, char* argv[] )
{
    if (argc != 2)
    {
        cout << "Usage: reader file" << endl;
        return 1;
    }

    ifstream mc_anvil_file;
    mc_anvil_file.open( argv[1] );

    Anvil anvil;
    mc_anvil_file >> anvil;
    mc_anvil_file.close();

    cout << "Number of chunks: " << anvil.NumChunks() << endl;
    cout << "Total space used by chunks: " << anvil.ChunksTotalSize() << endl;

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        if (anvil.chunks[i].id.offset != 0)
        {

        }
    }

    return 0;
}
