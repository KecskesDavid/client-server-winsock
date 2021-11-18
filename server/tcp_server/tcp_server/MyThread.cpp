#include "MyThread.h"
#include <iostream>

MyThread::MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession): threadList(threadList), criticalSession(criticalSession) {
    this->mSock = mSock;
    this->mSocketName = "";
    this->mNumber = 0;
}

void splitRequest(int& requestType, std::string& sender, std::string& receiver, std::string& message, char* RecvBuf, int BufLen) {
    int splitCase = 0;
    for (int i = 0; i < BufLen; i++) {
        if (RecvBuf[i] == '|') {
            splitCase++;
            continue;
        }
        if (RecvBuf[i] == '\0') {
            break;
        }

        switch (splitCase)
        {
        case 0: requestType = (int)RecvBuf[i] - 48; break;
        case 1: sender.append(1, RecvBuf[i]); break;
        case 2: receiver.append(1, RecvBuf[i]); break;
        case 3: message.append(1, RecvBuf[i]); break;
        default:
            break;
        }
    }
}

void printMessage(int requestType, std::string& sender, std::string& receiver, std::string& message) {
    printf("Message...\n");
    std::cout << "request type: " << requestType << std::endl;
    std::cout << "sender: " << sender << std::endl;
    std::cout << "receiver: " << receiver << std::endl;
    std::cout << "message: " << message << std::endl << std::endl;
}

void MyThread::run() {
    int iResult;
    const int BufLen = 1024;
    char RecvBuf[BufLen] = {};
    char SendBuf[BufLen] = {};

    while (1) {
        printf("Receiving message...\n");
        iResult = recv(this->mSock, RecvBuf, BufLen, 0);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(this->mSock);
        }

        int requestType;
        std::string sender;
        std::string receiver;
        std::string message;

        splitRequest(requestType, sender, receiver, message, RecvBuf, BufLen);
        printMessage(requestType, sender, receiver, message);

        EnterCriticalSection(&this->criticalSession);
        for (auto thread = this->threadList.begin(); thread != threadList.end(); thread++) {
            if ((*thread)->isExited()) {
                delete* thread;
                threadList.erase(thread);
            }
        }
        LeaveCriticalSection(&this->criticalSession);
    }
}