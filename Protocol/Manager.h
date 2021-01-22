#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include "CMessage.h"

// Virtual excchange class
class ACExcchange_Data
{
public:
	virtual int open() = 0;
	virtual int close() = 0;
	virtual CMessage* recv() = 0;
	virtual int send(CMessage* mess) = 0;
};

// UDP client class
class UDP_client : public ACExcchange_Data
{
private:
	bool WSAS_state = FALSE;
	WSADATA ws;
	WORD ws_version = MAKEWORD(2, 2);
	SOCKET Client_s = INVALID_SOCKET;
	sockaddr_in Server_addr;
	int err = NO_ERROR;
public:
	UDP_client(char* Server_IP, int Server_port);
	~UDP_client();
	int open();
	int close();
	CMessage* recv();
	int send(CMessage* mess);
};

// UDP server class
class UDP_server : public ACExcchange_Data
{
private:
	bool WSAS_state = FALSE;
	WSADATA ws;
	WORD ws_version = MAKEWORD(2, 2);
	SOCKET Server_s = INVALID_SOCKET;
	sockaddr_in Server_addr;
	sockaddr_in Client_addr;
	int err = NO_ERROR;
public:
	UDP_server(char* Server_IP, int Server_port);
	~UDP_server();
	int open();
	int close();
	CMessage* recv();
	int send(CMessage *mess);
};