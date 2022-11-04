#include "FileServerRequest.h"

FileServerRequest::FileServerRequest(unsigned char* client_id, unsigned char version, uint16_t code, uint32_t payload_size, unsigned char* payload) : client_id(client_id), version(version), code(code), payload_size(payload_size), payload(payload) {}

unsigned char* FileServerRequest::GenerateRawRequest()
{
	struct raw_request {
		uint16_t client_id;
		unsigned char version;
		uint8_t code;
		uint16_t payload_size;
		unsigned char* payload;
	};

	return nullptr;
}

