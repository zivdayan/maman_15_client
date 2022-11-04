#include "TCPClient.h"

TCPClient::TCPClient(std::string ip_addr, uint16_t port, std::string username) : ip_addr(ip_addr), port(port), username(username) {}

bool TCPClient::send_data(unsigned char* buf, unsigned int size)
{
	boost::asio::io_service ios;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip_addr), port);
	boost::asio::ip::tcp::socket socket(ios);

	socket.connect(endpoint);

	boost::array<char, 128> buffer;
	boost::system::error_code error;
	socket.write_some(boost::asio::buffer(buf, size), error);

	//size_t len = socket.read_some(boost::asio::buffer(buffer), error);
	//std::cout.write(buffer.data(), len);

	socket.close();

	return true;
}
;