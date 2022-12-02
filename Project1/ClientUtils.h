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
#include "Exceptions.h"

/**
 * @file ClientUtils.h
 * @brief Central class in the project, contains all of the main functions the clients needs to interact and parse server's resposnes.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

constexpr unsigned char CLIENT_VERSION = '1';
constexpr unsigned int RESPONSE_HEADERS_BYTES_SIZE = 7;
constexpr unsigned int RESPONSE_FILE_HEADERS_TOTAL_SIZE =  RESPONSE_HEADERS_BYTES_SIZE + 275;
constexpr unsigned int NUMBER_OF_FILE_SEND_TRIES = 3;


class ClientUtils
{
public:
	static TCPClient InitClient();
	static unsigned char* RegisterUser(TCPClient tcp_client, std::string username);
	static std::vector<std::string> SendRSAPublicKey(TCPClient tcp_client, unsigned char* client_id, std::string username);
	static std::string DecryptAESKey(std::string encrypted_aes_key, std::string private_key);
	static std::string EncryptFileAES(std::string aes_key, std::string file_data);
	static bool SendEncryptedFile(TCPClient tcp_client, std::string aes_key, unsigned char* client_id);
	static void SendValidFile(TCPClient tcp_client, unsigned char* client_id);
	static void SendInvalidCRC(TCPClient tcp_client, unsigned char* client_id);
	static void SendTerminatingSessionInvalidCRC(TCPClient tcp_client, unsigned char* client_id);
	
	static MeInfo ReadMeFile();
	static TransferInfo ReadTransferFile();



};


