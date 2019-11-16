#pragma once
#pragma once
#include<string>
using namespace std;
extern "C" {
	// 登陆
    __declspec(dllexport) BOOL YPLogin(char* localuserid);
	// 呼叫
	__declspec(dllexport) int VoipCallPerson(char* targetId, char* str);
	// 释放
	__declspec(dllexport) int DeleteVoip();
}
