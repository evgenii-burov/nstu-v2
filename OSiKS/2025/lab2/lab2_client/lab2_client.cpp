#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

const int PORT = 2007;
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    SOCKET client_socket = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int result;

    // Get server IP from user
    std::string server_ip;
    std::cout << "Enter server IP address (e.g., 127.0.0.1): ";
    std::getline(std::cin, server_ip);

    // Get message from user
    std::string user_message;
    std::cout << "Enter message to send: ";
    std::getline(std::cin, user_message);

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid IP address: " << server_ip << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Connect to server
    result = connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server at " << server_ip << "!" << std::endl;

    // Send user's message to server
    result = send(client_socket, user_message.c_str(), (int)user_message.length(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Message sent to server" << std::endl;

    // Receive response from server
    result = recv(client_socket, buffer, BUFFER_SIZE - 1, 0); // -1 to leave space for null terminator
    if (result > 0) {
        buffer[result] = '\0';
        std::cout << "Response from server: " << buffer << std::endl;
    }
    else if (result == 0) {
        std::cout << "Connection closed by server" << std::endl;
    }
    else {
        std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
    }

    // Cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}