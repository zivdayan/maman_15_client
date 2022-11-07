#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/stat.h>

class Utils
{
public:
    static void random_client_id(unsigned char* buf, int size);
    static void add_null_terminating_byte(std::string str, unsigned char* buf);
    static void add_null_terminating_byte(std::string str, char* buf);
    static long get_file_size(std::string filename);
};
