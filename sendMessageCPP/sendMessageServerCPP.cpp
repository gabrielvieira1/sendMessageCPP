#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Falha ao inicializar o Winsock." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Falha ao criar o socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Falha ao vincular o socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Falha ao aguardar conexões." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);

    std::cout << "Waiting for a client..." << std::endl;

    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Falha ao aceitar conexão." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Falha ao receber mensagem do cliente." << std::endl;
    }
    else {
        std::string message(buffer, bytesReceived);
        std::cout << "Received message: " << message << std::endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
