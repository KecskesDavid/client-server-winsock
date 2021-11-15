#include <stdio.h>
#include <list>

#include "winsock2.h"
#include "ws2tcpip.h"
#include "MyThread.h"
#include "SysThread.h"
#include "synchapi.h"
#include "MyThread.h"

#pragma comment(lib, "Ws2_32.lib")

int main() {
	printf("SERVER\n");
	WSADATA wsaData;
	SOCKET listenSocket;
	int port = 27015;

	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a socket for sending data
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);

	if (bind(listenSocket,
		(SOCKADDR*)&service,
		sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}

	if (listen(listenSocket, 3) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(listenSocket);
		WSACleanup();
		return 0;
	}
	int iResult;
	SOCKET acceptSocket;
	printf("Waiting for client to connect...\n");

	CRITICAL_SECTION criticalSession;
	InitializeCriticalSection(&criticalSession);
	std::list<MyThread*> threadList;

	while (1) {
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 0;
		}
		MyThread* newThread = new MyThread(acceptSocket, threadList, criticalSession);
		threadList.push_back(newThread);
		newThread->start();
		printf("Client connected successfully!\n");
	}

	printf("Closing.\n");
	closesocket(acceptSocket);

	printf("Exiting.\n");
	iResult = shutdown(listenSocket, SD_SEND);

	printf("Cleanup.\n");
	WSACleanup();
	return 0;
}
