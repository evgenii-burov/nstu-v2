#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    SOCKET client_socket = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int result;

    std::string server_ip;
    std::cout << "Enter server's IP address to connect to: ";
    std::getline(std::cin, server_ip);
    int port = 0;
    std::cout << "Enter server's port: ";
    std::cin >> port;
    std::cin.ignore(256, '\n');

    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Unable to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid IP address: " << server_ip << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    result = connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Unable to connect: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server at " << server_ip << "!" << std::endl;

    std::string user_message;
    std::cout << "Enter message to send (ticket numbers e.g. 123456 789012...)\nor enter \"/c\" to close the server: ";
    std::getline(std::cin, user_message);

    result = send(client_socket, user_message.c_str(), (int)user_message.length(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Unable to send: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Message sent to server" << std::endl;

    result = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (result > 0) {
        buffer[result] = '\0';
        std::cout << "Response from server: " << buffer << std::endl;
    }
    else if (result == 0) {
        std::cout << "Connection closed by server" << std::endl;
    }
    else {
        std::cerr << "Unable to receive: " << WSAGetLastError() << std::endl;
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}