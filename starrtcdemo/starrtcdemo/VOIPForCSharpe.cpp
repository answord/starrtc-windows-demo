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

BOOL YPCall(char* targetUserId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!checkVoip())
	{
		// 未登陆，不允许调用相关方法
		return FALSE;
	}

	string strTargetId = targetUserId;
	if (ypVoip->call(strTargetId))
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

}

void YPAccept(char* fromID)
{

}

void YPRefuse()
{

}

void YPHangup(int isActive)
{

}

void YPDeleteVoip()
{
	if(ypVoip != NULL) {
		delete ypVoip;
		ypVoip = NULL;
	}
}

void YPOnCalling(OnCallingCallback callback)
{
	ypVoip->pOnCalling = callback;
}
