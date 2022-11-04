#include "Utils.h"

void Utils::random_client_id(unsigned char* buf, int size)
{
    srand(time(NULL));

    int low = 'A';
    int high = 'z';
    
    for (size_t i = 0; i < size; i++)
        buf[i] = low + std::rand() % (high - low + 1);
}