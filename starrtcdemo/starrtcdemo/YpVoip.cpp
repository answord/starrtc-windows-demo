// CVoipDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "starrtcdemo.h"
#include "YpVoip.h"
#include "VOIPForCSharpe.h"
#include "afxdialogex.h"
#include "CropType.h"
#include "CLogin.h"

enum VOIP_LIST_REPORT_NAME
{
	VOIP_VIDEO_NAME = 0,
	VOIP_VIDEO_STATUS,
	VOIP_VIDEO_ID,
	VOIP_VIDEO_CREATER
};


YpVoip::YpVoip()
{
	m_strTargetId = "";
	m_pSoundManager = new CSoundManager(this);

	m_bShowSelfVideo = false;
	m_bShowOtherVideo = false;

	pOnCalling = NULL;
	pOnAudioCalling = NULL;
	pOnCancled = NULL;
	pOnRefused = NULL;
	pOnBusy = NULL;
	pOnConnected = NULL;
	pOnHangup = NULL;
	pOnError = NULL;
	pOnLine = NULL;

	pOnGetSelfVideoRaw = NULL;
	pOnGetOtherVideoRaw = NULL;

	InitializeCriticalSection(&m_critPicture);
}

YpVoip::~YpVoip()
{
	if (m_pVoipManager != NULL)
	{
		delete m_pVoipManager;
		m_pVoipManager = NULL;
	}
	stopGetData();
	if (m_pSoundManager != NULL)
	{
		delete m_pSoundManager;
		m_pSoundManager = NULL;
	}

}


// voip callback
/**
 * 被叫方响应
 * 被叫方收到主叫方的呼叫
 * @param fromID
 */
void YpVoip::onCalling(string fromID)
{
	if (pOnCalling != NULL)
	{
		pOnCalling((char*)fromID.c_str());
	}

	//CString str;
	//str.Format("是否同意用户:%s请求视频通话", fromID.c_str());
	//if (IDYES == AfxMessageBox(str, MB_YESNO))
	//{
	//	m_bAudio = false;
	//	m_strTargetId = fromID;
	//	m_pVoipManager->accept(fromID);
	//	m_bConnect = true;

	//	startGetData((CROP_TYPE)m_pUserManager->m_ServiceParam.m_CropType, true);
	//	if (m_pSoundManager != NULL)
	//	{
	//		m_pSoundManager->startSoundData(true);
	//	}
	//}
	//else
	//{
	//	m_pVoipManager->refuse();
	//}
}

/**
* 被叫方响应语音呼叫
* 被叫方收到主叫方的呼叫
* @param fromID
*/
void YpVoip::onAudioCalling(string fromID)
{
	if (pOnAudioCalling != NULL)
	{
		pOnAudioCalling((char*)fromID.c_str());
	}

	//CString str;
	//str.Format("是否同意用户:%s请求语音通话", fromID.c_str());
	//if (IDYES == AfxMessageBox(str, MB_YESNO))
	//{
	//	m_bAudio = true;
	//	m_strTargetId = fromID;
	//	m_pVoipManager->accept(fromID);
	//	m_bConnect = true;

	//	//startGetData((CROP_TYPE)m_pUserManager->m_ServiceParam.m_CropType, true);
	//	if (m_pSoundManager != NULL)
	//	{
	//		m_pSoundManager->startSoundData(true);
	//	}
	//}
	//else
	//{
	//	m_pVoipManager->refuse();
	//}
}

/**
 * 被叫方响应
 * 主叫方在被叫方接听之前挂断（通话被取消）
 * @param fromID
 */
void YpVoip::onCancled(string fromID)
{
	stopGetData();
	m_strTargetId = "";
	if (pOnCancled != NULL)
	{
		pOnCancled((char*)fromID.c_str());
	}
}

/**
 * 主叫方响应
 * 被叫方拒绝接通（通话被拒绝）
 * @param fromID
 */
void YpVoip::onRefused(string fromID)
{
	stopGetData();
	m_strTargetId = "";
	if (pOnRefused != NULL)
	{
		pOnRefused((char*)fromID.c_str());
	}
}

/**
 * 主叫方响应
 * 被叫方线路忙（对方通话中）
 * @param fromID
 */
void YpVoip::onBusy(string fromID)
{
	//关闭
	stopGetData();
	m_strTargetId = "";
	if (pOnBusy != NULL)
	{
		pOnBusy((char*)fromID.c_str());
	}
}

/**
 * 通话中错过的来电
 */
void YpVoip::onMiss(CIMMessage* pMsg)
{
}

/**
 * 主叫方响应
 * 被叫方接通（通话开始）
 * @param fromID
 */
void YpVoip::onConnected(string fromID)
{
	//设置插入数据
	m_bInsertData = true;
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->startSoundData(true);
	}
	m_bConnect = true;
	if (pOnConnected != NULL)
	{
		pOnConnected((char*)fromID.c_str());
	}
}

/**
 * 对方已经挂断
 * @param fromID
 */
