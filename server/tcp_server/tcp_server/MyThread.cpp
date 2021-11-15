#include "MyThread.h"

MyThread::MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession): threadList(threadList), criticalSession(criticalSession) {
    this->mSock = mSock;
    this->mSocketName = "";
    this->mNumber = 0;
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