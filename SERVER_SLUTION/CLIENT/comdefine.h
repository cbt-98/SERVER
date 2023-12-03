#ifndef _COMDEFINE_H
#define _COMDEFINE_H

#include <string>

using namespace std;

enum CMD_E
{
	LOGIN,
	LOGOUT,
	RESULT,
};

struct MsgHeader_S
{
	int len;
	CMD_E cmd;
};

struct MsgLogin_S:MsgHeader_S
{
	MsgLogin_S()
	{
		len = sizeof(MsgLogin_S);
		cmd = LOGIN;
		memset(user, 0, sizeof(user));
		memset(pwd, 0, sizeof(pwd));
	}
	char user[64];
	char pwd[64];
};

struct MsgResult_S:MsgHeader_S
{
	MsgResult_S()
	{
		len = sizeof(MsgLogin_S);
		cmd = RESULT;
		ret = -1;
	}
	short ret;
};

struct MsgLogout_S:MsgHeader_S
{
	MsgLogout_S()
	{
		len = sizeof(MsgLogout_S);
		cmd = LOGOUT;
		memset(user, 0, sizeof(user));
	}
	char user[64];
};

#endif
