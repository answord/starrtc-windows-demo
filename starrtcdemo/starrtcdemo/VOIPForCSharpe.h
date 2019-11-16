#pragma once
#pragma once
#include<string>
using namespace std;

typedef void(__stdcall *OnCallingCallback)(char* fromID);

extern "C" {
	// 登陆(一定要先调用登陆并登陆成功，才能调用其他函数)
    __declspec(dllexport) BOOL YPLogin(char* localUserId);
	// 发起呼叫
	__declspec(dllexport) BOOL YPCall(char* targetUserId);
	// 取消呼叫
	__declspec(dllexport) void YPCancel();
	// 同意通话
	__declspec(dllexport) void YPAccept(char* fromID);
	// 拒绝通话
	__declspec(dllexport) void YPRefuse();
	// 挂断
	__declspec(dllexport) void YPHangup(int isActive);
	// 释放
	__declspec(dllexport) void YPDeleteVoip();

	// 收到呼叫
	__declspec(dllexport) void YPOnCalling(OnCallingCallback callback);
}
