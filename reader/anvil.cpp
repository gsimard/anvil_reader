#include "anvil.hpp"
#include <cstring>

unsigned short int endian_swap(unsigned short int x)
{
    return (x>>8) |
        (x<<8);
}

unsigned long int endian_swap(unsigned long int x)
{
    return (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

unsigned long long endian_swap(unsigned long long x)
{
    return (x>>56) |
        ((x<<40) & 0x00FF000000000000) |
        ((x<<24) & 0x0000FF0000000000) |
        ((x<<8)  & 0x000000FF00000000) |
        ((x>>8)  & 0x00000000FF000000) |
        ((x>>24) & 0x0000000000FF0000) |
        ((x>>40) & 0x000000000000FF00) |
        (x<<56);
}

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
istream & operator>>(istream& input, Chunk& obj)
{
    byte data[4];

    // read chunk length
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
        obj.data = new byte[obj.length-1];
        input.read( (char*)obj.data, obj.length-1 );
    }
    else obj.data = NULL;
    //cout << "Data length: " << obj.length << endl;

	return input;
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
