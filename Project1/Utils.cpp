#include "Utils.h"

/**
 * @file Utils.cpp
 * @brief Contains basic useful functions used over the project.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

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


long Utils::get_file_size(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


std::string Utils::read_file_data(std::string file_path)
{

	std::ifstream infile(file_path);

	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	char* buffer = new char[length];

	infile.read(buffer, length);

	std::string final_buffer(buffer, length);

	delete buffer;
	return final_buffer;
 
}

uint32_t Utils::get_crc32(const std::string& my_string)
{
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.length());
    return result.checksum();
}