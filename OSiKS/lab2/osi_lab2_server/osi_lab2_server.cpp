#pragma comment (lib,"Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <iostream> 
#include <sstream>
#include <string>

using namespace std;
int main(void)
{
	WORD sockVer;
	WSADATA wsaData;
	int retVal;
	sockVer = MAKEWORD(2, 2);
	WSAStartup(sockVer, &wsaData);
	//Создаем сокет
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (servSock == INVALID_SOCKET)
	{
		printf("Unable to create socket\n");
		WSACleanup();
		system("pause");
		return SOCKET_ERROR;
	}

	SOCKADDR_IN sin;
	sin.sin_family = PF_INET;
	sin.sin_port = htons(2004);
	sin.sin_addr.s_addr = INADDR_ANY;

	retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
	if (retVal == SOCKET_ERROR)
	{
		printf("Unable to bind\n");
		WSACleanup();
		system("pause");
		return SOCKET_ERROR;
	}
	printf("Server started at %s, port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
	while (true)
	{
		//Пытаемся начать слушать сокет
		retVal = listen(servSock, 10);
		if (retVal == SOCKET_ERROR)
		{
			printf("Unable to listen\n");
			WSACleanup();
			system("pause");
			return SOCKET_ERROR;
		}
		//Ждем клиента
		SOCKET clientSock;
		SOCKADDR_IN from;
		int fromlen = sizeof(from);
		clientSock = accept(servSock, (struct sockaddr*)&from, &fromlen);
		if (clientSock == INVALID_SOCKET)
		{
			printf("Unable to accept\n");
			WSACleanup();
			system("pause");
			return SOCKET_ERROR;
		}
		printf("New connection accepted from %s, port %d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
		char szReq[256];
		retVal = recv(clientSock, szReq, 256, 0);
		//Пытаемся получить данные от клиента
		if (retVal == SOCKET_ERROR)
		{
			printf("Unable to recv\n");
			system("pause");
			return SOCKET_ERROR;
		}
		szReq[retVal] = '\0';
		printf("Data received\n");
		string s = (const char*)szReq;
		std::cout << s;
		if (!s.compare(0, 4, "stop"))// Команда на выключение сервера
		{
			printf("Received server shutdown request\n");
			s = "Server shutdown";
			retVal = send(clientSock, s.c_str(), 256, 0);

			closesocket(clientSock);
			break;
		}
		else
		{
			//Обрабатываем данные
			char szResp[256];
			string sResp = "";
			string sFromText = "";
			for (int i = 0; szReq[i] != '\0'; i++)
			{
				if (szReq[i] != '\n')
				{
					sFromText += szReq[i];
				}
				else
				{
					sFromText += to_string(sFromText.length()) + "\n";
					sResp += sFromText;
					sFromText = "";
				}
			}
			sFromText += to_string(sFromText.length()) + "\n";
			sResp += sFromText;
			if (sResp.empty())
			{
				sFromText += to_string(sFromText.length()) + "\n";
				sResp += sFromText;
			}
			//Пытаемся отослать данные клиенту
			strcpy(szResp, sResp.c_str());
			printf("\nSending response from server\n");
			retVal = send(clientSock, szResp, 256, 0);
			if (retVal == SOCKET_ERROR)
			{
				printf("Unable to send\n");
				system("pause");
				return SOCKET_ERROR;
			}
			closesocket(clientSock);
			printf("Connection closed\n");
		}
	}
	//Закрываем сокет	
	closesocket(servSock);
	WSACleanup();
	system("pause");
	return 0;
}
