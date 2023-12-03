#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "comdefine.h"

using namespace std;

int main()
{
    int ret = -1;
    short _port = 8889;
    string _ip = "172.16.8.25";

    WSADATA data;
    ret = WSAStartup(MAKEWORD(2, 2), &data);
    if (ret < 0) {
        cout << "Init failed!" << endl;
        WSACleanup();
        return ret;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    ret = bind(listenSock, (sockaddr*)(&addr), sizeof(addr));
    if (ret < 0)
    {
        cout << "bind failed! Error code:" << ret << endl;
        closesocket(listenSock);
        WSACleanup();
        return ret;
    }
    else
    {
        cout << "bind is successful." << endl;
    }

    ret = listen(listenSock, 5);
    if (ret < 0)
    {
        cout << "listen failed! Error code:" << WSAGetLastError() << endl;
        closesocket(listenSock);
        WSACleanup();
        return ret;
    }
    else
    {
        cout << "listen is successful." << endl;
    }

    sockaddr_in clientAddr = {};
    int len = sizeof(clientAddr);
    SOCKET clientSock = accept(listenSock, (sockaddr*)(&clientAddr), &len);
    cout << "accept:ip:" << inet_ntoa(clientAddr.sin_addr) << endl;

    while (1)
    {
        char* recvBuf[1024] = { 0 };
        ret = recv(clientSock, (char *)recvBuf, sizeof(recvBuf), 0);
        if (ret < 0)
        {
            cout << "recv failed! Error code:" << WSAGetLastError() << endl;
        }

        MsgHeader_S* recvHander = (MsgHeader_S*)recvBuf;
        cout << "CMD:" << recvHander->cmd << endl;

        // Receive resqust command
        switch (recvHander->cmd)
        {
        case LOGIN:
        {
            MsgLogin_S * loginData = (MsgLogin_S*)recvBuf;
            cout << "login user:" << loginData->user << " " << "pwd:" << loginData->pwd << endl;
            ret = 0;
            break;
        }
        case LOGOUT:
        {
            MsgLogout_S* logoutData = (MsgLogout_S*)recvBuf;
            cout << "logout user:" << logoutData->user << endl;
            ret = 0;
            break;
        }
        default:
        {
            ret = -1;
        }
        }

        // Return result
        MsgResult_S RetData = {};
        RetData.ret = ret;
        ret = send(clientSock, (char *)&RetData, sizeof(RetData), 0);
        
        Sleep(1000);
    }

    closesocket(listenSock);
    closesocket(clientSock);
    WSACleanup();
    return 0;
}
