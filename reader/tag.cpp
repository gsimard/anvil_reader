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
        cout << endl << "Type: " << TAG_NAMES[tag_type.b] << ": " << (unsigned long int)tag_type.b << endl;

        // if this is a TAG_End, there is no string, so don't try reading one
        if (tag_type.e != TAG_End)
            name = ReadString( input );
        else
            name = string("");
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

        cout << "Array length: " << tag_byte_array_size << endl;

        break;

    case TAG_String:
        tag_string = ReadString( input );
        break;

    case TAG_List:
        //FIXME
        cout << "about to read list" << endl;

        // read tag type
        tag_list_type.b = ReadByte( input );
        tag_list_size = ReadLongInt( input );

        cout << "List length: " << tag_list_size << endl;

        tags.clear();
        for( int i = 0 ; i < tag_list_size ; i++ )
        {
            Tag tag;
            tag.tag_type.b = tag_list_type.b;

            cout << endl << "List item " << i << endl;

            // read WITHOUT header if not a compound type
            if ( tag_list_type.e != TAG_Compound )
                tag.Read( input, true );
            else
                tag.Read( input, false );

            tags.push_back( tag );
        }

        break;

    case TAG_Compound:
        tags.clear();

        do
        {
            Tag tag;
            // read WITH header
            tag.Read( input );

            if (tag.tag_type.e != TAG_End)
                tags.push_back( tag );
            else
                break;
        } while( true );

        break;

    case TAG_Int_Array:
        tag_int_array_size = ReadLongInt( input );
        tag_int_array = new unsigned long int[tag_int_array_size];

        for( int i = 0 ; i < tag_int_array_size ; i++ )
            tag_int_array[i] = ReadLongInt( input );

        cout << "Array length: " << tag_int_array_size << endl;

        break;

    default:
        cout << "Fatal error: wrong tag type." << endl;
        exit(0);
        break;
    }

	return input;
}

// copy
Tag::Tag( const Tag& src )
{
    tag_type.e = src.tag_type.e;
    tag_byte = src.tag_byte;
    tag_short = src.tag_short;
    tag_int = src.tag_int;
    tag_long = src.tag_long;
    tag_float = src.tag_float;
    tag_double = src.tag_double;
    tag_list_size = src.tag_list_size;
    tag_list_type.e = src.tag_list_type.e;

    tag_byte_array_size = src.tag_byte_array_size;
    tag_byte_array = new byte[tag_byte_array_size];
    std::copy( src.tag_byte_array, src.tag_byte_array + src.tag_byte_array_size, tag_byte_array );

    tag_int_array_size = src.tag_int_array_size;
    tag_int_array = new unsigned long int[tag_int_array_size];
    std::copy( src.tag_int_array, src.tag_int_array + src.tag_int_array_size, tag_int_array );
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
    tag_byte_array = NULL;
    delete tag_int_array;
    tag_int_array = NULL;
}
