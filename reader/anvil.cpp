#include "anvil.hpp"

istream & operator>>(istream& input, Anvil& obj)
{
    byte data[4];

    // read chunk location
    for ( int i = 0 ; i < 1024 ; i++ )
    {
        input.read((char*)data, 4);
        obj.chunks[i].id.offset = ((unsigned long int)data[0])<<16 + ((unsigned long int)data[1])<<8 + ((unsigned long int)data[2]);
        cout << obj.chunks[i].id.offset << endl;
        obj.chunks[i].id.sector_count = data[3];
    }

    // read chunk timestamp
    for ( int i = 0 ; i < 1024 ; i++ )
    {
        input.read((char*)data, 4);
        obj.chunks[i].id.timestamp = *(unsigned long int*)data;
    }

	return input;
}

unsigned short int Anvil::NumChunks()
{
    unsigned short int r = 0;

    for ( int i = 0 ; i < 1024 ; i++ )
    {
        if (chunks[i].id.offset == 0 && chunks[i].id.sector_count == 0)
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
