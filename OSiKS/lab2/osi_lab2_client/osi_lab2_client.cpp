#pragma comment (lib,"Ws2_32.lib")
#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <iostream> 
#include <cstdio>
using namespace std;
int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA wsaData;
    int retVal = 0;
    WSAStartup(ver, (LPWSADATA)&wsaData);
    LPHOSTENT hostEnt;
    hostEnt = gethostbyname("localhost");
    if (!hostEnt)
    {
        printf("Unable to collect gethostbyname\n");
        WSACleanup();
        system("pause");
        return 1;
    }
    //Создаем сокет
    SOCKET clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock == SOCKET_ERROR)
    {
        printf("Unable to create socket\n");
        WSACleanup();
        system("pause");
        return 1;
    }
    string ip, port;
    cout << "ip>";
    cin >> ip;
    cout << "port>";
    cin >> port;
    cin.ignore();

    SOCKADDR_IN serverInfo;
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    serverInfo.sin_port = htons(stoi(port));
    //Пытаемся присоединится к серверу по ip и port
    retVal = connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to connect\n");
        WSACleanup();
        system("pause");
        return 1;
    }
    printf("Connection made sucessfully\n");
    printf("Enter strings of text ending with a dot:\n\n");
    char pBuf[256];
    //fgets(pBuf,256,stdin);
    cin.getline(pBuf, 256, '.');
    string s = (const char*)pBuf;
    printf("\nSending request from client\n");
    //Отсылаем данные на сервер
    retVal = send(clientSock, pBuf, strlen(pBuf), 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to send\n");
        WSACleanup();
        system("pause");
        return 1;
    }
    char szResponse[256];
    //Пытаемся получить ответ от сервера
    retVal = recv(clientSock, szResponse, 256, 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to recv\n");
        WSACleanup();
        system("pause");
        return 1;
    }
    char* Resp;
    Resp = szResponse;
    if (s.compare(0, 4, "stop"))
        printf("Server response is:\n\n%s", Resp);
    else
        printf("%s\n", Resp);
    closesocket(clientSock);
    WSACleanup();
    system("pause");
    return 0;
}
