#pragma once

#include <stdint.h>

class FileServerRequest
{
private:
    friend class ClientUtils;

    unsigned char* client_id;
    unsigned char version;
    uint16_t code;
    uint32_t payload_size;
    unsigned char* payload;

public:
    FileServerRequest(unsigned char* client_id, unsigned char version, uint16_t code, uint32_t payload_size, unsigned char* payload);

    /**
        Based on action needed the functions generates the required response (and accordingly the fields needed) buffer to be sent to the client.
    */
    unsigned char* GenerateRawRequest();
    
    // enum ResponseStatusCode { FILE_FOUND_AND_RESTORE = 210, ALL_FILES_LISTED = 211, FILE_DOESNT_EXIST = 1001, NO_FILES = 1002, GENERAL_ERROR = 1003, DELETE_OR_CREATE_SUCEEDED = 212 };



};

#include "ClientUtils.h"