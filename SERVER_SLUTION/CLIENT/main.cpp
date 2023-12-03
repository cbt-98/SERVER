#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include "comdefine.h"

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
        //Input resqust command
        cout << "Please enter the data to be sent:" << endl;
        cin >> vcwBuf.data();
        string stInput = vcwBuf.data();
        if (stInput.compare("login") == 0)
        {
            MsgLogin_S loginData = {};

            cout << "User:";
            cin.ignore();
            cin.getline(loginData.user, 64);

            cout << "Passward:";
            cin.ignore();
            cin.getline(loginData.pwd, 64);
            ret = send(clientSock, (char *) & loginData, sizeof(loginData), 0);
            if (ret < 0)
            {
                cout << "send failed! Error code:" << WSAGetLastError() << endl;
            }
        }
        else if (stInput.compare("logout") == 0)
        {
            MsgLogout_S logoutData = {};

            cout << "User:";
            cin.ignore();
            cin.getline(logoutData.user, 64);
            ret = send(clientSock, (char*)&logoutData, sizeof(logoutData), 0);
            if (ret < 0)
            {
                cout << "send failed! Error code:" << WSAGetLastError() << endl;
            }
        }
        else
        {
            cout << "Unknow CMD!" << endl;
            continue;
        }

        //Get result
        MsgResult_S retData = {};
        ret = recv(clientSock, (char *)&retData, sizeof(retData), 0);
        if (ret < 0)
        {
            cout << "recv failed! Error code:" << WSAGetLastError() << endl;
        }
        else
        {
            cout << "result:" << retData.ret << endl;
        }

        Sleep(1000);
    }
    closesocket(clientSock);
    WSACleanup();
}
