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

	//���غ���
	virtual void OnInit();
	virtual void Clear();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

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
	bool OnSubBigprizeRecord(const CMD_S_BigPrizeRecord * pBigPrizeRecord);

	//���ܺ���
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

	//����ť��Ϣ
	void touchAutoStart();
	void touchCancelAuto();
	void touchCreditScore();
	void touchStart();
	void touchBetScore();
	void touchLookPrize();
	void touchIconStop(const bool bStop[CELL_NUM]);
	void touchExit();
	void touchSetting();

	//����ͼ��ֹͣת��
	void allIconStopRoll();
	//��ʾ�÷ֶ���
	void showScoreInfoSchedule(float dt);
	//��Ϸ����������
	void resultSetOver();
	//�ж���˸���Ƿ���ʾ
	void checkFlashFrameShow(const BYTE cbResult[CELL_NUM], bool bShow[CELL_NUM]);

	//�Զ���ʼ��ʱ��
	void autoStartSchedule(float dt);
	//ȫ��ֹͣ������ʱ��
	void allStopRollSchedule(float dt);
	//������Ч��ʱ��
	void rollIconSoundSchedule(float dt);
	//�÷���Ч��ʱ��
	void getScoreSoundSchedule(float dt);
	//ֹͣ������Ч
	void stopRollSound();
	//Ѻ��ʱ���Ƿ������Ǿ�
	bool isBetStarUseful();

private:
	CStar97Game * m_pGame;
	CResManager * m_pResManager;

	CStar97ResultLayer * m_pResultLayer;	//�����
	CStar97UILayer * m_pUILayer;	//UI��
	CStar97PrizeLayer * m_pPrizeLayer;	//�󽱲�
	CStar97SettingLayer * m_pSettingLayer;	//����ͼ��

	//��Ϸ����
	bool m_bInit;	//�Ƿ��ѳ�ʼ��
	bool m_bEnableSound;	//�Ƿ���Ҫ����
	int m_nCellScore;	//��Ԫ����
	LONGLONG m_lCreditScore;	//�Ϸ�
	int m_nBetScore;	//Ѻע
	tagWinScore m_winScore;	//����Ӯ��
	BYTE m_cbResult[CELL_NUM];	//�������
	bool m_bOddsLine[LINE_NUM];	//�������
	BYTE m_cbStarRatio;	//���Ǳ���
	BYTE m_cbHandselRatio;	//�ʽ���
	LONGLONG m_lHandselPoolScore;	//�ʽ�ط���

	int m_nGameStatus;	//��Ϸ״̬
	bool m_bAutoPlay;	//�Ƿ��Զ���
	int m_nBetLevel;	//Ѻע�ȼ�
	bool m_bTouchBegin;	//�Ƿ��ѵ����ʼ
};

}

#endif	//_STAR97_GAME_LAYER_H_
