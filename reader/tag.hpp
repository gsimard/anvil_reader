#include <iostream>
#include <vector>
using namespace std;

typedef unsigned char byte;

enum tag_e
{
    TAG_End = 0,
    TAG_Byte = 1,
    TAG_Short = 2,
    TAG_Int = 3,
    TAG_Long = 4,
    TAG_Float = 5,
    TAG_Double = 6,
    TAG_Byte_Array = 7,
    TAG_String = 8,
    TAG_List = 9,
    TAG_Compound = 10,
    TAG_Int_Array = 11
};

union tag_t {
    tag_e e;
    byte b;
};

struct Tag
{
    tag_t tag_type;
    vector<Tag> tags;

    string name;

    byte tag_byte;
    unsigned short int tag_short;
    unsigned long int tag_int;
    unsigned long long tag_long;
    float tag_float;
    double tag_double;
    byte *tag_byte_array;
    string tag_string;

    // list, compound

    unsigned long int* tag_int_array;

    static string ReadName(istream& input);

    friend istream& operator>>( istream &input, Tag& obj );
    ~Tag();
};
