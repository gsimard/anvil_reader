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
    string name;

    byte tag_byte;
    unsigned short int tag_short;
    unsigned long int tag_int;
    unsigned long long tag_long;
    float tag_float;
    double tag_double;
    byte *tag_byte_array;
    string tag_string;
    unsigned long int* tag_int_array;

    // list and compound use tags for their children
    vector<Tag> tags;

    // list further uses these two
    unsigned long int tag_list_size;
    tag_t tag_list_type;

    istream& Read( istream& input );

    Tag();
    ~Tag();
};
