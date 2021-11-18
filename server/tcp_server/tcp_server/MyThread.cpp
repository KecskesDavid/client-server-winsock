#include "MyThread.h"
#include <iostream>

MyThread::MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession): threadList(threadList), criticalSession(criticalSession) {
    this->mSock = mSock;
    this->mSocketName = "";
    this->mNumber = 0;
}

void splitRequest(int& requestType, std::string sender, char* receiver, char* message, char* RecvBuf, int BufLen) {
    int splitCase = 0;
    for (int i = 0; i < BufLen; i++) {
        if (RecvBuf[i] == '/') {
            splitCase++;
            continue;
        }
        if (RecvBuf[i] == '\0') {
            break;
        }

        switch (splitCase)
        {
        case 0: requestType = (int)RecvBuf[i];
        case 1: sender += RecvBuf[i];
        case 2: receiver += RecvBuf[i];
        case 3: message += RecvBuf[i];
        default:
            break;
        }
    }
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
        char receiver[BufLen] = {};
        char message[BufLen] = {};

        splitRequest(requestType, sender, receiver, message, RecvBuf, BufLen);

        printf("Message...\n");
        printf("%d\n", requestType);
        printf("%\n", sender);
        printf("%S\n", receiver);
        printf("%S\n", message);

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