#pragma once
#pragma once
#include<string>
using namespace std;

typedef void(__stdcall *OnCallingCallback)(char* fromID);
typedef void(__stdcall *OnCancledCallback)(char* fromID);
typedef void(__stdcall *OnRefusedCallback)(char* fromID);
typedef void(__stdcall *OnBusyCallback)(char* fromID);
typedef void(__stdcall *OnConnectedCallback)(char* fromID);
typedef void(__stdcall *OnHangupCallback)(char* fromID);
typedef void(__stdcall *OnErrorCallback)(char* errorCode);

typedef void(__stdcall *OnGetVideoRawCallback)(char* userId, int w, int h, HBITMAP videoData, int videoDataLen);

extern "C" {
	// 登陆(一定要先调用登陆并登陆成功，才能调用其他函数)
    __declspec(dllexport) BOOL YPLogin(char* localUserId);
	// 发起通话（主叫方调用）
	__declspec(dllexport) BOOL YPCall(char* targetUserId, BOOL showSelfVideo, BOOL showOtherVideo);
	// 主叫方主动取消呼叫（主叫方调用）
	__declspec(dllexport) void YPCancel();
	// 同意跟主叫方通话（被叫方调用）
	__declspec(dllexport) void YPAccept(char* fromID, BOOL showSelfVideo, BOOL showOtherVideo);
	// 拒绝跟主叫方通话（被叫方调用）
	__declspec(dllexport) void YPRefuse();
	// 挂断（双方都可调用）
	__declspec(dllexport) void YPHangup();
	// 释放
	__declspec(dllexport) void YPDeleteVoip();

	// 被叫方收到主叫方的呼叫（被叫方响应）
	__declspec(dllexport) void YPOnCalling(OnCallingCallback callback);
	// 主叫方在被叫方接听之前挂断，即通话被取消（被叫方响应）
	__declspec(dllexport) void YPOnCancled(OnCancledCallback callback);
	// 被叫方拒绝接通（主叫方响应）
	__declspec(dllexport) void YPOnRefused(OnCancledCallback callback);
	// 被叫方线路忙（主叫方响应）
	__declspec(dllexport) void YPOnBusy(OnBusyCallback callback);
	// 被叫方接通，通话开始（主叫方响应）
	__declspec(dllexport) void YPOnConnected(OnConnectedCallback callback);
	// 对方已经挂断（双方都可响应）
	__declspec(dllexport) void YPOnHangup(OnHangupCallback callback);
	// 收到voip报错
	__declspec(dllexport) void YPOnError(OnErrorCallback callback);

	// 收到已方视频数据
	__declspec(dllexport) void YPOnGetSelfVideoRaw(OnGetVideoRawCallback callback);
	// 收到对方视频数据
	__declspec(dllexport) void YPOnGetOtherVideoRaw(OnGetVideoRawCallback callback);
}
