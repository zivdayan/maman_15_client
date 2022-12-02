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
#include <boost/crc.hpp>
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"


/**
 * @file Utils.h
 * @brief Contains basic useful functions used over the project.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

class Utils
{
public:
    static void random_client_id(unsigned char* buf, int size);
    static void add_null_terminating_byte(std::string str, unsigned char* buf);
    static void add_null_terminating_byte(std::string str, char* buf);
    static long get_file_size(std::string filename);
    static uint32_t get_crc32(const std::string& my_string);
    static std::string read_file_data(std::string file_path);
    

};
