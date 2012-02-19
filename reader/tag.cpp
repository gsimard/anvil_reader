#include <sstream>
#include <cstdlib> // DEBUG

#include "tag.hpp"
#include "endian.hpp"


// >> Tag
istream& Tag::Read( istream& input )
{
    // read tag type (one byte)
    tag_type.b = Tag::ReadByte( input );
    // DEBUG
    cout << "Type: " << (unsigned long int)tag_type.b << endl;

    switch( tag_type.e )
    {
    case TAG_End:
        break;
    case TAG_Byte:
        break;
    case TAG_Short:
        break;
    case TAG_Int:
        break;
    case TAG_Long:
        break;
    case TAG_Float:
        break;
    case TAG_Double:
        break;
    case TAG_Byte_Array:
        break;
    case TAG_String:
        break;
    case TAG_List:
        // read tag type (one byte)
        tag_list_type.b = Tag::ReadByte( input );
        tag_list_size = Tag::ReadLongInt( input );

        for( int i = 0 ; i < tag_list_size ; i++ )
        {
            Tag tag;
            tag.tag_type.b = tag_list_type.b;

            // FIXME !!
            // Here we should load WITHOUT name nor type, actually, we should skip the "header"
            tag.Read( input );
            tags.push_back( tag );
        }

        break;

    case TAG_Compound:
    {
        Tag tag;
        name = Tag::ReadName( input );

        tags.clear();
        while ( tag.Read( input ), tag.tag_type.e != TAG_End)
            tags.push_back( tag );
    }
        break;

    case TAG_Int_Array:
        break;
    default:
        break;
    }

    exit(0);

	return input;
}

byte Tag::ReadByte(istream& input)
{
    byte r;

    input.read( (char*)&r, 1 );

    return r;
}

unsigned short int Tag::ReadShortInt(istream& input)
{
    unsigned short int r;
    byte m_data[2];

    input.read( (char*)m_data, 2 );
    r = endian_swap( *(unsigned short int*)m_data );

    return r;
}

unsigned long int Tag::ReadLongInt(istream& input)
{
    unsigned long int r;
    byte m_data[4];

    input.read( (char*)m_data, 4 );
    r = endian_swap( *(unsigned long int*)m_data );

    return r;
}

string Tag::ReadName(istream& input)
{
    // read string length
    unsigned short int length;
    length = Tag::ReadShortInt( input );

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

// ctor
Tag::Tag()
{
    tag_type.e = TAG_End;
    tag_byte = 0;
    tag_short = 0;
    tag_int = 0;
    tag_long = 0;
    tag_float = 0.0;
    tag_double = 0.0d;
    tag_byte_array = NULL;
    tag_int_array = NULL;
    tag_list_size = 0;
    tag_list_type.e = TAG_End;
}

// dtor
Tag::~Tag()
{
    delete tag_byte_array;
    delete tag_int_array;
}