void YpVoip::onHangup(string fromID)
{
	stopGetData();
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopSoundData();
	}

	if (m_pVoipManager != NULL)
	{
		if (m_bConnect)
		{
			m_pVoipManager->hangup(0);
		}
		else
		{
			m_pVoipManager->cancel();
		}
		m_bConnect = false;
	}
	m_strTargetId = "";
	if (pOnHangup != NULL)
	{
		pOnHangup((char*)fromID.c_str());
	}
}

/**
 * voip报错
 * @param errorCode
 */
void YpVoip::onError(string errorCode)
{
	//开启自己
	stopGetData();
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopSoundData();
	}
	if (m_pVoipManager != NULL)
	{
		m_pVoipManager->hangup(0);
	}
	//断开连接
	//CString strErr;
	//strErr.Format("err:%s", errorCode.c_str());
	//AfxMessageBox(strErr);
	m_strTargetId = "";
	if (pOnError != NULL)
	{
		pOnError((char*)errorCode.c_str());
	}
}

/**
 * 收到实时数据
 * @param data
 */
void YpVoip::onReceiveRealtimeData(uint8_t* data, int length)
{
}

int YpVoip::getVideoRaw(string strUserId, int w, int h, uint8_t* videoData, int videoDataLen)
{
	if (!m_bShowOtherVideo)
	{
		return 0;
	}

	try
	{
		uint8_t* videoDataRGB = NULL;
		if (videoData != NULL && videoDataLen > 0)
		{
			videoDataRGB = new uint8_t[w*h * 3];
			CUtil::yuv420p_to_rgb24(videoData, videoDataRGB, w, h);
		}

		EnterCriticalSection(&m_critPicture);
		if (videoDataRGB != NULL && videoDataLen > 0)
		{
			CImage image;       //使用图片类
			image.Create(w, h, 24, 0);
			//首地址  
			byte* pRealData = (byte*)image.GetBits();
			//首地址    
			//行距  
			int pit = image.GetPitch();
			for (int i = 0; i < w; i++)
			{
				for (int j = 0; j < h; j++)
				{
					*(pRealData + pit * j + i * 3) = (int)videoDataRGB[3 * j*w + 3 * i];
					*(pRealData + pit * j + i * 3 + 1) = (int)videoDataRGB[3 * j*w + 3 * i + 1];
					*(pRealData + pit * j + i * 3 + 2) = (int)videoDataRGB[3 * j*w + 3 * i + 2];
				}
			}
			HBITMAP hbmp = image.Detach();
			pOnGetOtherVideoRaw((char*)m_pUserManager->m_ServiceParam.m_strUserId.c_str(), w, h, hbmp, videoDataRGB, videoDataLen);

			//DeleteObject(hbmp);
			//delete[] videoDataRGB;
			//videoDataRGB = NULL;
		}
		LeaveCriticalSection(&m_critPicture);
	}
	catch (char *str)
	{
		printf(str);
	}

	return 0;
}

/**
 * 数据传输模式切换
 * @param state
 */
void YpVoip::onTransStateChanged(int state)
{
}

void YpVoip::stopLive()
{
	stopGetData();
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopSoundData();
	}

	if (m_pVoipManager != NULL)
	{
		if (m_bConnect)
		{
			m_pVoipManager->hangup(1);
		}
		else
		{
			m_pVoipManager->cancel();
		}
		m_bConnect = false;
	}
	m_strTargetId = "";
}

void YpVoip::addUpId()
{
}

void YpVoip::insertVideoRaw(uint8_t* videoData, int dataLen, int isBig)
{
	if (m_pVoipManager != NULL && m_bAudio == false)
	{
		m_pVoipManager->insertVideoRaw(videoData, dataLen, isBig);
	}
}

int YpVoip::cropVideoRawNV12(int w, int h, uint8_t* videoData, int dataLen, int yuvProcessPlan, int rotation, int needMirror, uint8_t* outVideoDataBig, uint8_t* outVideoDataSmall)
{
	int ret = 0;
	if (m_pVoipManager != NULL)
	{
		ret = m_pVoipManager->cropVideoRawNV12(w, h, videoData, dataLen, (int)CROP_TYPE::VIDEO_CROP_CONFIG_480BW_480BH_SMALL_NONE, 0, 0, outVideoDataBig, outVideoDataSmall);
	}
	return ret;
}
void YpVoip::drawPic(YUV_TYPE type, int w, int h, uint8_t* videoData, int videoDataLen)
{
	if (!m_bShowSelfVideo)
	{
		return;
	}
	
	try
	{
		uint8_t* videoDataRGB = NULL;
		if (videoData != NULL && videoDataLen > 0)
		{
			videoDataRGB = new uint8_t[w*h * 3];
			if (type == FMT_NV12 || type == FMT_NV21)
			{
				CUtil::yuv420sp_to_rgb24(type, videoData, videoDataRGB, w, h);
			}
			else if (type == FMT_YUV420P)
			{
				CUtil::yuv420p_to_rgb24(videoData, videoDataRGB, w, h);
			}
			else
			{
				memcpy(videoDataRGB, videoData, sizeof(uint8_t)*videoDataLen);
			}
		}

		EnterCriticalSection(&m_critPicture);
		if (videoDataRGB != NULL && videoDataLen > 0)
		{
			CImage image;       //使用图片类
			image.Create(w, h, 24, 0);
			//首地址  
			byte* pRealData = (byte*)image.GetBits();
			//首地址    
			//行距  
			int pit = image.GetPitch();
			for (int i = 0; i < w; i++)
			{
				for (int j = 0; j < h; j++)
				{
					*(pRealData + pit * j + i * 3) = (int)videoDataRGB[3 * j*w + 3 * i];
					*(pRealData + pit * j + i * 3 + 1) = (int)videoDataRGB[3 * j*w + 3 * i + 1];
					*(pRealData + pit * j + i * 3 + 2) = (int)videoDataRGB[3 * j*w + 3 * i + 2];
				}
			}
			HBITMAP hbmp = image.Detach();
			pOnGetSelfVideoRaw((char*)m_pUserManager->m_ServiceParam.m_strUserId.c_str(), w, h, hbmp, videoDataRGB, videoDataLen);

			//DeleteObject(hbmp);
			//delete[] videoDataRGB;
			//videoDataRGB = NULL;
		}
		LeaveCriticalSection(&m_critPicture);
	}
	catch (char *str)
	{
		printf(str);
	}
}

