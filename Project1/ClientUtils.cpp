#include "ClientUtils.h"




TCPClient ClientUtils::InitClient()
{
	TransferInfo transfer_info = ReadTransferFile();
	TCPClient tcp_client = TCPClient(transfer_info.ip_addr, transfer_info.port, transfer_info.username);
	return tcp_client;
}

unsigned char* ClientUtils::RegisterUser(TCPClient tcp_client, std::string username)
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

	unsigned char* client_id = new unsigned char[16];
	std::memcpy(client_id, response.c_str() + RESPONSE_HEADERS_BYTES_SIZE, 16);

	delete buffer;

	return client_id;
}

std::vector<std::string> ClientUtils::SendRSAPublicKey(TCPClient tcp_client, unsigned char* client_id, std::string username)
{
	
	// 1. Create an RSA decryptor. this is done here to generate a new private/public key pair
	RSAPrivateWrapper rsapriv;

	// 2. get the public key
	std::string pubkey = rsapriv.getPublicKey();	// you can get it as std::string ...
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

std::string ClientUtils::DecryptAESKey(std::string encrypted_aes_key, std::string private_key)
{
	RSAPrivateWrapper rsapriv_other(private_key);

	std::string decrypted = rsapriv_other.decrypt(encrypted_aes_key);

	return decrypted;
}

std::string ClientUtils::EncryptFileAES(std::string aes_key, std::string file_path)
{
	
	// 1. Generate a key and initialize an AESWrapper. You can also create AESWrapper with default constructor which will automatically generates a random key.
	unsigned char key[AESWrapper::DEFAULT_KEYLENGTH];
	std::memcpy(key, file_path.c_str(), AESWrapper::DEFAULT_KEYLENGTH);

	AESWrapper aes(key, AESWrapper::DEFAULT_KEYLENGTH);

	std::string plaintext = "TESTING"; // TODO: read from file

	// 2. encrypt a message (plain text)
	std::string ciphertext = aes.encrypt(plaintext.c_str(), plaintext.length());

	return ciphertext;
}

std::string ClientUtils::SendEncryptedFile(TCPClient tcp_client, std::string aes_key, unsigned char* client_id)
{
	TransferInfo transfer_info = ReadTransferFile();
	std::string file_path = transfer_info.file_path;
	std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);


	std::string encrypted_payload = ClientUtils::EncryptFileAES(aes_key, file_path);

	char* raw_payload = new char[encrypted_payload.length()];
	std::memcpy(raw_payload, encrypted_payload.c_str(), encrypted_payload.length());

	FileServerRequest request = FileServerRequest(client_id, CLIENT_VERSION, int(FileServerRequest::RequestsType::REQUEST_SEND_FILE), encrypted_payload.length(), raw_payload);
	std::tuple<const uint8_t*, const uint64_t> raw_request = request.GenerateEncryptedFileSendRequest(base_filename,Utils::get_file_size(file_path),encrypted_payload);

	auto buffer = std::get<0>(raw_request);
	auto buffer_size = std::get<1>(raw_request);

	auto response = tcp_client.send_data(buffer, buffer_size);


	return std::string();


}