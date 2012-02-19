#include <sstream>
#include <cstdlib> // DEBUG

#include "tag.hpp"
#include "endian.hpp"


// >> Tag
istream & operator>>(istream& input, Tag& obj)
{
    byte m_data[4];
    Tag tag;

    // read tag type (one byte)
    input.read( (char*)&obj.tag_type.b, 1 );
    // DEBUG
    cout << obj.tag_type.e << endl;

    switch( obj.tag_type.e )
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
        break;
    case TAG_Compound:
        obj.name = Tag::ReadName( input );

        obj.tags.clear();
        while (input >> tag, tag.tag_type.e != TAG_End)
            obj.tags.push_back( tag );

        break;

    case TAG_Int_Array:
        break;
    default:
        break;
    }

    exit(0);

	return input;
}

string Tag::ReadName(istream& input)
{
    // read string length
    byte m_data[2];
    unsigned short int length;

    input.read( (char*)m_data, 2 );
    length = endian_swap( *(unsigned short int*)m_data );

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

// dtor
Tag::~Tag()
{
    delete tag_byte_array;
    delete tag_int_array;
}
