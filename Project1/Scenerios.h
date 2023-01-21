#pragma once
#include <iostream>
#include "ClientUtils.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>


void valid_send_file_flow(std::string username);

void flow_with_file_validation_recovery(std::string username);

void flow_invalid_crc(std::string username);
