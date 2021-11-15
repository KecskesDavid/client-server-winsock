#include "MyThread.h"

using namespace std;

MyThread::MyThread(SOCKET mSock, list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession): threadList(threadList), criticalSession(criticalSession) {
    this->mSock = mSock;
}

void MyThread::run() {
    int iResult;
    const int BufLen = 1024;
    char RecvBuf[BufLen] = {};
    char SendBuf[BufLen] = {};

    iResult = recv(this->mSock, RecvBuf, BufLen, 0);

    EnterCriticalSection(&this->criticalSession);
    scanf_s("%s", SendBuf, sizeof(SendBuf));
    for (auto thread = this->threadList.begin(); thread != threadList.end(); thread++) {
        if ((*thread)->isExited()) {
            delete* thread;
            threadList.erase(thread);   
        }
        else {
            printf("%s", SendBuf);
            iResult = send((*thread)->mSock, SendBuf, BufLen, 0);
            if (iResult == SOCKET_ERROR)
            {
                printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
                closesocket((*thread)->mSock);
            }
        }
    }
    LeaveCriticalSection(&this->criticalSession);
}