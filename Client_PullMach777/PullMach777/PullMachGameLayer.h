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

	//���غ���
	virtual void OnInit();
	virtual void Clear();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	//��Ϣ����
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

	//���ܺ���
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

	//����ť��Ϣ
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

	//��ʾ����
	void ShowChatView(eChatDirection eDirection);
	//��ȡ����
	void ShowBox();
	//����ͼ��ֹͣת��
	void allIconStopRoll();
	//��Ϸ���������϶�ʱ��
	void resultSetOverSchedule(float dt);
	//�Զ���ʼ��ʱ��
	void autoStartSchedule(float dt);
	//ȫ��ֹͣ������ʱ��
	void allStopRollSchedule(float dt);
	//������Ч��ʱ��
	void rollIconSoundSchedule(float dt);

private:
	//������Ϸ��������
	void addGameAnimationCatch();
	//ͨ������֡��������
	CCAnimation * createAniBySpriteFrame(int nBeginFrameId, int nFrameCount, float fDelay, unsigned int nLoops = 1);
	//�������з�ת�Ķ���
	CCAnimation * createAniBySpriteFrameWithSwing(int nBeginFrameId, int nFrameCount, float fDelay, unsigned int nLoops = 1);

public:
	CChatViewLayer*		m_pChatViewLayer;
	CBoxLayer*			m_pBoxLayer;
	CGamePrizeLayer*	m_pGamePrizeLayer;

private:
	CPullMachGame * m_pGame;
	CPMResManager * m_pResManager;

	CPullMachResultLayer * m_pResultLayer;	//�����
	CPullMachUILayer * m_pUILayer;	//UI��
	CPullMachHelpLayer * m_pHelpLayer;	//������

	//��Ϸ����
	bool m_bEnableSound;	//�Ƿ���Ҫ����
	int m_nGameStatus;	//��Ϸ״̬
	int m_nCellScore;	//��Ԫ����
	LONGLONG m_lCreditScore;	//�Ϸ�
	int m_nBetScore;	//Ѻע
	LONGLONG m_lTotalPoints;	//���ֵ÷�
	BYTE m_cbResult[CELL_NUM];	//�������
	BYTE m_cbWinLineId;	//�н���ID
	BYTE m_cbWinType;	//�н�����
	BYTE m_cbNextType;	//�¾�����
	bool m_bIsBigPrizeMode;	//�Ƿ��ģʽ
	LONGLONG m_lRemainBigPrizeScore;	//ʣ��󽱵÷�
	int m_nPrizeTimes;	//�󽱱���
	int m_nVerLogoCount[VER_LINE_NUM];	//����logo��Ŀ

	bool m_bAutoPlay;	//�Ƿ��Զ���
	bool m_bTouchBegin;	//�Ƿ��ѵ����ʼ
	bool m_bSendEnd;	//�Ƿ��ѷ��ͽ�����Ϣ
};

}

#endif	//_PullMach_GAME_LAYER_H_
