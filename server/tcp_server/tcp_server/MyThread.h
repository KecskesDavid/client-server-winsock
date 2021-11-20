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
	std::string createUserNameList();
	void sendUsernames();
	std::string createGroupResponse(std::string groupName);
	void createGroup(std::string, std::string);
	std::string createSuccessfullyAddedToGroup(std::string groupName);
	void addMemberToGroup(std::string groupName, std::string receiver);
	std::string createGroupMessage(std::string message, std::string sender, std::string groupName);
	void sendGroup(std::string message, std::string sender, std::string groupName);
	std::string createPublicMessage(std::string message);
	void sendPublic(std::string);
	std::string createPrivateMessage(std::string message);
	void sendPrivate(std::string message, std::string receiver);
	void mapRequest(RequestType requestType, std::string& sender, std::string& receiver, std::string& message);
	virtual void run();
};

