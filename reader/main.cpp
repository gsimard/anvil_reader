#include <iostream>
#include <fstream>
#include <sstream>
#include "anvil.hpp"
using namespace std;


int main( int argc, char* argv[] )
{
    byte *data = new byte[4];
    data[0] = 0;
    data[1] = 0;
    data[2] = 1;
    data[3] = 0;

    byte *data2 = new byte[4];

    string m_string = string( (char*)data, 4 );
    istringstream ss( m_string, ios_base::binary | ios_base::in );

    unsigned long int m_int;
    ss.read( (char*)data2, 4 );
    m_int = *(unsigned long int*)data2;
    cout << m_int << endl;

    delete data;
    delete data2;
    return 0;

    if (argc != 2)
    {
        cout << "Usage: reader file" << endl;
        return 1;
    }

    ifstream mc_anvil_file;
    mc_anvil_file.open( argv[1], ios::binary );

    Anvil anvil;
    mc_anvil_file >> anvil;
    mc_anvil_file.close();

    cout << "Number of chunks: " << anvil.NumChunks() << endl;
    cout << "Total space used by chunks: " << anvil.ChunksTotalSize() << endl;

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

    return 0;
}
