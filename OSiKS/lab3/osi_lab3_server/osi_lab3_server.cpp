#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>

const int USERNAME_LENGTH = 256;
const int MESSAGE_BUFFER_SIZE = 1024;

struct ClientInfo {
    SOCKET client_socket;
    std::string name;
    std::string ip_address;
    unsigned short port;
};

SOCKET server_socket;
std::vector<ClientInfo> clients;
std::mutex mtx;

void broadcast(const std::string& message, SOCKET sender_socket = INVALID_SOCKET, bool include_sender = false) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& client : clients) {
        // Skip if this is the sender and we shouldn't include them
        if (client.client_socket == sender_socket && !include_sender) {
            continue;
        }
        // Skip clients that haven't set their name yet (except for sender)
        if (client.name.empty() && client.client_socket != sender_socket) {
            continue;
        }
        send(client.client_socket, message.c_str(), message.size() + 1, 0);
    }
}

void handle_client(ClientInfo client_info) {
    char buffer[MESSAGE_BUFFER_SIZE];

    // Get username first
    int bytes_received = recv(client_info.client_socket, buffer, USERNAME_LENGTH, 0);
    if (bytes_received <= 0) {
        closesocket(client_info.client_socket);
        return;
    }
    buffer[bytes_received] = '\0';
    client_info.name = buffer;

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << client_info.name << " connected to the server from "
            << client_info.ip_address << ", port: " << client_info.port << "\n";
        // Update the client's name in the vector
        auto it = std::find_if(clients.begin(), clients.end(),
            [&](const ClientInfo& c) { return c.client_socket == client_info.client_socket; });
        if (it != clients.end()) {
            it->name = client_info.name;
        }
    }

    // Notify all clients about new connection (except the new client)
    std::string join_message = client_info.name + " has joined the server from " +
        client_info.ip_address + ", port: " + std::to_string(client_info.port);
    broadcast(join_message, client_info.client_socket);

    while (true) {
        bytes_received = recv(client_info.client_socket, buffer, MESSAGE_BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            std::string disconnected_name;
            {
                std::lock_guard<std::mutex> lock(mtx);
                auto it = std::find_if(clients.begin(), clients.end(),
                    [&](const ClientInfo& c) { return c.client_socket == client_info.client_socket; });

                if (it != clients.end()) {
                    disconnected_name = it->name;
                    std::cout << disconnected_name << " disconnected.\n";
                    clients.erase(it);
                }
            }

            if (!disconnected_name.empty()) {
                // Notify all clients about disconnection
                broadcast(disconnected_name + " has left the chat.");
            }

            closesocket(client_info.client_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << client_info.name << ": " << buffer << "\n";
        }
        // Broadcast the message to all other clients
        broadcast(client_info.name + ": " + buffer, client_info.client_socket);
    }
}

int main() {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2004);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started on port 2004. Waiting for connections...\n";

    while (true) {
        SOCKADDR_IN client_addr;
        int client_addr_len = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            continue;
        }

        // Get client IP and port
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        unsigned short client_port = ntohs(client_addr.sin_port);

        ClientInfo new_client = { client_socket, "", client_ip, client_port };

        {
            std::lock_guard<std::mutex> lock(mtx);
            clients.push_back(new_client);
        }

        std::thread(handle_client, new_client).detach();
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}