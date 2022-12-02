#include "ClientUtils.h"


/**
 * @file ClientUtils.cpp
 * @brief Central class in the project, contains all of the main functions the clients needs to interact and parse server's resposnes.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

TCPClient ClientUtils::InitClient()
{
	TransferInfo transfer_info = ReadTransferFile();
	TCPClient tcp_client = TCPClient(transfer_info.ip_addr, transfer_info.port, transfer_info.username);
	return tcp_client;
}

unsigned char* ClientUtils::RegisterUser(TCPClient tcp_client, std::string username)
{
	try
	{
		unsigned char* fake_client_id = new unsigned char[16];
		Utils::random_client_id(fake_client_id, 16);

		std::string str = username;
		char* payload = new char[str.length() + 1];

		Utils::add_null_terminating_byte(str, payload);

		FileServerRequest request = FileServerRequest(fake_client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_REGISTER_CODE), str.length() + 1, payload);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateRegisterRequest();

		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		auto response = tcp_client.send_data(buffer, buffer_size);

		if (response.length() == RESPONSE_HEADERS_BYTES_SIZE)
			throw std::exception("User is already registered! (Code 2101)");

		unsigned char* client_id = new unsigned char[16];
		std::memcpy(client_id, response.c_str() + RESPONSE_HEADERS_BYTES_SIZE, 16);

		delete buffer;
		delete payload;

		return client_id;
	}
	catch (const std::exception& ex)
	{
		throw ExceptionRegistrationFailed(ex);
	}
}

std::vector<std::string> ClientUtils::SendRSAPublicKey(TCPClient tcp_client, unsigned char* client_id, std::string username)
{
	
	try
	{
		
		RSAPrivateWrapper rsapriv;

		
		std::string pubkey = rsapriv.getPublicKey();
		std::string skey = rsapriv.getPrivateKey();


		char* raw_pubkey = new char[pubkey.length()];
		std::memcpy(raw_pubkey, pubkey.c_str(), pubkey.length());


		FileServerRequest request = FileServerRequest(client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_SEND_PK), pubkey.length(), raw_pubkey);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateSendingPKRequest(username);

		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		auto response = tcp_client.send_data(buffer, buffer_size);

		int client_int_start = RESPONSE_HEADERS_BYTES_SIZE;
		int client_int_end = RESPONSE_HEADERS_BYTES_SIZE + 16;
		int aes_key_start = RESPONSE_HEADERS_BYTES_SIZE + 16;
		int aes_key_end = response.length() - aes_key_start;


		auto _client_id = response.substr(client_int_start, client_int_end - client_int_start);
		auto encrypted_aes_key = response.substr(aes_key_start, aes_key_end);

		std::vector<std::string> packed_encryped_and_sk = { skey, encrypted_aes_key };

		delete buffer;
		return packed_encryped_and_sk;
	}
	catch (const std::exception& ex)
	{
		throw ExceptionSendRSAPublicKey(ex);
	}
}

MeInfo ClientUtils::ReadMeFile()
{
	try
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
	catch (const std::exception& ex)
	{
		throw ExceptionConfigFile(ex);
	}

}

TransferInfo ClientUtils::ReadTransferFile()
{
	try
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
				port = std::atoi(str.substr(str.find(':') + 1, str.length()).c_str());
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
	catch (const std::exception& ex)
	{
		throw ExceptionConfigFile(ex);
	}
}

std::string ClientUtils::DecryptAESKey(std::string encrypted_aes_key, std::string private_key)
{
	try {
		RSAPrivateWrapper rsapriv_other(private_key);

		std::string decrypted = rsapriv_other.decrypt(encrypted_aes_key);

		return decrypted;
	}
	catch (const std::exception& ex)
	{
		throw ExceptionDecryptAESKey(ex);
	}
}

#include <iostream>
#include <iomanip>
void hexify(const unsigned char* buffer, unsigned int length)
{
	std::ios::fmtflags f(std::cout.flags());
	std::cout << std::hex;
	for (size_t i = 0; i < length; i++)
		std::cout << std::setfill('0') << std::setw(2) << (0xFF & buffer[i]) << (((i + 1) % 16 == 0) ? "\n" : " ");
	std::cout << std::endl;
	std::cout.flags(f);
}

std::string ClientUtils::EncryptFileAES(std::string aes_key, std::string file_data)
{
	try {
		// 1. Generate a key and initialize an AESWrapper. You can also create AESWrapper with default constructor which will automatically generates a random key.
		unsigned char* key = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
		std::memcpy(key, aes_key.c_str(), AESWrapper::DEFAULT_KEYLENGTH);

		hexify(key, AESWrapper::DEFAULT_KEYLENGTH);

		AESWrapper aes(key, AESWrapper::DEFAULT_KEYLENGTH);


		// 2. encrypt a message (plain text)
		std::string ciphertext = aes.encrypt(file_data.c_str(), file_data.length());

		return ciphertext;
	}
	catch (std::exception& e)
	{
		throw ExceptionSendingEncryptedFile(e);
	}
}



bool ClientUtils::SendEncryptedFile(TCPClient tcp_client, std::string aes_key, unsigned char* client_id)
{
	try {
		TransferInfo transfer_info = ReadTransferFile();
		std::string file_path = transfer_info.file_path;
		std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);


		std::string file_data = Utils::read_file_data(file_path);

		std::string encrypted_payload = ClientUtils::EncryptFileAES(aes_key, file_data);

		char* raw_payload = new char[encrypted_payload.length()];
		std::memcpy(raw_payload, encrypted_payload.c_str(), encrypted_payload.length());

		FileServerRequest request = FileServerRequest(client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_SEND_FILE), encrypted_payload.length(), raw_payload);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateEncryptedFileSendRequest(base_filename, Utils::get_file_size(file_path), encrypted_payload);

		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		uint32_t file_crc = Utils::get_crc32(file_data);

		auto response = tcp_client.send_data(buffer, buffer_size);


		int payload_start_index = RESPONSE_FILE_HEADERS_TOTAL_SIZE;
		int payload_length = response.length() - payload_start_index;

		auto crc_pointer = response.c_str() + payload_start_index;

		uint32_t data;
		std::memcpy((char*)&data, crc_pointer, sizeof(data));

		delete raw_payload;
		return file_crc == data;
	}
	

	catch (std::exception& e)
	{
		throw ExceptionSendingEncryptedFile(e);
	}

	

}

void ClientUtils::SendValidFile(TCPClient tcp_client, unsigned char* client_id)
{

	try
	{

		TransferInfo transfer_info = ReadTransferFile();
		std::string file_path = transfer_info.file_path;
		std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		FileServerRequest request = FileServerRequest(client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_VALID_CRC), base_filename.length(), nullptr);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateValidationFileIndication(base_filename);
		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		tcp_client.send_data(buffer, buffer_size);
	}


	catch (std::exception& e)
	{
		throw ExceptionSendingFileIndication(e);
	}

}


void ClientUtils::SendInvalidCRC(TCPClient tcp_client, unsigned char* client_id)
{
	try {
		TransferInfo transfer_info = ReadTransferFile();
		std::string file_path = transfer_info.file_path;
		std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		FileServerRequest request = FileServerRequest(client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_INVALID_CRC), base_filename.length(), nullptr);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateValidationFileIndication(base_filename);
		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		tcp_client.send_data(buffer, buffer_size);

	}
	
	catch (std::exception& e)
	{
		throw ExceptionSendingFileIndication(e);
	}
	
}
void ClientUtils::SendTerminatingSessionInvalidCRC(TCPClient tcp_client, unsigned char* client_id)
{
	try {
		TransferInfo transfer_info = ReadTransferFile();
		std::string file_path = transfer_info.file_path;
		std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		FileServerRequest request = FileServerRequest(client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_INVALID_CRC_TERMINATING), base_filename.length(), nullptr);
		std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateValidationFileIndication(base_filename);
		auto buffer = std::get<0>(raw_request);
		auto buffer_size = std::get<1>(raw_request);

		tcp_client.send_data(buffer, buffer_size);
	}

	
	catch (std::exception& e)
	{
		throw ExceptionSendingFileIndication(e);
	}
}