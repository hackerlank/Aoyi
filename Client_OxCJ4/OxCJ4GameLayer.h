#ifndef _OX_CJ4_GAME_LAYER_
#define _OX_CJ4_GAME_LAYER_

#include "GameLogic.h"
#include "CommonDef.h"

namespace GameOxCJ4
{

class CResManager;
class COxCJ4UserInfo;
class COxCJ4UILayer;
class COxCJ4JettonControl;
class COxCJ4CardHeap;
class COxCJ4TouchCard;
class COxCJ4CaiJinLayer;
class COxCJ4QuitConfirm;

enum ENumTableUser
{
	NUM_TABLE_USER=4,
};
enum EMeUserIndex
{
	ME_USER_INDEX=0,
};

enum enTimeStatus
{
	Time_Status_Begin = 0,
	Time_Status_Call,
	Time_Status_Jetton,
	Time_Status_Show,
};

class COxCJ4GameLayer : public CGameLayer
{
public:
	COxCJ4GameLayer(CResManager * pResManager);
	virtual ~COxCJ4GameLayer();

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
	bool OnEventGameSceneCall(bool bLookon, const CMD_S_StatusCall * pStatusCall);
	bool OnEventGameSceneScore(bool bLookOn, const CMD_S_StatusScore * pStatusScore);
	bool OnEventGameScenePlay(bool bLookOn, const CMD_S_StatusPlay * pStatusPlay);

	void OnSubGameFree(float dt);
	bool OnSubCallBanker(const CMD_S_CallBanker * pCallBanker);
	bool OnSubGameStart(const CMD_S_GameStart * pGameStart);
	bool OnSubAddScore(const CMD_S_AddScore * pAddScore);
	bool OnSubSendCard(const CMD_S_SendCard * pSendCard);
	bool OnSubOpenCard(const CMD_S_Open_Card * pOpenCard);
	bool OnSubGameEnd(const CMD_S_GameEnd * pGameEnd);
	bool OnSubWinnings(const CMD_S_Winnings * pWinnings);
	bool OnSubWinWeek(const DBR_GR_AddWinningsResult * pWinWeek);

	//功能函数
public:
	bool IsInit() { return m_bInit;}
	void ShowGameMessage(const char* sMsg);
	void OnUserScoreChange(WORD wChairID, LONGLONG lUserScore);
	void OnUserSit(WORD wChairID, const tagLocalUser* pLocalUser);
	void OnUserStateChange(WORD wChairID, int nUserState, bool bLookup);
	void OnUserLeave(WORD wChairID);
	bool IsLookon();

	void SetGameStatus(int cbGameStatus);
	int GetGameStatus() { return m_nGameStatus; }
	WORD GetMeChairID() { return m_pGameDirector->m_nMeChairID;}
	WORD GetUserChairID(WORD wChairID);
	WORD GetUserTableID(WORD wChairID);
	BYTE GetUserStatus(WORD wChairID);

	CGameDirector* GetGameDirector() { return m_pGameDirector; }
	void MePlaceJetton(LONGLONG lScore);
	void SortHandCard();
	bool ShowMyCard(bool bIsSelfShow);
	void TeachMeOut();
	void AutoPlay(bool bCanAuto);
	void QuitGame();
	void GetAllUserJetIconPos(CCPoint jettonPos[GAME_PLAYER]);
	BYTE UpdateOxType();
	void PlaySound(int nResId);
	void PlayMusic(int nResId, bool bLoop);
	void PlayOxSound(CCNode * pTarget, void * data);

	//辅助函数
private:
	void UpdateUserInfo(WORD wChairID = INVALID_CHAIR);
	LONGLONG ComputeUnitValue(LONGLONG lCellScore, bool bIsRealCell, int nMaxMulti, WORD nUserCount);
	void SetCanUseJetton(LONGLONG lMaxScore, LONGLONG lCellScore);

	void SetGameTime(int nType, int nTime);
	void SetClockTime(int nTime);
	void OnTimeBegin(float dt);
	void OnTimeCallBanker(float dt);
	void OnTimeJetton(float dt);
	void OnTimeOpenCard(float dt);

	void SetCardTouchCanUse(float dt);
	void ShowAllUserCard();
	void PlaceJettonChangeStep(float dt);
	void BankerJettonChangeStep(float dt);
	void ShowAllUserGetScore(float dt);
	void PlaySendSound(float dt);

	int GetOxTypeResID(BYTE cbOxType);
	int GetOxSoundResID(BYTE cbOxType, BYTE cbUserGender);
private:
	template<class T> void RandValue(T data[], int nCount);

private:
	CResManager * m_pResManager;
	COxCJ4UserInfo * m_pUserInfo;
	COxCJ4UILayer * m_pUILayer;
	COxCJ4JettonControl * m_pJettonLayer;
	CCLayer * m_pCardLayer;
	CCLayer * m_pTipsLayer;
	COxCJ4CaiJinLayer * m_pCaiJinLayer;
	COxCJ4QuitConfirm * m_pConfirmLayer;

	CCSpriteBatchNode * m_pBackHeapBatch[GAME_PLAYER];
	COxCJ4CardHeap * m_pOxCardHeap[4];
	COxCJ4TouchCard * m_pTouchCard[MAX_COUNT];

	CCSprite * m_pReadySprite[4];
	CCSprite * m_pCallingBankerSprite;
	CCSprite * m_pOverShowSprite[4];
	CCSprite * m_pClockSprite;
	CCLabelAtlas * m_pClockLabel;
	CCSprite * m_pCardTypeSprite[4];

	bool m_bInit;
	bool m_bEnableSound;
	int m_nGameStatus;
	CCPoint m_JettonIconPos[4];
	CCPoint m_CallingPos[4];
	CCPoint m_OxTypePos[4];
	CCPoint m_GetScorePos[4];

	bool m_bAutoPlay;
	bool m_bCanBegin;
	bool m_bHaveJetton;
	bool m_bHaveShow;
	BYTE m_cbPlayStatus[GAME_PLAYER];
	WORD m_wCurCallUser;
	WORD m_wCurrentBanker;
	int m_nGameTime;

	LONGLONG m_lUserJetton[GAME_PLAYER];
	LONGLONG m_lCellScore;
	LONGLONG m_lTurnMaxScore;
	LONGLONG m_lAutoJettonScore;
	LONGLONG m_lJetUnitValue;	//1金币对应单位数值

	BYTE m_cbHandCard[MAX_COUNT];
	BYTE m_cbAllUserCard[GAME_PLAYER][MAX_COUNT];
	BYTE m_cbOxType[GAME_PLAYER];
	LONGLONG m_lGetScore[GAME_PLAYER];
	bool m_bWinnings[GAME_PLAYER];
	std::string m_szWinsName[GAME_PLAYER];

	CGameLogic m_GameLogic;

};

}

#endif	//_OX_CJ4_GAME_LAYER_

