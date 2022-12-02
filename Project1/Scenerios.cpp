#include "Scenerios.h"

void valid_send_file_flow()
{
	std::string username = std::string("NewUser");

	auto tcp_client = ClientUtils::InitClient();
	unsigned char* client_id = ClientUtils::RegisterUser(tcp_client, username);

	std::cout << "Registered succesfully!" << std::endl;

	tcp_client = ClientUtils::InitClient();
	auto aes_key_to_decrypt = ClientUtils::SendRSAPublicKey(tcp_client, client_id, username);

	std::string private_key = aes_key_to_decrypt.front();
	std::string encrypted_aes_key = aes_key_to_decrypt.back();

	std::string decrypted_aes_key = ClientUtils::DecryptAESKey(encrypted_aes_key, private_key);

	tcp_client = ClientUtils::InitClient();

	bool validated_file = false;
	for (size_t i = 0; i < NUMBER_OF_FILE_SEND_TRIES; i++)
	{
		validated_file = ClientUtils::SendEncryptedFile(tcp_client, decrypted_aes_key, client_id);
		if (validated_file)
		{
			std::cout << "File recieved succesfully!" << std::endl;
			ClientUtils::SendValidFile(tcp_client, client_id);
			break;
		}

		else
			ClientUtils::SendInvalidCRC(tcp_client, client_id);
	}

	if (!validated_file)
		ClientUtils::SendTerminatingSessionInvalidCRC(tcp_client, client_id);



	delete client_id;
}

void flow_with_file_validation_recovery()
{
	std::string username = std::string("Ziv_Dayan");

	auto tcp_client = ClientUtils::InitClient();
	unsigned char* client_id = ClientUtils::RegisterUser(tcp_client, username);

	tcp_client = ClientUtils::InitClient();
	auto aes_key_to_decrypt = ClientUtils::SendRSAPublicKey(tcp_client, client_id, username);

	std::string private_key = aes_key_to_decrypt.front();
	std::string encrypted_aes_key = aes_key_to_decrypt.back();

	std::string decrypted_aes_key = ClientUtils::DecryptAESKey(encrypted_aes_key, private_key);

	std::string dummy_decrypted_aes_key = ClientUtils::DecryptAESKey(encrypted_aes_key, private_key); //Creating fake key to create invalid file stream for one iteration
	dummy_decrypted_aes_key[dummy_decrypted_aes_key.size() - 1] = 'X';

	tcp_client = ClientUtils::InitClient();

	bool validated_file = false;
	for (size_t i = 0; i < NUMBER_OF_FILE_SEND_TRIES; i++)
	{
		if (i == 0)
			validated_file = ClientUtils::SendEncryptedFile(tcp_client, dummy_decrypted_aes_key, client_id);
		else
			validated_file = ClientUtils::SendEncryptedFile(tcp_client, decrypted_aes_key, client_id);


		if (validated_file)
		{
			ClientUtils::SendValidFile(tcp_client, client_id);
			break;
		}

		else
			ClientUtils::SendInvalidCRC(tcp_client, client_id);
	}

	if (!validated_file)
		ClientUtils::SendTerminatingSessionInvalidCRC(tcp_client, client_id);



	delete client_id;
}


void flow_invalid_crc()
{
	std::string username = std::string("Ziv_Dayan");

	auto tcp_client = ClientUtils::InitClient();
	unsigned char* client_id = ClientUtils::RegisterUser(tcp_client, username);

	tcp_client = ClientUtils::InitClient();
	auto aes_key_to_decrypt = ClientUtils::SendRSAPublicKey(tcp_client, client_id, username);

	std::string private_key = aes_key_to_decrypt.front();
	std::string encrypted_aes_key = aes_key_to_decrypt.back();

	std::string decrypted_aes_key = ClientUtils::DecryptAESKey(encrypted_aes_key, private_key);
	decrypted_aes_key[decrypted_aes_key.size() - 1] = 'X'; // Corrupting the aes key to produce invalid crc.

	tcp_client = ClientUtils::InitClient();

	bool validated_file = false;
	for (size_t i = 0; i < NUMBER_OF_FILE_SEND_TRIES; i++)
	{
		validated_file = ClientUtils::SendEncryptedFile(tcp_client, decrypted_aes_key, client_id);
		if (validated_file)
		{
			ClientUtils::SendValidFile(tcp_client, client_id);
			break;
		}

		else
			ClientUtils::SendInvalidCRC(tcp_client, client_id);
	}

	if (!validated_file)
		ClientUtils::SendTerminatingSessionInvalidCRC(tcp_client, client_id);



	delete client_id;
}
