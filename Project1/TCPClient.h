#pragma once
#include <iostream>
#include "ClientUtils.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>

class TCPClient
{
private:


	std::string ip_addr;
	std::string username;
	uint16_t port;

public:
	TCPClient(std::string ip_addr, uint16_t port, std::string username);
	bool send_data(unsigned char* buf, unsigned int size);
	bool recieve_data();


};

