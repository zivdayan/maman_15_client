#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "TransferInfo.h"
#include "MeInfo.h"
#include "TCPClient.h"

#include "FileServerRequest.h"

#include "Utils.h"

#include <iostream>
#include <string>
#include "RSAWrapper.h"
#include "AESWrapper.h"


constexpr unsigned char CLIENT_VERSION = '1';
constexpr unsigned int RESPONSE_HEADERS_BYTES_SIZE = 7;
constexpr unsigned int RESPONSE_FILE_HEADERS_TOTAL_SIZE =  RESPONSE_HEADERS_BYTES_SIZE + 275;


class ClientUtils
{
public:
	static TCPClient InitClient();
	static unsigned char* RegisterUser(TCPClient tcp_client, std::string username);
	static std::vector<std::string> SendRSAPublicKey(TCPClient tcp_client, unsigned char* client_id, std::string username);
	static std::string DecryptAESKey(std::string encrypted_aes_key, std::string private_key);
	static std::string EncryptFileAES(std::string aes_key, std::string file_data);
	static std::string SendEncryptedFile(TCPClient tcp_client, std::string aes_key, unsigned char* client_id);
	

	static MeInfo ReadMeFile();
	static TransferInfo ReadTransferFile();

};



