#include "MyThread.h"
#include <iostream>

MyThread::MyThread(SOCKET mSock, std::list<MyThread*>& threadList, CRITICAL_SECTION& criticalSession): threadList(threadList), criticalSession(criticalSession) {
    this->mSock = mSock;
    this->mSocketName = "";
    this->mNumber = 0;
}

//-------------- Creating the socket, naming the user --------------//
void MyThread::createSocket(std::string sender) {
    // std::cout << "Creating socket!" << std::endl;
    
    this->mSocketName = sender;
    this->mNumber = this->threadList.size();
    sendUsernames();
}


//-------------- Send usernames --------------//
std::string MyThread::createUserNameList() {
    std::string listOfNames = "#";
    for (auto& x : this->threadList) {
        listOfNames.append(x->mSocketName + "|");
    }
    return listOfNames;
}

//-------------- Send usernames --------------//
void MyThread::sendUsernames()
{
    std::string listOfNames = createUserNameList();
    for (auto& x : this->threadList) {
        if (send(x->mSock, listOfNames.c_str(), listOfNames.length(), 0) == SOCKET_ERROR)
        {
            printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
            closesocket(x->mSock);
        }
    }
}

std::string MyThread::createGroupResponse(std::string groupName) {
    return "GROUP >> You have successfully created the group: " + groupName;
}

//-------------- Creating group, naming the group, add himself --------------//
void MyThread::createGroup(std::string groupName, std::string sender) {
    // std::cout << "Creating group!" << std::endl;

    this->groupMember.insert(this);
    this->groupName = groupName;

    std::string responseMessage = createGroupResponse(groupName);
    if (send(this->mSock, responseMessage.c_str(), responseMessage.length(), 0) == SOCKET_ERROR)
    {
        printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
        closesocket(this->mSock);
    }
    return;
}


//-------------- Create response if the user is succesfully added --------------//
std::string MyThread::createSuccessfullyAddedToGroup(std::string groupName) {
    return "GROUP >> You have been successfully added to the group: " + groupName;
}

//-------------- Adding new member to specific group --------------//
void MyThread::addMemberToGroup(std::string groupName, std::string receiver)
{
    for (auto& x : this->threadList) {
        if (x->mSocketName == receiver) {
            // Adding the member to the group
            x->groupName = groupName;
            std::string message = createSuccessfullyAddedToGroup(groupName);
            if (send(x->mSock, message.c_str(), message.length(), 0) == SOCKET_ERROR)
            {
                printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
                closesocket(x->mSock);
            }
            return;
        }
    }
}

//-------------- Create group message --------------//
std::string MyThread::createGroupMessage(std::string message, std::string sender, std::string groupName) {
    return "GROUP >> " + groupName + " >> FROM >> " + sender + " >> " + message;
}

//-------------- Send group message --------------//
void MyThread::sendGroup(std::string message, std::string sender, std::string groupName)
{
    std::string finalMessage = createGroupMessage(message, sender, groupName);
    for (auto& x : this->threadList) {
        if (x->groupName == groupName) {
            if (send(x->mSock, finalMessage.c_str(), finalMessage.length(), 0) == SOCKET_ERROR)
            {
                printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
                closesocket(x->mSock);
            }
        }
    }
}


//-------------- Create public message --------------//
std::string MyThread::createPublicMessage(std::string message) {
    return "PUBLIC >> " + this->mSocketName + " >> " + message;
}

//-------------- Send public message --------------//
void MyThread::sendPublic(std::string message)
{
    std::string finalMessage = createPublicMessage(message);
    for (auto& x : this->threadList) {
        if (send(x->mSock, finalMessage.c_str(), finalMessage.length(), 0) == SOCKET_ERROR)
        {
            printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
            closesocket(x->mSock);
        }
    }
}


//-------------- Create private message --------------//
std::string MyThread::createPrivateMessage(std::string message) {
    return "PRIVATE >> " + this->mSocketName + " >> " + message;
}

//-------------- Send private message --------------//
void MyThread::sendPrivate(std::string message, std::string receiver) 
{
    std::string finalMessage = createPrivateMessage(message);
    for (auto& x : this->threadList) {
        if (x->mSocketName == receiver) {
            if (send(x->mSock, finalMessage.c_str(), finalMessage.length(), 0) == SOCKET_ERROR)
            {
                printf("Hiba a kuldesnel a kovetkezo hibakoddal: %d\n", WSAGetLastError());
                closesocket(x->mSock);
            }
            return;
        }
    }
}

//-------------- Mapping the request from the client to the correct function --------------//
void MyThread::mapRequest(RequestType requestType, std::string& sender, std::string& receiver, std::string& message) {
    switch (requestType)
    {
    case RequestType::CREATE_SOCKET: MyThread::createSocket(sender); break;
    case RequestType::CREATE_GROUP: MyThread::createGroup(message, sender); break;
    case RequestType::SEND_PUBLIC: MyThread::sendPublic(message); break;
    case RequestType::SEND_GROUP: MyThread::sendGroup(message, sender, receiver); break;
    case RequestType::SEND_PRIVATE: MyThread::sendPrivate(message, receiver); break;
    case RequestType::ADD_TO_GROUP: MyThread::addMemberToGroup(message, receiver); break;
    default: break;
    }
}

std::string getRequestName(RequestType requestType) {
    switch (requestType)
    {
    case RequestType::CREATE_SOCKET: return "create socket"; break;
    case RequestType::CREATE_GROUP: return "create group"; break;
    case RequestType::SEND_PUBLIC: return "send public"; break;
    case RequestType::SEND_GROUP: return "send group"; break;
    case RequestType::SEND_PRIVATE: return "send private"; break;
    case RequestType::ADD_TO_GROUP: return "add to group"; break;
    default: break;
    }
}

void printMessage(RequestType requestType, std::string& sender, std::string& receiver, std::string& message) {
    printf("Message:\n");
    std::cout << "request type: " << getRequestName(requestType) << std::endl;
    std::cout << "sender: " << sender << std::endl;
    std::cout << "receiver: " << receiver << std::endl;
    std::cout << "message: " << message << std::endl << std::endl;
}

//-------------- Takes every parameter that a request can have then the message and splits it --------------//
void splitRequest(RequestType& requestType, std::string& sender, std::string& receiver, std::string& message, char* RecvBuf, int BufLen) {
    int splitCase = 0;
    int request{};

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
        case 0: request = (int)RecvBuf[i] - 48; break;
        case 1: sender.append(1, RecvBuf[i]); break;
        case 2: receiver.append(1, RecvBuf[i]); break;
        case 3: message.append(1, RecvBuf[i]); break;
        default:
            break;
        }
    }
    
    switch (request)
    {
    case 0:  requestType = RequestType::CREATE_SOCKET; break;
    case 1:  requestType = RequestType::CREATE_GROUP; break;
    case 2:  requestType = RequestType::SEND_PUBLIC; break;
    case 3:  requestType = RequestType::SEND_GROUP; break;
    case 4:  requestType = RequestType::SEND_PRIVATE; break;
    case 5:  requestType = RequestType::ADD_TO_GROUP; break;
    default: break;
    }

    printMessage(requestType, sender, receiver, message);
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

        RequestType requestType;
        std::string sender;
        std::string receiver;
        std::string message;

        splitRequest(requestType, sender, receiver, message, RecvBuf, BufLen);
        mapRequest(requestType, sender, receiver, message);

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