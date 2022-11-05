#pragma once

#include <stdint.h>
#include <tuple>

class FileServerRequest
{
private:
    friend class ClientUtils;

    unsigned char* client_id;
    unsigned char version;
    uint16_t code;
    uint32_t payload_size;
    char* payload;

public:
    FileServerRequest(unsigned char* client_id, unsigned char version, uint16_t code, uint32_t payload_size, char* payload);

    /**
        Based on action needed the functions generates the required response (and accordingly the fields needed) buffer to be sent to the client.
    */
    
    std::tuple<const uint8_t*, const uint64_t> GenerateRawRequest();
    
    enum RequestsType {      };



};

#include "ClientUtils.h"