#ifndef _GAME_DAN_SHUANG_H_
#define _GAME_DAN_SHUANG_H_

#include "stdafx.h"
#include "CMD_Game.h"
#include "GameLogic.h"

class CDanShuangGameView;

class CDanShuangGame : public CGameBase
{
	//限制信息
private:
	LONGLONG						m_lMeMaxScore;							//玩家最大下注
	LONGLONG						m_lAllMaxScore;							//可用最大总下注
	LONGLONG						m_lAreaLimitScore;						//区域限制
	LONGLONG						m_lApplyBankerCondition;				//申请条件

	//下注信息
private:
	LONGLONG						m_lMeScore[AREA_COUNT];					//玩家自己区域下注
	LONGLONG						m_lAllScore[AREA_COUNT];				//所有玩家区域下注

	//庄家信息
private:
	WORD							m_wCurrentBanker;						//当前庄家
	LONGLONG						m_lBankerScore;							//庄家积分
	bool							m_bEnableSysBanker;						//是否允许系统做庄

	//状态变量
private:
	bool							m_bMeApplyBanker;						//是否已申请坐庄
	bool							m_bMePlaceJetton;						//玩家是否下注

	//辅助变量
private:
	int								m_nGameStatus;							//游戏状态

private:
	CDanShuangGameView				*m_pDanShuangGameView;					//游戏视图
	CGameLogic						m_GameLogic;							//游戏逻辑

public:
	CGameDirector					*m_pDirector;							//导演类

public:
	CDanShuangGame(CGameDirector *pDirector);
	~CDanShuangGame();

	//通用接口
public:
	virtual bool Init();
	virtual void Uninit();
	virtual bool IsInGame();
	virtual bool EnterGame();
	virtual void LeaveGame();

	virtual void OnAudioChange() {}

	virtual void OnUserSit(unsigned short wChairID){}
	virtual void OnUserLeave(unsigned short wChairID){}

	virtual void OnTouchOption(){}
	virtual void OnTouchClose(){}

	virtual bool OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize);
	virtual bool OnGameMessage(WORD wMainCmd, WORD wSubCmd, const void * pBuffer, WORD wDataSize);
	virtual void OnGB2312Message(const char* sMsg, int nType);
	virtual void OnUTF8Message(const char* sMsg, int nType);
	virtual void OnMeScoreChange(LONGLONG lScore);
	virtual void OnMeInsureScoreChange(LONGLONG lInsureScore);

	//消息处理
private:
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//游戏抛币
	bool OnSubGameAnimation(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//取消下注
	bool OnSubClearJetton(const void * pBuffer, WORD wDataSize);

	//查看分数记录
	bool OnSubScoreRecord(const void * pBuffer, WORD wDataSize);

	//赋值函数
private:
	//设置庄家
	void SetBankerInfo(WORD wBanker,LONGLONG lScore);

	//功能函数
private:
	//更新控制
	void UpdateButtonControl(bool bEnablePlaceJetton = true);
	//取得区域最大下注
	LONGLONG GetMaxPlaceJetton(BYTE cbArea);

	//辅助功能
public:
	//取得游戏状态
	int GetGameStatus() { return m_nGameStatus; }

	//玩家操作
public:
	//加注消息
	bool OnPlaceJetton(BYTE cbJettonArea, LONGLONG lJettonScore);
	//取消下注
	bool OnClearJetton();
	//申请坐庄
	bool OnApplyBanker();
	//取消坐庄
	bool OnCancelBanker();
};

#endif