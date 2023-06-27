#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Falha ao inicializar o Winsock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Falha ao criar o socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    // Converter o endereço IP
    if (InetPtonA(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) != 1) {
        std::cerr << "Falha ao converter o endereço IP." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Falha ao conectar ao servidor." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    const char* message = "Hello from client!";
    if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Falha ao enviar a mensagem." << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
