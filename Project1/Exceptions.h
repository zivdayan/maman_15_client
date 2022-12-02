#pragma once

#include <string>
#include <iostream>
#include <exception>



class ExceptionRegistrationFailed : public std::exception {
private:
	std::string msg;
public:
	ExceptionRegistrationFailed(const std::exception e);
	virtual const char* what() const throw();
};




class ExceptionDecryptAESKey : public std::exception {
private:
	std::string msg;
public:
	ExceptionDecryptAESKey(const std::exception e);
	virtual const char* what() const throw();
};




class ExceptionSendRSAPublicKey : public std::exception {
private:
	std::string msg;
public:
	ExceptionSendRSAPublicKey(const std::exception e);
	virtual const char* what() const throw();
};




class ExceptionSendingEncryptedFile : public std::exception {
private:
	std::string msg;
public:
	ExceptionSendingEncryptedFile(const std::exception e);
	virtual const char* what() const throw();
};



class ExceptionSendingFileIndication : public std::exception {
private:
	std::string msg;
public:
	ExceptionSendingFileIndication(const std::exception e);
	virtual const char* what() const throw();
};


class ExceptionConfigFile : public std::exception {
private:
	std::string msg;
public:
	ExceptionConfigFile(const std::exception e);
	virtual const char* what() const throw();



};

