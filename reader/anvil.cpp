#include <cstring>
#include <sstream>
#include <zlib.h>

#include "anvil.hpp"
#include "endian.hpp"
using namespace std;

// >> Anvil
istream& Anvil::Read( istream& input )
{
    byte m_data[4];

    // read chunk location
    for ( int i = 0 ; i < 1024 ; i++ )
    {
        input.read((char*)m_data, 4);

        chunks[i].id.offset = (((unsigned long int)m_data[0])<<16) +
                              (((unsigned long int)m_data[1])<<8)  +
                               ((unsigned long int)m_data[2]);
        chunks[i].id.sector_count = m_data[3];
    }

    // read chunk timestamp
    for ( int i = 0 ; i < 1024 ; i++ )
        chunks[i].id.timestamp = ReadLongInt( input );

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        // if this chunk is not empty, then read it
        if (chunks[i].id.sector_count != 0)
        {
            // DEBUG
            //cout << "Sector count: " << (int)chunks[i].id.sector_count << endl;
            //cout << "Offset: " << ((unsigned long int)chunks[i].id.offset * 4096) << endl;

            //cout << "***********\nCHUNK START\n***********" << endl;

            // seek at the proper location in the file
            input.seekg( chunks[i].id.offset * 4096, ios::beg );
            chunks[i].Read( input );

            //cout << "*********\nCHUNK END\n*********" << endl;
        }
    }

	return input;
}

// >> Chunk
#define ZLIB_BUF_SIZE 128*1024
istream& Chunk::Read( istream& input )
{
    byte *zlib_src  = new byte[ZLIB_BUF_SIZE];
    byte *zlib_dest = new byte[ZLIB_BUF_SIZE];

    // read chunk length
    length = ReadLongInt( input );
    // DEBUG
    //cout << "Chunk length: " << length << endl;

    // read compression type
    input.read( (char*)&compression_type.b, 1 );
    // DEBUG
    //cout << "Compression type: " << compression_type.e << endl;

    // read data
    if (length > 0)
    {
        input.read( (char*)zlib_src, length-1 );

        unsigned long int destLen = ZLIB_BUF_SIZE;
        int zlib_res = uncompress( zlib_dest, &destLen, zlib_src, length-1 );

        if (zlib_res == Z_OK)
        {
            // set the data to the uncompressed data
            data = new byte[destLen];
            memcpy( data, zlib_dest, destLen );

            // change the length to the uncompressed data
            length = destLen;

            // create a stringstream from the data
            string m_string = string( (char*)data, length );
            istringstream ss( m_string, ios_base::binary | ios_base::in );

            Tag tag;
            tag.parent = NULL;
            tag.Read( ss ); // need to set parent before read because of recursion.
            tags.push_back( tag );
        }
        else
        {
            //   uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
            //   enough memory, Z_BUF_ERROR if there was not enough room in the output
            //   buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.
            data = NULL;
            cerr << "zlib: error " << zlib_res << endl;
        }
    }
    else data = NULL;
    //cout << "Data length: " << length << endl;

    delete zlib_src;
    delete zlib_dest;

	return input;
}

ChunkID::ChunkID()
{
    offset = 0;
    sector_count = 0;
    timestamp = 0;
}

// ctor
Chunk::Chunk()
{
    length = 0;
    compression_type.e = none;
    data = NULL;
}

// dtor
Chunk::~Chunk()
{
    delete data;
    data = NULL;
}

unsigned short int Anvil::NumChunks()
{
    unsigned short int r = 0;

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        if (chunks[i].id.offset != 0 && chunks[i].id.sector_count != 0)
            r++;
    }

    return r;
}

unsigned long int Anvil::ChunksTotalSize()
{
    unsigned long int r = 0;

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        // a chunk sector is 4KB
        r += chunks[i].id.sector_count * 4096;
    }

    return r;
}

unsigned short int Anvil::ChunkAt(int x, int z)
{
    return 4 * ((x % 32) + (z % 32) * 32);
}
