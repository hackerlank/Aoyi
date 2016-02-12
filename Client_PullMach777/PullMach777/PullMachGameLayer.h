#ifndef _PullMach_GAME_LAYER_H_
#define _PullMach_GAME_LAYER_H_

#include "CMD_Game.h"
#include "PullMachGame.h"

namespace GamePullMach777
{

class CPMResManager;
class CPullMachGame;
class CPullMachResultLayer;
class CPullMachUILayer;
class CPullMachHelpLayer;

class CPullMachGameLayer : public CGameLayerWithLayout
{
public:
	CPullMachGameLayer(CPullMachGame * pGame, CPMResManager * pResManager);
	virtual ~CPullMachGameLayer();

	//重载函数
	virtual void OnInit();
	virtual void Clear();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	//消息处理
public:
	bool OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize);
	bool OnGameMessage(WORD wSubCmd, const void * pBuffer, WORD wDataSize);
	void SendUserReady();
	void SendGameMessage(WORD wSubCmd, const void* pData, WORD wDataSize);

private:
	bool OnEventGameSceneFree(bool bLookon, const CMD_S_StatusFree * pStatusFree);
	bool OnEventGameScenePlay(bool bLookOn, const CMD_S_StatusPlay * pStatusPlay);

	bool OnSubGameStart(const CMD_S_GameStart * pGameStart);
	bool OnSubBeginRoll(const CMD_S_BeginGame * pBeginGame);
	bool OnSubGameEnd(const CMD_S_GameEnd * pGameEnd);

	//功能函数
public:
	void PlaySound(enSoundResDef nResId);
	void PlayMusic(enSoundResDef nResId, bool bLoop);

public:
	CPullMachGame * getGame() { return m_pGame;}
	CGameDirector* GetGameDirector() { return m_pGame->GetGameDirector(); }

	bool IsLookon();
	bool IsBigPrizeMode() { return m_bIsBigPrizeMode;}

	void SetGameStatus(int cbGameStatus);
	int GetGameStatus() { return m_nGameStatus; }
	BYTE GetUserStatus(WORD wChairID);

	LONGLONG GetUserScore();
	void updateUserScore(LONGLONG lScore);
	int getCellScore() { return m_nCellScore;}

	//处理按钮消息
	void touchStart();
	void touchAutoStart();
	void touchCancelAuto();
	void touchCreditScore();
	void touchBetScore();
	void touchStopLeft();
	void touchStopMid();
	void touchStopRight();
	void touchHelp();
	void touchExit();

	//显示聊天
	void ShowChatView(eChatDirection eDirection);
	//领取宝箱
	void ShowBox();
	//所有图标停止转动
	void allIconStopRoll();
	//游戏结果处理完毕定时器
	void resultSetOverSchedule(float dt);
	//自动开始定时器
	void autoStartSchedule(float dt);
	//全部停止滚动定时器
	void allStopRollSchedule(float dt);
	//滚动音效定时器
	void rollIconSoundSchedule(float dt);

private:
	//加载游戏动画缓存
	void addGameAnimationCatch();
	//通过精灵帧创建动画
	CCAnimation * createAniBySpriteFrame(int nBeginFrameId, int nFrameCount, float fDelay, unsigned int nLoops = 1);
	//创建带有反转的动画
	CCAnimation * createAniBySpriteFrameWithSwing(int nBeginFrameId, int nFrameCount, float fDelay, unsigned int nLoops = 1);

public:
	CChatViewLayer*		m_pChatViewLayer;
	CBoxLayer*			m_pBoxLayer;
	CGamePrizeLayer*	m_pGamePrizeLayer;

private:
	CPullMachGame * m_pGame;
	CPMResManager * m_pResManager;

	CPullMachResultLayer * m_pResultLayer;	//结果层
	CPullMachUILayer * m_pUILayer;	//UI层
	CPullMachHelpLayer * m_pHelpLayer;	//帮助层

	//游戏变量
	bool m_bEnableSound;	//是否需要声音
	int m_nGameStatus;	//游戏状态
	int m_nCellScore;	//单元积分
	LONGLONG m_lCreditScore;	//上分
	int m_nBetScore;	//押注
	LONGLONG m_lTotalPoints;	//当局得分
	BYTE m_cbResult[CELL_NUM];	//开奖结果
	BYTE m_cbWinLineId;	//中奖线ID
	BYTE m_cbWinType;	//中奖类型
	BYTE m_cbNextType;	//下局类型
	bool m_bIsBigPrizeMode;	//是否大奖模式
	LONGLONG m_lRemainBigPrizeScore;	//剩余大奖得分
	int m_nPrizeTimes;	//大奖倍数
	int m_nVerLogoCount[VER_LINE_NUM];	//竖行logo数目

	bool m_bAutoPlay;	//是否自动玩
	bool m_bTouchBegin;	//是否已点击开始
	bool m_bSendEnd;	//是否已发送结束消息
};

}

#endif	//_PullMach_GAME_LAYER_H_
