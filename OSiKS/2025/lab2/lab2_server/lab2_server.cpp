#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

const int PORT = 2007;
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    SOCKET server_socket = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int result;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    result = bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    while(true)
    {
        // Listen for connections
        result = listen(server_socket, SOMAXCONN);
        if (result == SOCKET_ERROR) {
            std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        std::cout << "Server listening on port " << PORT << std::endl;

        // Accept client connection
        client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
        std::cout << "Client connected!" << std::endl;

        // Receive message from client
        result = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (result > 0) {
            buffer[result] = '\0'; // Null-terminate the string
            std::cout << "Message from client: " << buffer << std::endl;

            // Send response to client
            std::string response = "";
            if (result != 6 && result % 7 != 6)
                response = "Incorrect data sent!";
            else {
                std::istringstream buffer_stream(buffer);
                int ticket_number = 0, ticket_number_saved = 0;
                while(buffer_stream >> ticket_number)
                {
                    ticket_number_saved = ticket_number;
                    int sum_first_half = 0, sum_second_half = 0;
                    for (int i = 0; i < 3; i++) {
                        sum_first_half += ticket_number % 10;
                        ticket_number /= 10;
                    }
                    for (int i = 0; i < 3; i++) {
                        sum_second_half += ticket_number % 10;
                        ticket_number /= 10;
                    }
                    if (sum_first_half == sum_second_half) {
                        response = response + std::to_string(ticket_number_saved) + ' ';
                    }
                }
                if (response == "") {
                    response = "The were no lucky tickets in your message.";
                }
            }

            result = send(client_socket, response.c_str(), (int)response.length(), 0);
            if (result == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            }
            else {
                std::cout << "Response sent to client: " << response << std::endl;
            }
        }
        else if (result == 0) {
            std::cout << "Connection closed by client" << std::endl;
        }
        else {
            std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
        }
    }

    // Cleanup
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    std::cout << "Server shutdown complete" << std::endl;
    return 0;
}