#include <sstream>
#include <cstdlib> // DEBUG

#include "tag.hpp"
#include "endian.hpp"
using namespace std;

// >> Tag
istream& Tag::Read( istream& input, bool skip_header )
{
    // read tag type (one byte)
    if (skip_header == false)
    {
        tag_type.b = ReadByte( input );
        name = ReadString( input );
    }

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
        tag_list_type.b = ReadByte( input );
        tag_list_size = ReadLongInt( input );

        tags.clear();
        for( int i = 0 ; i < tag_list_size ; i++ )
        {
            Tag tag;
            tag.tag_type.b = tag_list_type.b;

            // read WITHOUT header
            tag.Read( input, true );
            tags.push_back( tag );
        }

        break;

    case TAG_Compound:
    {
        Tag tag;

        // read WITH header
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
