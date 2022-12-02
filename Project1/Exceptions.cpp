#include "Exceptions.h"



ExceptionRegistrationFailed::ExceptionRegistrationFailed(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "The registration process failed. Check your input and try again. \n";
	formatted_msg += e.what();
	msg = formatted_msg.c_str();
	
}

const char* ExceptionRegistrationFailed::what() const throw() {
	std::cout << msg;
	return msg.c_str();
}



ExceptionDecryptAESKey::ExceptionDecryptAESKey(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "An error occured during the decryption of the AES key. \n Might be a data corruption of the server. \n Please try again.";
	formatted_msg += e.what();

	msg = formatted_msg.c_str();
}

const char* ExceptionDecryptAESKey::what() const throw() {
	return msg.c_str();
}

ExceptionSendingEncryptedFile::ExceptionSendingEncryptedFile(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Failed to send the response to the server. Please try again. \n";
	formatted_msg += e.what();

	msg = formatted_msg.c_str();
}

const char* ExceptionSendingEncryptedFile::what() const throw() {
	return msg.c_str();
}



ExceptionSendingFileIndication::ExceptionSendingFileIndication(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Failed to send an encrypted file. Make sure the configured file exists and connection is valid. \n";
	formatted_msg += e.what();

	msg = formatted_msg.c_str();
}

const char* ExceptionSendingFileIndication::what() const throw() {
	return msg.c_str();
}


ExceptionConfigFile::ExceptionConfigFile(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Could not open the configuration file.";
	formatted_msg += e.what();

	msg = formatted_msg.c_str();
}

const char* ExceptionConfigFile::what() const throw() {
	return msg.c_str();
}

ExceptionSendRSAPublicKey::ExceptionSendRSAPublicKey(const std::exception e) {
	std::string formatted_msg = std::string();
	formatted_msg = "Could not send PK to the server. Check your input and try again \n";
	formatted_msg += e.what();

	msg = formatted_msg.c_str();
}

const char* ExceptionSendRSAPublicKey::what() const throw() {
	return msg.c_str();
}
