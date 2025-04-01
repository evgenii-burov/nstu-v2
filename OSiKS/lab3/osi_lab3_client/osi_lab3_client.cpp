#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <conio.h>

const int MESSAGE_BUFFER_SIZE = 1024;

std::queue<std::string> queued_received_messages;
std::mutex mtx;

void receive_messages(SOCKET client_socket) {
    char buffer[MESSAGE_BUFFER_SIZE];
    while (true) {
        int bytes_received = recv(client_socket, buffer, MESSAGE_BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Disconnected from server.\n";
            break;
        }
        buffer[bytes_received] = '\0';

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << buffer << "\n";
        }
    }
}

int main() {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    std::string server_ip;
    std::cout << "Enter server IP: ";
    std::getline(std::cin, server_ip);

    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2004);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server.\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::string name;
    std::cout << "Enter your username: ";
    std::getline(std::cin, name);
    send(client_socket, name.c_str(), name.size() + 1, 0);

    std::cout << "Connected to server. Press <enter> to start typing a message:\n";

    // Start a thread to receive messages
    std::thread receiver(receive_messages, client_socket);
    receiver.detach();

    while (true) {
        std::string message;
        while ((_getch()) != '\r');
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << ">";
            std::getline(std::cin, message);
        }
        if (message == "/exit") {
            break;
        }

        if (send(client_socket, message.c_str(), message.size() + 1, 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message.\n";
            break;
        }
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}