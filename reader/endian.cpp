#include "endian.hpp"
#include <iostream>
using namespace std;

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

unsigned char ReadByte(istream& input)
{
    unsigned char r;

    input.read( (char*)&r, 1 );

    return r;
}

unsigned short int ReadShortInt(istream& input)
{
    unsigned short int r;
    unsigned char m_data[2];

    input.read( (char*)m_data, 2 );
    r = endian_swap( *(unsigned short int*)m_data );

    return r;
}

unsigned long int ReadLongInt(istream& input)
{
    unsigned long int r;
    unsigned char m_data[4];

    input.read( (char*)m_data, 4 );
    r = endian_swap( *(unsigned long int*)m_data );

    return r;
}

string ReadString(istream& input)
{
    // read string length
    unsigned short int length;
    length = ReadShortInt( input );

    // DEBUG
    cout << "Length: " << length << endl;

    char *m_char = new char[length];
    input.read( m_char, length );

    string s( m_char, length );
    // DEBUG
    cout << "String: " << s << endl;

    delete m_char;
    return s;
}
