#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <vector>

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

    vector<char> vcrBuf(256, 0);
    vector<char> vcwBuf(256, 0);
    while (1)
    {
        ret = recv(clientSock, vcrBuf.data(), vcrBuf.capacity() - 1, 0);
        if (ret < 0)
        {
            cout << "recv failed! Error code:" << WSAGetLastError() << endl;
        }
        else
        {
            cout << "recv:" << vcrBuf.data() << endl;
        }

        if (strcmp(vcrBuf.data(), "name") == 0)
        {
        
            strncpy_s(vcwBuf.data(), vcwBuf.capacity() - 1, "cbt", strlen("cbt") + 1);
        }
        else
        {
            strncpy_s(vcwBuf.data(), vcwBuf.capacity() - 1, "Copy that.", strlen("Copy that.") + 1);
        }
        
        ret = send(clientSock, vcwBuf.data(), strlen(vcwBuf.data()) + 1, 0);
        if (ret < 0)
        {
            cout << "Recover failed! Error code:" << WSAGetLastError() << endl;
        }
        else
        {
            cout << "Recover:" << vcwBuf.data() << endl;
        }

        Sleep(1000);
    }

    closesocket(listenSock);
    closesocket(clientSock);
    WSACleanup();
    return 0;
}
