#pragma once
#include <list>
#include <stdio.h>
#include "winsock2.h"
#include "ws2tcpip.h"
#include "SysThread.h"
#include "synchapi.h" 
#include <string>

class MyThread: public SysThread {
private:
	SOCKET mSock;
	std::string mSocketName;
	std::list<MyThread*>& threadList;
	CRITICAL_SECTION& criticalSession;
	int mNumber;

public:
	MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession);
	virtual void run();
};

