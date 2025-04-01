#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

const int BUFFER_SIZE = 1024;

// Function to receive messages from the server
void ReceiveMessages(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection to the server lost." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0'; // Null-terminate the received message
        std::cout << buffer << std::endl; // Print the message
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Server address
    std::string serverIP;
    std::cout << "Enter server IP: ";
    std::cin >> serverIP;

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2004); // Server port
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

    // Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    //Sending username
    std::cout << "Connected to the server. Input your username." << std::endl;
    std::string message;
    while (true)
    {
        std::getline(std::cin, message);
        if (send(clientSocket, message.c_str(), message.size() + 1, 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send name." << std::endl;
            break;
        }
    }

    // Start a thread to receive messages from the server
    std::thread receiveThread(ReceiveMessages, clientSocket);
    receiveThread.detach();

    // Main loop to send messages to the server
    
    while (true) {
        std::string message;
        std::getline(std::cin, message); // Read input from the user

        if (message == "/exit") {
            std::cout << "Disconnecting from the server..." << std::endl;
            break;
        }

        // Send the message to the server
        if (send(clientSocket, message.c_str(), message.size() + 1, 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message." << std::endl;
            break;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}