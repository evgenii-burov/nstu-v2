#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

const int PORT = 2004;
const int MAX_CLIENTS = 5;
const int USERNAME_LENGTH = 20;

struct ClientInfo {
    SOCKET socket;
    std::string ipAddress;
    std::string name;
    int port;
};

std::vector<ClientInfo> clients; // Stores client information
std::mutex mtx; // Protects shared resources

void notifyClients(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& client : clients) {
        send(client.socket, message.c_str(), message.size() + 1, 0);
    }
}

void handleClient(ClientInfo client) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(client.socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::lock_guard<std::mutex> lock(mtx);
            SOCKET socket = client.socket;
            // Find the disconnected client in the vector
            auto it = std::find_if(clients.begin(), clients.end(),
                [socket](const ClientInfo& some_client) { return some_client.socket == socket; });

            if (it != clients.end()) {
                std::string leaveMessage = "Client disconnected: " + it->ipAddress + ":" + std::to_string(it->port);
                clients.erase(it); // Remove the client from the vector
                notifyClients(leaveMessage); // Notify other clients
            }

            std::cout << "Client disconnected." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0'; // Null-terminate the received message

        // Lock for thread-safe console output
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    closesocket(client.socket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2004);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 2004" << std::endl;

    while (true) {
        SOCKADDR_IN clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        // Convert the client's IP address to a string
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);

        //Get clients name
        std::string name;
        char name_buffer[USERNAME_LENGTH];
        int bytes_received = recv(clientSocket, name_buffer, sizeof(name_buffer) - 1, 0);
        if(bytes_received < 0)
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << "Couldn't receive a name" << std::endl;
            closesocket(clientSocket);
            continue;
        }
        name_buffer[bytes_received] = '\0'; // Null-terminate the received message
        name = name_buffer;

        // Add the new client to the vector
        {
            std::lock_guard<std::mutex> lock(mtx);
            clients.push_back({ clientSocket, clientIP, name, clientPort });
            std::cout << "New client connected: " << name << " from " << clientIP << ":" << clientPort << std::endl;
        }

        // Notify all clients about the new connection
        std::string joinMessage = "New client connected: " + name + " from " + \
            std::string(clientIP) + ":" + std::to_string(clientPort);
        notifyClients(joinMessage);

        // Create a thread to handle the new client
        std::thread clientThread(handleClient, clients.back());
        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}