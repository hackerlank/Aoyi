#ifndef _STAR97_GAME_LAYER_H_
#define _STAR97_GAME_LAYER_H_

#include "CMD_Game.h"

namespace GameStar97
{

class CResManager;
class CStar97Game;
class CStar97ResultLayer;
class CStar97UILayer;
class CStar97PrizeLayer;
class CStar97SettingLayer;

class CStar97GameLayer : public CGameLayer
{
public:
	CStar97GameLayer(CStar97Game * pGame, CResManager * pResManager);
	virtual ~CStar97GameLayer();

	//重载函数
	virtual void OnInit();
	virtual void Clear();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

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
	bool OnSubBigprizeRecord(const CMD_S_BigPrizeRecord * pBigPrizeRecord);

	//功能函数
public:
	void PlaySound(int nResId);
	void PlayMusic(int nResId, bool bLoop);

public:
	bool IsInit() { return m_bInit;}
	CStar97Game * getGame()
	{
		return m_pGame;
	}
	bool IsLookon();
	void SetGameStatus(int cbGameStatus);
	int GetGameStatus() { return m_nGameStatus; }
	LONGLONG GetUserScore();
	BYTE GetUserStatus(WORD wChairID);
	void updateUserScore(LONGLONG lScore);

	//处理按钮消息
	void touchAutoStart();
	void touchCancelAuto();
	void touchCreditScore();
	void touchStart();
	void touchBetScore();
	void touchLookPrize();
	void touchIconStop(const bool bStop[CELL_NUM]);
	void touchExit();
	void touchSetting();

	//所有图标停止转动
	void allIconStopRoll();
	//显示得分动画
	void showScoreInfoSchedule(float dt);
	//游戏结果处理完毕
	void resultSetOver();
	//判断闪烁框是否显示
	void checkFlashFrameShow(const BYTE cbResult[CELL_NUM], bool bShow[CELL_NUM]);

	//自动开始定时器
	void autoStartSchedule(float dt);
	//全部停止滚动定时器
	void allStopRollSchedule(float dt);
	//滚动音效定时器
	void rollIconSoundSchedule(float dt);
	//得分音效定时器
	void getScoreSoundSchedule(float dt);
	//停止滚动音效
	void stopRollSound();
	//押分时，是否处在明星局
	bool isBetStarUseful();

private:
	CStar97Game * m_pGame;
	CResManager * m_pResManager;

	CStar97ResultLayer * m_pResultLayer;	//结果层
	CStar97UILayer * m_pUILayer;	//UI层
	CStar97PrizeLayer * m_pPrizeLayer;	//大奖层
	CStar97SettingLayer * m_pSettingLayer;	//设置图层

	//游戏变量
	bool m_bInit;	//是否已初始化
	bool m_bEnableSound;	//是否需要声音
	int m_nCellScore;	//单元积分
	LONGLONG m_lCreditScore;	//上分
	int m_nBetScore;	//押注
	tagWinScore m_winScore;	//当局赢分
	BYTE m_cbResult[CELL_NUM];	//开奖结果
	bool m_bOddsLine[LINE_NUM];	//中线情况
	BYTE m_cbStarRatio;	//明星倍数
	BYTE m_cbHandselRatio;	//彩金倍数
	LONGLONG m_lHandselPoolScore;	//彩金池分数

	int m_nGameStatus;	//游戏状态
	bool m_bAutoPlay;	//是否自动玩
	int m_nBetLevel;	//押注等级
	bool m_bTouchBegin;	//是否已点击开始
};

}

#endif	//_STAR97_GAME_LAYER_H_