void YpVoip::onNewMessage(CIMMessage* var1)
{
	CString strMsg = "";
	strMsg.Format("%s:%s", var1->m_strFromId.c_str(), var1->m_strContentData.c_str());
	//m_HistoryMsg.InsertString(0, strMsg);
}

void YpVoip::changeShowStyle(string strUserId)
{
}
void YpVoip::closeCurrentLive()
{
}
void YpVoip::getLocalSoundData(char* pData, int nLength)
{
	if (m_pVoipManager != NULL)
	{
		m_pVoipManager->insertAudioRaw((uint8_t*)pData, nLength);
	}
}

void YpVoip::querySoundData(char** pData, int* nLength)
{
	if (m_pVoipManager != NULL)
	{
		m_pVoipManager->querySoundData((uint8_t**)pData, nLength);
	}
}

/**
 * msgServer处于在线状态
 */
int YpVoip::online(int maxContentLen)
{
	if (pOnLine != NULL)
	{
		pOnLine(true);
	}
	return 0;
}

/**
 * msgServer中断状态
 */
int YpVoip::offline()
{
	if (pOnLine != NULL)
	{
		pOnLine(false);
	}
	return 0;
}

bool YpVoip::login(string strLocalId)
{
	m_pUserManager = new CUserManager();
	m_pUserManager->m_ServiceParam.m_strUserId = strLocalId;

	CLogin login(m_pUserManager, this);
	m_pVoipManager = new XHVoipManager(this);
	bool bSuccess = login.logIn();
	return bSuccess;
}

bool YpVoip::call(string strTargetId, bool showSelfVideo, bool showOtherVideo)
{
	m_bShowSelfVideo = showSelfVideo;
	m_bShowOtherVideo = showOtherVideo;

	bool isCallSuccess = false;
	//呼叫对方
	if (m_pVoipManager != NULL)
	{
		isCallSuccess = m_pVoipManager->call(strTargetId);
	}
	if (!isCallSuccess) {
		return false;
	}

	startGetData((CROP_TYPE)m_pUserManager->m_ServiceParam.m_CropType, false);
	return true;
}

bool YpVoip::callAudio(string strTargetId)
{
	m_bShowSelfVideo = false;
	m_bShowOtherVideo = false;

	bool isCallSuccess = false;
	m_bAudio = true;
	//呼叫对方
	if (m_pVoipManager != NULL)
	{
		isCallSuccess = m_pVoipManager->audioCall(strTargetId);
	}
	if (!isCallSuccess) {
		return false;
	}

	//startGetData((CROP_TYPE)m_pUserManager->m_ServiceParam.m_CropType, false);
	return true;
}

void YpVoip::cancel()
{
	stopLive();
}

void YpVoip::accept(string fromID, bool showSelfVideo, bool showOtherVideo)
{
	m_bShowSelfVideo = showSelfVideo;
	m_bShowOtherVideo = showOtherVideo;

	m_bAudio = false;
	m_strTargetId = fromID;
	
	m_pVoipManager->accept(fromID);
	m_bConnect = true;

	startGetData((CROP_TYPE)m_pUserManager->m_ServiceParam.m_CropType, true);
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->startSoundData(true);
	}
}

void YpVoip::acceptAudio(string fromID)
{
	m_bShowSelfVideo = false;
	m_bShowOtherVideo = false;

	m_bAudio = true;
	m_strTargetId = fromID;

	m_pVoipManager->accept(fromID);
	m_bConnect = true;

	//startGetData((CROP_TYPE)m_pUserManager->m_ServiceParam.m_CropType, true);
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->startSoundData(true);
	}
}

void YpVoip::refuse()
{
	m_pVoipManager->refuse();
}

void YpVoip::hangup()
{
	stopLive();
}