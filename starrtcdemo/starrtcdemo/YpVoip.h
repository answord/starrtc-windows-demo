#pragma once
#include "CVoipDataShowView.h"
#include "XHVoipManager.h"
#include "CDataControl.h"
#include "CShowLiveDlg.h"
#include "CSqliteDB.h"
#include "CSoundManager.h"
#include "CUserManager.h"
#include "VOIPForCSharpe.h"
#include "ILoginManagerListener.h"

class YpVoip : public CLocalDataControl, public IVoipManagerListener, public ILoginManagerListener, public CPicControlCallback, public IShowLiveCallback, public ISoundCallback
{

public:
	YpVoip();   // 标准构造函数
	virtual ~YpVoip();

public:
	// voip callback
	/**
	 * 被叫方响应
	 * 被叫方收到主叫方的呼叫
	 * @param fromID
	 */
	virtual void onCalling(string fromID);

	/**
	 * 被叫方响应语音呼叫
	 * 被叫方收到主叫方的呼叫
	 * @param fromID
	 */
	virtual void onAudioCalling(string fromID);

	/**
	 * 被叫方响应
	 * 主叫方在被叫方接听之前挂断（通话被取消）
	 * @param fromID
	 */
	virtual void onCancled(string fromID);

	/**
	 * 主叫方响应
	 * 被叫方拒绝接通（通话被拒绝）
	 * @param fromID
	 */
	virtual void onRefused(string fromID);

	/**
	 * 主叫方响应
	 * 被叫方线路忙（对方通话中）
	 * @param fromID
	 */
	virtual void onBusy(string fromID);

	/**
	 * 通话中错过的来电
	 */
	virtual void onMiss(CIMMessage* pMsg);

	/**
	 * 主叫方响应
	 * 被叫方接通（通话开始）
	 * @param fromID
	 */
	virtual void onConnected(string fromID);

	/**
	 * 对方已经挂断
	 * @param fromID
	 */
	virtual void onHangup(string fromID);

	/**
	 * voip报错
	 * @param errorCode
	 */
	virtual void onError(string errorCode);

	/**
	 * 收到实时数据
	 * @param data
	 */
	virtual void onReceiveRealtimeData(uint8_t* data, int length);

	/**
	 * 收到实时数据
	 */
	virtual int getVideoRaw(string strUserId, int w, int h, uint8_t* videoData, int videoDataLen);

	/**
	* 数据传输模式切换
	* @param state
	*/
	virtual void onTransStateChanged(int state);

public:
	/**
	 * 停止视频回调
	 */
	virtual void stopLive();

	/**
	 * upid增加
	 */
	virtual void addUpId();
	/**
	 * 插入数据回调
	 */
	virtual void insertVideoRaw(uint8_t* videoData, int dataLen, int isBig);
	/**
	 * 裁剪视频数据
	 */
	virtual int cropVideoRawNV12(int w, int h, uint8_t* videoData, int dataLen, int yuvProcessPlan, int rotation, int needMirror, uint8_t* outVideoDataBig, uint8_t* outVideoDataSmall);
	/**
	 * 绘制
	 */
	virtual void drawPic(YUV_TYPE type, int w, int h, uint8_t* videoData, int videoDataLen);
	// msg callback
	/**
	 * 接收到消息
	 */
	virtual void onNewMessage(CIMMessage* var1);

	// picture control callback
	virtual void changeShowStyle(string strUserId);
	virtual void closeCurrentLive();
public:
	virtual void getLocalSoundData(char* pData, int nLength);
	virtual void querySoundData(char** pData, int* nLength);
public:
	/**
		* msgServer处于在线状态
		*/
	virtual int online(int maxContentLen);

	/**
		* msgServer中断状态
		*/
	virtual int offline();
public:
	CStatic m_ShowArea;
	//CVoipDataShowView* m_pDataShowView;
	CUserManager* m_pUserManager;
	XHVoipManager* m_pVoipManager;
	CSoundManager* m_pSoundManager;
	bool m_bConnect;
	string m_strTargetId;
	bool m_bAudio;

	bool m_bShowSelfVideo;
	bool m_bShowOtherVideo;

	CRITICAL_SECTION m_critPicture;

	OnCallingCallback pOnCalling;
	OnAudioCallingCallback pOnAudioCalling;
	OnCancledCallback pOnCancled;
	OnRefusedCallback pOnRefused;
	OnBusyCallback pOnBusy;
	OnConnectedCallback pOnConnected;
	OnHangupCallback pOnHangup;
	OnErrorCallback pOnError;
	OnLineCallback pOnLine;

	OnGetVideoRawCallback pOnGetSelfVideoRaw;
	OnGetVideoRawCallback pOnGetOtherVideoRaw;

	/*
	 * 登陆
	 */
	bool login(string strLocalId);
	/**
	 * 主叫方调用
	 * 发起通话
	 * @param strTargetId 对方ID
	 */
	bool call(string strTargetId, bool showSelfVideo, bool showOtherVideo);
	/**
	 * 主叫方调用
	 * 发起语音通话
	 * @param strTargetId 对方ID
	 */
	bool callAudio(string strTargetId);
	/**
	 * 主叫方调用
	 * 对方接听或拒绝前 主叫方主动取消呼叫
	 */
	void cancel();
	/**
	 * 被叫方调用
	 * 同意跟主叫方通话
	 * @param fromID
	 */
	void accept(string fromID, bool showSelfVideo, bool showOtherVideo);
	/*
	 * 被叫方调用
	 * 同意跟主叫方语音通话
	 */
	void acceptAudio(string fromID);
	/**
	 * 被叫方调用
	 * 拒绝跟主叫方通话
	 */
	void refuse();
	/**
	 * 双方都可调用
	 * 挂断
	 */
	void hangup();
};