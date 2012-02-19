#include <cstring>
#include <sstream>
#include <zlib.h>

#include "anvil.hpp"
#include "endian.hpp"


// >> Anvil
istream & operator>>(istream& input, Anvil& obj)
{
    byte data[4];

    // set to zero
    for ( int i = 0 ; i < 1024 ; i++ )
        memset( &obj.chunks[i], 0, sizeof(Chunk) );

    // read chunk location
    for ( int i = 0 ; i < 1024 ; i++ )
    {
        input.read((char*)data, 4);

        obj.chunks[i].id.offset = (((unsigned long int)data[0])<<16) +
                                  (((unsigned long int)data[1])<<8)  +
                                  ((unsigned long int)data[2]);
        obj.chunks[i].id.sector_count = data[3];
    }

    // read chunk timestamp
    for ( int i = 0 ; i < 1024 ; i++ )
    {
        input.read( (char*)data, 4 );
        obj.chunks[i].id.timestamp = endian_swap( *(unsigned long int*)data );
    }

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        // if this chunk is not empty, then read it
        if (obj.chunks[i].id.sector_count != 0)
        {
            // DEBUG
            //cout << "Sector count: " << (int)obj.chunks[i].id.sector_count << endl;
            //cout << "Offset: " << ((unsigned long int)obj.chunks[i].id.offset * 4096) << endl;

            // seek at the proper location in the file
            input.seekg( obj.chunks[i].id.offset * 4096, ios::beg );
            input >> obj.chunks[i];
        }
    }

	return input;
}

// >> Chunk
#define ZLIB_BUF_SIZE 128*1024
istream & operator>>(istream& input, Chunk& obj)
{
    byte *zlib_src  = new byte[ZLIB_BUF_SIZE];
    byte *zlib_dest = new byte[ZLIB_BUF_SIZE];

    // read chunk length
    byte data[4];
    input.read( (char*)data, 4 );
    obj.length = endian_swap( *(unsigned long int*)data );
    // DEBUG
    //cout << "Chunk length: " << obj.length << endl;

    // read compression type
    input.read( (char*)&obj.compression_type.b, 1 );
    // DEBUG
    //cout << "Compression type: " << obj.compression_type.e << endl;

    // read data
    if (obj.length > 0)
    {
        input.read( (char*)zlib_src, obj.length-1 );

        unsigned long int destLen = ZLIB_BUF_SIZE;
        int zlib_res = uncompress( zlib_dest, &destLen, zlib_src, obj.length-1 );

        if (zlib_res == Z_OK)
        {
            // set the data to the uncompressed data
            obj.data = new byte[destLen];
            memcpy( obj.data, zlib_dest, destLen );

            // change the length to the uncompressed data
            obj.length = destLen;

            // create a stringstream from the data
            string m_string = string( (char*)obj.data, obj.length );
            istringstream ss( m_string, ios_base::binary | ios_base::in );

            Tag tag;
            ss >> tag;
            obj.tags.push_back( tag );
        }
        else
        {
            //   uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
            //   enough memory, Z_BUF_ERROR if there was not enough room in the output
            //   buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.
            obj.data = NULL;
            cout << "zlib: error " << zlib_res << endl;
        }
    }
    else obj.data = NULL;
    //cout << "Data length: " << obj.length << endl;

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
