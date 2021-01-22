#include <iostream>

#include "Manager.h"

using namespace std;

int main()
{
	char IP[] = "127.0.0.1";
	string name = " Artem ";
	char c[1000];
	SR_Navig data;
	data.x = 100.01;
	CR_Navig a(&data);
	Addr addr_to = { 12, 14, 2 };
	Addr addr_from = { 15, 2, 7 };


	CMessage mess(R_NAVIGATION, addr_from, addr_to, NOT_REQUIRED, PRIORITY_MAX, &a);


	UDP_client client(IP, 9000);
	cout << client.open() << endl;
	int i = 0;
	while (i < 1) 
	{
		client.send(&mess);
		mess.~CMessage();
		data.x = data.x * 2;
		a = CR_Navig(&data);
		mess = CMessage(R_NAVIGATION, addr_from, addr_to, NOT_REQUIRED, PRIORITY_MAX, &a);
		i++;
	}
	cout << "end" << endl;
	return 0;
}
