#ifndef _GAME_DAN_SHUANG_H_
#define _GAME_DAN_SHUANG_H_

#include "stdafx.h"
#include "CMD_Game.h"
#include "GameLogic.h"

class CDanShuangGameView;

class CDanShuangGame : public CGameBase
{
	//������Ϣ
private:
	LONGLONG						m_lMeMaxScore;							//��������ע
	LONGLONG						m_lAllMaxScore;							//�����������ע
	LONGLONG						m_lAreaLimitScore;						//��������
	LONGLONG						m_lApplyBankerCondition;				//��������

	//��ע��Ϣ
private:
	LONGLONG						m_lMeScore[AREA_COUNT];					//����Լ�������ע
	LONGLONG						m_lAllScore[AREA_COUNT];				//�������������ע

	//ׯ����Ϣ
private:
	WORD							m_wCurrentBanker;						//��ǰׯ��
	LONGLONG						m_lBankerScore;							//ׯ�һ���
	bool							m_bEnableSysBanker;						//�Ƿ�����ϵͳ��ׯ

	//״̬����
private:
	bool							m_bMeApplyBanker;						//�Ƿ���������ׯ
	bool							m_bMePlaceJetton;						//����Ƿ���ע

	//��������
private:
	int								m_nGameStatus;							//��Ϸ״̬

private:
	CDanShuangGameView				*m_pDanShuangGameView;					//��Ϸ��ͼ
	CGameLogic						m_GameLogic;							//��Ϸ�߼�

public:
	CGameDirector					*m_pDirector;							//������

public:
	CDanShuangGame(CGameDirector *pDirector);
	~CDanShuangGame();

	//ͨ�ýӿ�
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

	//��Ϣ����
private:
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ�ױ�
	bool OnSubGameAnimation(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ע
	bool OnSubClearJetton(const void * pBuffer, WORD wDataSize);

	//�鿴������¼
	bool OnSubScoreRecord(const void * pBuffer, WORD wDataSize);

	//��ֵ����
private:
	//����ׯ��
	void SetBankerInfo(WORD wBanker,LONGLONG lScore);

	//���ܺ���
private:
	//���¿���
	void UpdateButtonControl(bool bEnablePlaceJetton = true);
	//ȡ�����������ע
	LONGLONG GetMaxPlaceJetton(BYTE cbArea);

	//��������
public:
	//ȡ����Ϸ״̬
	int GetGameStatus() { return m_nGameStatus; }

	//��Ҳ���
public:
	//��ע��Ϣ
	bool OnPlaceJetton(BYTE cbJettonArea, LONGLONG lJettonScore);
	//ȡ����ע
	bool OnClearJetton();
	//������ׯ
	bool OnApplyBanker();
	//ȡ����ׯ
	bool OnCancelBanker();
};

#endif