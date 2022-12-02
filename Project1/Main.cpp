#include "Scenerios.h"

std::string static const CLIENT_FAILED_MSG = "The Client could not complete the scenerio due to an error.";

int main()
{
	try {
		// It is recommended to comment out the rest of the un-wanted scenerios to simulate.
		valid_send_file_flow();
		//flow_with_file_validation_recovery();
		//flow_invalid_crc();
	}
	catch (const std::exception& ex) {
		std::cout << CLIENT_FAILED_MSG << "\n" << "Full exception:" << ex.what() << std::endl;
	}
	
	return 0;
}