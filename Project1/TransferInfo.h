#pragma once

#include <string>



class TransferInfo
{
private:
	friend class ClientUtils;
	std::string ip_addr;
	std::string file_path;
	std::string username;
	uint16_t port;

public:
	TransferInfo(std::string ip_addr, uint16_t port, std::string username, std::string file_path);
	
};

#include "ClientUtils.h"