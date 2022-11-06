#include "TCPClient.h"
#include<windows.h> 

TCPClient::TCPClient(std::string ip_addr, uint16_t port, std::string username) : ip_addr(ip_addr), port(port), username(username) {}

std::string TCPClient::send_data(const uint8_t* raw_buffer, const uint64_t size)
{
	boost::asio::io_service ios;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip_addr), port);
	boost::asio::ip::tcp::socket socket(ios);

	socket.connect(endpoint);

	boost::system::error_code error;
	socket.write_some(boost::asio::buffer(raw_buffer, size), error);
	
	

	boost::array<char, 1028> buffer;

	socket.wait(boost::asio::ip::tcp::socket::wait_read);


	size_t len = socket.read_some(boost::asio::buffer(buffer), error);
	std::string response_data(buffer.begin(), len);

	socket.close();

	std::cout << "Recieved response (size:" << std::to_string(len) << " )" + response_data << std::endl;

	return response_data;

};