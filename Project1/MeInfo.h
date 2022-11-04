#pragma once

#include <string>

class MeInfo
{
private:
	friend class ClientUtils;
	std::string username;
	std::string uuid;
	std::string private_key;

public:
	MeInfo(std::string username, std::string uuid, std::string private_key);

};

#include "ClientUtils.h"