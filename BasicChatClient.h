#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <thread>
#include <future>
#include <mutex>
#include "Packet.h"

#pragma comment(lib, "Ws2_32.lib")

/*
Sys call order

1.getaddrinfo()
2.socket()
3.bind()
4.listen()
*/

#define PORT 13900
#define PORTSTR "13900"
#define BACKLOG 50
#define IPADDR "161.35.232.244"
#define MAXMSGLEN 256


class BasicChatClient
{

public:
	BasicChatClient();
	~BasicChatClient();

	void Initialize();
	void RecvMsg();
	void SendMsg(std::string& msg);
	void PrintMsg(std::string msg);
	void SendPacket(int serverSockfd, Packet* p);
	void SendString( std::string msg);
	void SendCommand(int serverfd, Packet::COMMANDTYPE cmdtype, std::string arg);
	void Run();
	void GetInput();
	void ParseUserInput(std::string line);
	void AddMessageToLog(std::string* msg);
	std::string GetCurTime();
	std::vector<std::string> message_log;


private:
#ifdef _WIN32
	WSADATA wsaData;
#endif
	int sockfd;
	bool serverConnected;
	struct sockaddr_in serverInfo;
	static std::mutex cout_mutex;
	static std::mutex message_log_mutex;


};

