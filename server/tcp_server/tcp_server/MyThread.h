#pragma once
#include <set>
#include <list>
#include <stdio.h>
#include "winsock2.h"
#include "ws2tcpip.h"
#include "SysThread.h"
#include "RequestType.h"
#include "synchapi.h" 
#include <string>

class MyThread: public SysThread {
private:
	SOCKET mSock;
	std::string mSocketName;
	std::list<MyThread*>& threadList;
	std::string groupName;
	std::set<MyThread*> groupMember;
	CRITICAL_SECTION& criticalSession;
	int mNumber;

public:
	MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession);
	void createSocket(std::string);
	void createGroup(std::string);
	void sendPublic(std::string);
	void mapRequest(RequestType requestType, std::string& sender, std::string& receiver, std::string& message);
	virtual void run();
};

