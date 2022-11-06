#include "Utils.h"

void Utils::random_client_id(unsigned char* buf, int size)
{
    srand(time(NULL));

    int low = 'A';
    int high = 'z';
    
    for (size_t i = 0; i < size; i++)
        buf[i] = low + std::rand() % (high - low + 1);
}
void Utils::add_null_terminating_byte(std::string str, unsigned char* buf)
{
    std::vector<char> writable(str.begin(), str.end());
    writable.push_back('\0'); //Adding null-terminated byte

    std::memcpy(buf, &writable[0], str.length() + 1);
    
}
void Utils::add_null_terminating_byte(std::string str, char* buf)
{
    std::vector<char> writable(str.begin(), str.end());
    writable.push_back('\0'); //Adding null-terminated byte

    std::memcpy(buf, &writable[0], str.length() + 1);

}
