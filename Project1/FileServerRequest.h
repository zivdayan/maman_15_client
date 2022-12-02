#pragma once

#include <stdint.h>
#include <tuple>
#include <string>


/**
 * @file FileServerRequest.h
 * @brief contains the base functions to generate and parse the protocol's structs.
 * @author Ziv Dayan

 The Open University - Defensive Systems Programming  || Maman 15

 */

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



    std::tuple<const uint8_t*, const uint64_t> GenerateRegisterRequest();
    std::tuple<const uint8_t*, const uint64_t> GenerateSendingPKRequest(std::string username);
    std::tuple<const uint8_t*, const uint64_t> GenerateEncryptedFileSendRequest(std::string file_name, size_t file_size, std::string encrypted_file_content);

    std::tuple<const uint8_t*, const uint64_t> GenerateValidationFileIndication(std::string file_name);


    /**
        Based on action needed the functions generates the required response (and accordingly the fields needed) buffer to be sent to the client.
    */
    enum RequestsType {
        REQUEST_REGISTER_CODE = 1100,
        REQUEST_SEND_PK = 1101,
        REQUEST_SEND_FILE = 1103,
        REQUEST_VALID_CRC = 1104,
        REQUEST_INVALID_CRC = 1105,
        REQUEST_INVALID_CRC_TERMINATING = 1106
    };


};

#include "ClientUtils.h"