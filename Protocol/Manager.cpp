#include "Manager.h"

 // UDP_server function
UDP_server::UDP_server(char* Server_IP, int Server_port)
{
	Server_addr.sin_family = AF_INET;
	Server_addr.sin_port = htons(Server_port);
	Server_addr.sin_addr.s_addr = inet_addr(Server_IP);
	Client_addr.sin_addr.s_addr = 0;
	if (WSAStartup(ws_version, &ws) == NO_ERROR)
	{
		WSAS_state = TRUE;
	}
	else
	{
		err = WSAGetLastError();
	}
};

UDP_server::~UDP_server()
{
	if (WSAS_state != FALSE)
	{
		if (Server_s != INVALID_SOCKET)
		{
			closesocket(Server_s);
		}
		WSACleanup();
	}
};

int UDP_server::open()
{
	int reply = ERROR;
	if (Server_s == INVALID_SOCKET)
	{
		Server_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (Server_s != INVALID_SOCKET)
		{
			if (bind(Server_s, (const sockaddr*)&Server_addr, sizeof(Server_addr)) != SOCKET_ERROR)
			{
				reply = NO_ERROR;
			}
		}
	}
	return reply;
};

int UDP_server::close()
{
	int reply = closesocket(Server_s);
	if (reply == NO_ERROR)
	{
		Server_s = INVALID_SOCKET;
	}
	return reply;
};

CMessage* UDP_server::recv()
{
	sockaddr_in IN_addr;
	int in_size = sizeof(IN_addr);
	int client_size = sizeof(Client_addr);
	CMessage* reply = nullptr;
	unsigned long recv_buff_size = 0;
	if (Server_s != INVALID_SOCKET)
	{
		ioctlsocket(Server_s, FIONREAD, &recv_buff_size);
		if (recv_buff_size > 0)
		{
			char* buff = new char[recv_buff_size];
			if (Client_addr.sin_addr.s_addr == 0)
			{
				if (recvfrom(Server_s, buff, recv_buff_size, 0, (struct sockaddr*) & Client_addr, &client_size) != SOCKET_ERROR)
				{
					reply = &CMessage(buff, recv_buff_size);
				}
			}
			else
			{
				if (recvfrom(Server_s, buff, recv_buff_size, 0, (struct sockaddr*) & IN_addr, &in_size) != SOCKET_ERROR)
				{
					if (Client_addr.sin_addr.s_addr == IN_addr.sin_addr.s_addr)
					{
						reply = &CMessage(buff, recv_buff_size);
					}
				}
			}

		}
	}
	return reply;
};	


int UDP_server::send(CMessage* mess)
{
	int reply = ERROR;
	if (mess != nullptr)
	{
		if (mess->get_mess() != nullptr && Client_addr.sin_addr.s_addr != 0)
		{
			reply = sendto(Server_s, mess->get_mess(), MESSAGE_LEN, 0, (SOCKADDR*)&Client_addr, sizeof(Client_addr));
		}
	}
	return reply;
};

// UDP_client function
UDP_client::UDP_client(char* Server_IP, int Server_port)
{
	Server_addr.sin_family = AF_INET;
	Server_addr.sin_port = htons(Server_port);
	Server_addr.sin_addr.s_addr = inet_addr(Server_IP);
	if (WSAStartup(ws_version, &ws) == NO_ERROR)
	{
		WSAS_state = TRUE;
	}
	else
	{
		err = WSAGetLastError();
	}
};

UDP_client::~UDP_client()
{
	if (WSAS_state != FALSE)
	{
		if (Client_s != INVALID_SOCKET)
		{
			closesocket(Client_s);
		}
		WSACleanup();
	}
};

int UDP_client::open()
{
	int reply = ERROR;
	if (Client_s == INVALID_SOCKET)
	{
		Client_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (Client_s != INVALID_SOCKET)
		{
			reply = NO_ERROR;
		}
	}
	return reply;
};

int UDP_client::close()
{
	int reply = closesocket(Client_s);
	if (reply == NO_ERROR)
	{
		Client_s = INVALID_SOCKET;
	}
	return reply;
};

CMessage* UDP_client::recv()
{
	sockaddr_in IN_addr;
	int in_size = sizeof(IN_addr);
	CMessage* reply = nullptr;
	unsigned long recv_buff_size = 0;
	if (Client_s != INVALID_SOCKET)
	{
		ioctlsocket(Client_s, FIONREAD, &recv_buff_size);
		if (recv_buff_size > 0)
		{
			char* buff = new char[recv_buff_size];
			if (recvfrom(Client_s, buff, recv_buff_size, 0, (struct sockaddr*) & IN_addr, &in_size) == recv_buff_size)
			{
				if (Server_addr.sin_addr.s_addr == IN_addr.sin_addr.s_addr)
				{
					reply = &CMessage(buff, recv_buff_size);
				}
			}
		}
	}
	return reply;
};

int UDP_client::send(CMessage* mess)
{
	int reply = ERROR;
	if (mess != nullptr)
	{
		if (mess->get_mess() != nullptr)
		{
			reply = sendto(Client_s, mess->get_mess(), MESSAGE_LEN, 0, (SOCKADDR*)&Server_addr, sizeof(Server_addr));
		}
	}
	return reply;
};