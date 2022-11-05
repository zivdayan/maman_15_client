#include "FileServerRequest.h"

FileServerRequest::FileServerRequest(unsigned char* client_id, unsigned char version, uint16_t code, uint32_t payload_size, char* payload) : client_id(client_id), version(version), code(code), payload_size(payload_size), payload(payload) {}

std::tuple<const uint8_t*, const uint64_t> FileServerRequest::GenerateRawRequest()
{

#	pragma pack(push, 1)

	struct RawRequest {
		unsigned char client_id[16];
		unsigned char version;
		uint16_t code;
		uint32_t payload_size;
	};

	struct RegisterRequest
	{
		RawRequest raw_req;
		char payload[255];
	};


	struct PublicKeySendRequest
	{
		RawRequest raw_req;
		char payload[255];
	};

	struct SendingFileRequest
	{
		RawRequest raw_req;
		char payload[255];
	};

	struct ValidCRCRequest
	{
		RawRequest raw_req;
		char payload[255];
	};

	struct RegisterRequest req;

	std::memcpy(req.raw_req.client_id, client_id, 16);
	req.raw_req.version = version;
	req.raw_req.code = code;
	req.raw_req.payload_size = payload_size;
	
	std::memcpy(req.payload, payload, payload_size);

	uint8_t* raw_buffer = new uint8_t;
	const uint8_t* temp_buffer = reinterpret_cast<const uint8_t*>(&req);

	std::memcpy(raw_buffer, temp_buffer, sizeof(req));
	
	const uint8_t* final_buffer = raw_buffer;

	return std::make_tuple(final_buffer, sizeof(req));

#pragma pack(pop)
}


