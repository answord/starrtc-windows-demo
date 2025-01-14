#include "stdafx.h"
#include "VOIPForCSharpe.h"
#include "CStarRTCMenuDialog.h"
#include "CUserManager.h"
#include "CLogin.h"
#include "CVoipDlg.h"
#include "Resource.h"
#include "YpVoip.h"

YpVoip *ypVoip;

bool checkVoip()
{
	if (ypVoip == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

BOOL YPLogin(char* localUserId)
{
	if (ypVoip == NULL)
	{
		ypVoip = new YpVoip();
	}

	string strLocalId = localUserId;
	if(ypVoip->login(strLocalId))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL YPCall(char* targetUserId, BOOL showSelfVideo, BOOL showOtherVideo)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return FALSE;
	}

	string strTargetId = targetUserId;
	if (ypVoip->call(strTargetId, showSelfVideo, showOtherVideo))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL YPCallAudio(char* targetUserId)
{
	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return FALSE;
	}

	string strTargetId = targetUserId;
	if (ypVoip->callAudio(strTargetId))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void YPCancel()
{
	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return;
	}

	ypVoip->cancel();
}

void YPAccept(char* fromID, BOOL showSelfVideo, BOOL showOtherVideo)
{
	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return;
	}

	string strFromID = fromID;
	ypVoip->accept(strFromID, showSelfVideo, showOtherVideo);
}

void YPAcceptAudio(char* fromID)
{
	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return;
	}

	string strFromID = fromID;
	ypVoip->acceptAudio(strFromID);
}

void YPRefuse()
{
	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return;
	}

	ypVoip->refuse();
}

void YPHangup()
{
	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return;
	}

	ypVoip->hangup();
}

void YPDeleteHBITMAP(HBITMAP hBitmap, uint8_t* videoDataRGB)
{
	DeleteObject(hBitmap);
	hBitmap = NULL;

	delete[] videoDataRGB;
	videoDataRGB = NULL;
}

void YPDeleteVoip()
{
	if (ypVoip != NULL)
	{
		delete ypVoip;
		ypVoip = NULL;
	}
}

void YPOnCalling(OnCallingCallback callback)
{
	ypVoip->pOnCalling = callback;
}

void YPOnAudioCalling(OnAudioCallingCallback callback)
{
	ypVoip->pOnAudioCalling = callback;
}

void YPOnCancled(OnCancledCallback callback)
{
	ypVoip->pOnCancled = callback;
}
void YPOnRefused(OnCancledCallback callback)
{
	ypVoip->pOnRefused = callback;
}

void YPOnBusy(OnBusyCallback callback)
{
	ypVoip->pOnBusy = callback;
}

void YPOnConnected(OnConnectedCallback callback)
{
	ypVoip->pOnConnected = callback;
}

void YPOnHangup(OnHangupCallback callback)
{
	ypVoip->pOnHangup = callback;
}

void YPOnError(OnErrorCallback callback)
{
	ypVoip->pOnError = callback;
}

void YPOnLine(OnLineCallback callback)
{
	ypVoip->pOnLine = callback;
}

void YPOnGetSelfVideoRaw(OnGetVideoRawCallback callback)
{
	ypVoip->pOnGetSelfVideoRaw = callback;
}

void YPOnGetOtherVideoRaw(OnGetVideoRawCallback callback)
{
	ypVoip->pOnGetOtherVideoRaw = callback;
}