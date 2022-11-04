#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "TransferInfo.h"
#include "MeInfo.h"
#include "TCPClient.h"

#include "FileServerRequest.h"

#include "Utils.h"

class ClientUtils
{
public:
	static void InitClient();
	static MeInfo ReadMeFile();
	static TransferInfo ReadTransferFile();
};



