#include "ClientUtils.h"


#include <iostream>
#include <string>


void ClientUtils::InitClient()
{
	MeInfo me_info = ReadMeFile();
	TransferInfo transfer_info = ReadTransferFile();
	
	TCPClient tcp_client = TCPClient(transfer_info.ip_addr, transfer_info.port, transfer_info.username);
	unsigned char* client_id = new unsigned char[16];
	Utils::random_client_id(client_id, 16);

	std::cout << client_id << std::endl;
	
	std::string str = std::string("ZivDayan");
	std::vector<char> writable(str.begin(), str.end());
	writable.push_back('\0'); //Adding null-terminated byte

	char* c = &writable[0];

	FileServerRequest request = FileServerRequest(client_id, '1', 1100, str.length() + 1, c);
	std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateRawRequest();

	auto buffer= std::get<0>(raw_request);
	auto buffer_size= std::get<1>(raw_request);

	tcp_client.send_data(buffer, buffer_size);
	delete buffer;
}

MeInfo ClientUtils::ReadMeFile()
{
	std::ifstream file("me.info");	
	std::string str;
	std::string username, uuid, private_key;

	int line = 1;
	while (std::getline(file, str)) {
		switch (line)
		{
		case 1:
			username = str;
			break;
		case 2:
			uuid = str;
			break;
		case 3:
			private_key = str;
			break;
		};
		line++;
	}
	return MeInfo(username, uuid, private_key);
}

TransferInfo ClientUtils::ReadTransferFile()
{
	std::ifstream file("transfer.info");
	std::string str;


	int line = 1;

	std::string ip_addr;
	uint16_t port; 
	std::string username;
	std::string file_path;

	while (std::getline(file, str)) {
		switch (line)
		{
		case 1:
			
			ip_addr = str.substr(0, str.find(':'));
			port = std::atoi(str.substr(str.find(':') + 1 , str.length()).c_str());
			break;
		case 2:
			username = str;
			break;
		case 3:
			file_path = str;
			break;
		}
		line++;
	}
	return TransferInfo(ip_addr, port, username, file_path);
}
