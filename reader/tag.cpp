#include <sstream>
#include <cstdlib> // DEBUG

#include "tag.hpp"
#include "endian.hpp"
using namespace std;

std::string TAG_NAMES[12]
{
    "TAG_End",
        "TAG_Byte",
        "TAG_Short",
        "TAG_Int",
        "TAG_Long",
        "TAG_Float",
        "TAG_Double",
        "TAG_Byte_Array",
        "TAG_String",
        "TAG_List",
        "TAG_Compound",
        "TAG_Int_Array"
};

// >> Tag
istream& Tag::Read( istream& input, bool skip_header )
{
    // read tag type (one byte)
    if (skip_header == false)
    {
        tag_type.b = ReadByte( input );
        // DEBUG
        cout << "Type: " << TAG_NAMES[tag_type.b] << endl;

        name = ReadString( input );
    }

    switch( tag_type.e )
    {
    case TAG_End:
        break;

    case TAG_Byte:
        tag_byte = ReadByte( input );
        break;

    case TAG_Short:
        tag_short = ReadShortInt( input );
        break;

    case TAG_Int:
        tag_int = ReadLongInt( input );
        break;

    case TAG_Long:
        tag_long = ReadLongLongInt( input );
        break;

    case TAG_Float:
        tag_float = ReadFloat( input );
        break;

    case TAG_Double:
        tag_double = ReadDouble( input );
        break;

    case TAG_Byte_Array:
        tag_byte_array_size = ReadLongInt( input );
        tag_byte_array = new byte[tag_byte_array_size];

        input.read( (char*)tag_byte_array, tag_byte_array_size );

        break;

    case TAG_String:
        tag_string = ReadString( input );
        break;

    case TAG_List:
        // read tag type
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
        tag_int_array_size = ReadLongInt( input );
        tag_int_array = new unsigned long int[tag_int_array_size];

        for( int i = 0 ; i < tag_int_array_size ; i++ )
            tag_int_array[i] = ReadLongInt( input );

        break;

    default:
        break;
    }

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
    tag_byte_array_size = 0;
    tag_int_array = NULL;
    tag_int_array_size = 0;
    tag_list_size = 0;
    tag_list_type.e = TAG_End;
}

// dtor
Tag::~Tag()
{
    delete tag_byte_array;
    delete tag_int_array;
}
