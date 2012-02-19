#include <istream>
#include <string>

unsigned char ReadByte(std::istream& input);
unsigned short int ReadShortInt(std::istream& input);
unsigned long int ReadLongInt(std::istream& input);

std::string ReadString(std::istream& input);

unsigned short int endian_swap(unsigned short int x);
unsigned long int endian_swap(unsigned long int x);
unsigned long long endian_swap(unsigned long long x);
