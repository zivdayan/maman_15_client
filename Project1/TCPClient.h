#pragma once
#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>

#include <chrono>
#include <thread>


/**
 * @file TCPClient.h
 * @brief Basic class enables to have raw socket communication between the client and the server.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

class TCPClient
{
private:


	std::string ip_addr;
	std::string username;
	uint16_t port;

public:
	TCPClient(std::string ip_addr, uint16_t port, std::string username);
	std::string send_data(const uint8_t* buf, const uint64_t size);


};

#include "ClientUtils.h"