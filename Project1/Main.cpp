#include <iostream>
#include "ClientUtils.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>

int main()
{
	
	std::string username = std::string("ZivDayan");
	
	auto tcp_client = ClientUtils::InitClient();	
	unsigned char* client_id = ClientUtils::RegisterUser(tcp_client, username);

	tcp_client = ClientUtils::InitClient();
	auto aes_key_to_decrypt = ClientUtils::SendRSAPublicKey(tcp_client, client_id, username);
	
	std::string private_key = aes_key_to_decrypt.front();
	std::string encrypted_aes_key = aes_key_to_decrypt.back();

	std::string decrypted_aes_key = ClientUtils::DecryptAESKey(encrypted_aes_key, private_key);
	
	delete client_id;
	return 0;
}