#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>

using namespace std;

int main()
{
    int ret = -1;
    short _port = 8889;
    string _ip = "127.0.0.1";

    WSADATA data;
    ret = WSAStartup(MAKEWORD(2, 2), &data);
    if (ret < 0) {
        cout << "Init failed!" << endl;
        WSACleanup();
        return ret;
    }

    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.S_un.S_addr = inet_addr(_ip.c_str());

    ret = connect(clientSock, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0)
    {
        cout << "connect failed! Error code:" << ret << endl;
    }
    else
    {
        cout << "connect is successful." << endl;
    }

    vector<char> vcrBuf(256, 0);
    vector<char> vcwBuf(256, 0);
    while (1)
    {
        cout << "Please enter the data to be sent:" << endl;
        cin >> vcwBuf.data();
        cout << "send:" << vcwBuf.data() << endl;
        ret = send(clientSock, vcwBuf.data(), strlen(vcwBuf.data()) + 1, 0);
        if (ret < 0)
        {
            cout << "send failed! Error code:" << WSAGetLastError() << endl;
        }

        ret = recv(clientSock, vcrBuf.data(), vcrBuf.capacity() - 1, 0);
        if (ret < 0)
        {
            cout << "recv failed! Error code:" << WSAGetLastError() << endl;
        }
        else
        {
            cout << "recv:" << vcrBuf.data() << endl;
        }

        Sleep(1000);
    }
    closesocket(clientSock);
    WSACleanup();
}
