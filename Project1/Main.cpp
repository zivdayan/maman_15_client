#include "Scenerios.h"
#include "argparse.cpp"

std::string static const CLIENT_FAILED_MSG = "The Client could not complete the scenerio due to an error.";
std::string static const DEFAULT_USERNAME = "username";

int main(int argc, char* argv[])
{
	argparse::ArgumentParser program("tcp_encrypted_file_manager");
	std::string username = DEFAULT_USERNAME;
	try {

		program.add_argument("--basic-demo")
			.default_value(false)
			.implicit_value(true);

		program.add_argument("--fail-and-recover-demo")
			.default_value(false)
			.implicit_value(true);

		program.add_argument("--invalid-and-failure-demo")
			.default_value(false)
			.implicit_value(true);

		program.add_argument("-u", "--username")
			.default_value(DEFAULT_USERNAME);

		program.add_description("A client used to communicate with an Encrypted TCP File Server - to securly transfer files over a network");

		program.parse_args(argc, argv);

		if (program.is_used("-u") || program.is_used("--username"))
			username = program.get<std::string>("-u");

		bool picked_scenerio = false;

		std::cout << "Starting sending flow..." << std::endl;

		if (program.is_used("--basic-demo"))
		{
			picked_scenerio = true;
			valid_send_file_flow(username);
		}


		if (program.is_used("--fail-and-recover-demo"))
		{
			picked_scenerio = true;
			flow_with_file_validation_recovery(username);
		}
			

		if (program.is_used("--invalid-and-failure-demo"))
		{
			picked_scenerio = true;
			flow_invalid_crc(username);
		}
			

		if (!picked_scenerio)
		{
			std::cout << "No specific demo scenerio was found - starting basic file send" << "\n" << std::endl;
			valid_send_file_flow(username);
		}

	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		std::exit(1);
	}
	catch (const std::exception& ex) {
		std::cout << CLIENT_FAILED_MSG << "\n" << "Full exception:" << ex.what() << std::endl;
	}
	
	return 0;
}