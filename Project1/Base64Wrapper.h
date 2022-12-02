#pragma once

#include <string>
#include <base64.h>

/**
 * @file Base64Wrapper.h
 * @brief A class handles base64 encoding/decoding.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

class Base64Wrapper
{
public:
	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);
};
