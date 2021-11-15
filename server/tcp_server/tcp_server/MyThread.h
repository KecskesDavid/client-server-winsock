#pragma once
#include <list>
#include <stdio.h>
#include "winsock2.h"
#include "ws2tcpip.h"
#include "SysThread.h"
#include "synchapi.h" 

class MyThread: public SysThread {
private:
	SOCKET mSock;
	std::list<MyThread*>& threadList;
	CRITICAL_SECTION& criticalSession;
public:
	MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession);
	virtual void run();
};

