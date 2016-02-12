#ifndef _GAME_DAN_SHUANG_VIEW_H_
#define _GAME_DAN_SHUANG_VIEW_H_

#include "stdafx.h"
#include "ScoreRecordLayer.h"
#include "JettonMenuSprite.h"

#define JETTON_COUNT				4									//����
#define SHOW_APPLY_BANKER			5									// ׯ����Ŀ

/////////////////////////////////////////////
//��ׯ�б���Ϣ
struct tagApplyUser
{
	WORD							wChairID;								//������Ӻ�
	string							strUserName;							//�������
};

class CDanShuangGame;

class CDanShuangGameView : public CCLayer
{
	//��ע��Ϣ
private:
	LONGLONG						m_lMeScore[AREA_COUNT];					//����Լ�������ע
	LONGLONG						m_lAllScore[AREA_COUNT];				//�������������ע

	//λ����Ϣ
private:
	CCRect							m_rcAreaRange[AREA_COUNT];				//��ע�����η�Χ
	CCPoint							m_PointAreaMid[AREA_COUNT];				//��������λ��
	tagTriangle						m_RedAreaTriangle[3];					//���������������
	tagTriangle						m_BlueAreaTriangle[3];					//���������������

	//������Ϣ
private:
	enOpenCardType					m_enNowOpenType;						//��ǰ��������

	//��ʷ��Ϣ
private:
	LONGLONG						m_lMeStatisticScore;					//�����Ϸ�ɼ�ͳ��
	enOpenCardType					m_enHistoryRecord[MAX_HISTORY_RECORD];	//��ʷ��¼���
	BYTE							m_cbHistoryRecordCount;					//��ʷ��¼��Ŀ
	vector<tagApplyUser>			m_ApplyUserArray;						// ����ׯ�Ҷ���

	//״̬����
private:
	LONGLONG						m_lCurrentJetton;						//��ǰ����

	//ׯ����Ϣ
private:	
	WORD							m_wBankerUser;							//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	LONGLONG						m_lBankerScore;							//ׯ�һ���
	LONGLONG						m_lBankerWinScore;						//ׯ���ܳɼ�
	bool							m_bEnableSysBanker;						//�ɷ�ϵͳ��ׯ

	//���ֳɼ�
private:
	LONGLONG						m_lMeCurGameScore;						//�ҵĳɼ�
	LONGLONG						m_lMeCurGameReturnScore;				//�ҵĳɼ�
	LONGLONG						m_lBankerCurGameScore;					//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;							//��Ϸ˰��

	//������Ϣ
private:
	BYTE							m_cbGameTime;							//ʱ��
	CCSize							m_screenSize;							//��Ļ�ߴ�
	CCSprite						*m_pSpriteGameStatus;					//��Ϸ״̬
	CCLabelAtlas					*m_pLabelLeaveTime;						//ʣ��ʱ��

	//��Ϸͼ��
	CCLayer							*m_pLayerCoinAnimation;					//Ӳ�Ҷ���ͼ��
	CCSprite						*m_pSpriteHand;							//����������
	CCSprite						*m_pSpriteCoin;							//��������Ӳ��
	WORD							m_wCoinAnimateTime;						//Ӳ�Ҷ���ʱ��
	CCLayer							*m_pLayerFlashWinner;					//��ʤ������˸ͼ��
	CCLayer							*m_pLayerBankerList;					//��ׯ�б�ͼ��
	CCLayer							*m_pLayerBankerInfo;					//ׯ����Ϣͼ��
	CScoreRecordLayer				*m_pLayerScoreRecord;					//������¼ͼ��
	CCLayer							*m_pLayerHistoryRecord;					//��ʷ��¼ͼ��
	CCClippingNode					*m_pRecordClipNode;						//��¼�����ֲ�
	CCSpriteBatchNode				*m_pJettonBatchNode;					//�����þ��鼯����
	CCLayer							*m_pLayerQuit;							//�˳�ͼ��

	//�����Ϣ
	CCLabelAtlas					*m_pLabelUserScore;						//��ҷ���
	CCLabelTTF						*m_pLabelUserName;						//�������
	CCLabelAtlas					*m_pLabelUserMark;						//��ҳɼ�

	//������Ϣ
	CCLabelAtlas					*m_pLabelMeTableScore[AREA_COUNT];		//������ҷ���
	CCLabelAtlas					*m_pLabelAllTableScore[AREA_COUNT];		//����ȫ������
	CCSprite						*m_pTableScoreBK[2][AREA_COUNT];		//���ӷ�������
	CCLabelTTF						*m_pLabelMessage;						//��ʾ��Ϣ

	//ׯ����Ϣ
	CCLabelTTF						*m_pBankerUser;							//ׯ������
	CCLabelAtlas					*m_pBankerScore;						//ׯ�ҷ���
	CCLabelAtlas					*m_pBankerTime;							//ׯ����ׯ����
	CCLabelAtlas					*m_pBankerMark;							//ׯ�ҳɼ�

	//��ׯ�б�
	CCLabelTTF						*m_pLabelApplyBankerUser[SHOW_APPLY_BANKER];

	//�ǿ��ư�ť
private:
	CCMenuItemImage					*m_pBankerInfoButton;					//ׯ����Ϣ
	CCMenuItemImage					*m_pBankerList;							//��ׯ�б�
	CCMenuItemImage					*m_pQuit;								//�˳�
	CCMenuItemImage					*m_pScoreRecord;						//������¼

	//���ư�ť
public:
	CJettonMenuSprite				*m_pJetton0;							//����100
	CJettonMenuSprite				*m_pJetton1;							//����1000
	CJettonMenuSprite				*m_pJetton2;							//����10000
	CJettonMenuSprite				*m_pJetton3;							//����100000
	CCMenuItemImage					*m_pClearJetton;						//�����ע
	CCMenuItemImage					*m_pApplyBanker;						//������ׯ
	CCMenuItemImage					*m_pCancelBanker;						//ȡ����ׯ

	//�������
private:
	CDanShuangGame					*m_pDanShuangGame;						//��Ϸ�ӿ�
	CGameLogic						m_GameLogic;							//��Ϸ�߼�

	//������Ϸ�����þ��鼯����
private:
	class GameJettonBatchNode : public CCSpriteBatchNode
	{
	public:
		//��������
		static GameJettonBatchNode* create(const char* fileImage) {
			return GameJettonBatchNode::create(fileImage, kDefaultSpriteBatchCapacity);
		}
		static GameJettonBatchNode * create(const char *fileImage, unsigned int capacity);

		//ͨ��������ǣ��Ƴ��ӽڵ�,�����ͬ���ֵ�����нڵ�
		virtual void removeChildByTag(int tag, bool cleanup);
	};

public:
	CDanShuangGameView(CDanShuangGame *pDanShuangGame);
	~CDanShuangGameView(void);

	static CDanShuangGameView* create(CDanShuangGame *pDanShuangGame);
	//��ʼ��
	virtual bool init();
	void OnDisconnected(float dt);

	//����
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	//���̲���
	virtual void ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag);
	virtual void ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag){};

	//�������
private:
	//���п��Ҷ���
	void BeginOpenCoin();
	//�ױ��ӳ�
	void ThrowCoinDelay(float dt);
	//����Ӳ��λ��
	void UpdateCoinPostion(float dt);
	//��ʾ������Ϣ
	void ShowOpenCardInfo(float dt);
	//��ʾ�Լ�
	void DisplaySelf(CCNode *pNode);
	//��ʾ�����
	void ShowSettlement(CCNode *pNode);
	//����
	void DestorySelf(CCNode *pNode);

	//���Ƽ�¼�������
private:
	//���ƽ������
	void OpenTypeAnimation();
	//�ͷż�¼����
	void TypeRelSelf(CCNode * pNode);
	//�����Լ���ʶ
	void TypeCutSelfTag(CCNode * pNode);

	//��ť�ص�����
private:
	//����100
	void OnJettonButton100(CCObject *pSender);
	//����1000
	void OnJettonButton1000(CCObject *pSender);
	//����10000
	void OnJettonButton10000(CCObject *pSender);
	//����100000
	void OnJettonButton100000(CCObject *pSender);
	//���
	void OnClearJetton(CCObject *pSender);
	//��ׯ
	void OnApplyBanker(CCObject *pSender);
	//������¼
	void OnScoreRecord(CCObject *pSender);
	//ׯ����Ϣ
	void OnBankerInfo(CCObject *pSender);
	//��ׯ�б�
	void OnBankerList(CCObject *pSender);
	//ȡ����ׯ
	void OnCancelBanker(CCObject *pSender);
	//�ر�
	void OnClose(CCObject *pSender);
	//�˳�
	void OnQuit(CCObject *pSender);
	//ȷ���˳�
	void OnSubmitQuit(CCObject *pSender);
	//ȡ���˳�
	void OnCancelQuit(CCObject *pSender);

	//״̬��ʱ��
private:
	//����ʱ��
	void SetTime(int iTime, int iType);
	//����ʱ��
	void OnTimeFree(float dt);
	//����ʱ��
	void OnTimeAnimation(float dt);
	//��עʱ��
	void OnTimePlaceJetton(float dt);
	//����ʱ��
	void OnTimeEnd(float dt);

	//��Ϸ���ƽӿ�
public:
	//��ʤ������˸
	void SetWinnerSide(BYTE cbWinnerSide);
	//������г���
	void ClearAllJetton();
	//��������ע
	void ClearUserJetton(WORD wChairID, const LONGLONG lAllJettonScore[AREA_COUNT]);
	//���ó���
	void PlaceUserJetton(BYTE cbJettonArea, LONGLONG lJettonScore, WORD wChairID);
	//���ÿ������
	void SetCardInfo(enOpenCardType enCardType, bool bAnimation);
	//���óɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore,
		LONGLONG lGameRevenue);
	//����ׯ�ҷ���
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore);
	//����û�����ׯ�б�
	void AddApplyBanker(const tagApplyUser &applyUser);
	//ȡ�������ׯ
	void CancelApplyBanker(const tagApplyUser &applyUser);
	//���ȡ����ע
	void UserClearJetton(WORD wClearUser);
	//��ʾ��ʾ��Ϣ
	void ShowMessage(const char *Msg, bool bIsGB2312 = true);
	//������ʷ��¼
	void SetGameHistory(const BYTE cbOpenRecord[MAX_HISTORY_RECORD]);
	//��ʾ�л�ׯ��
	void ShowChangeBanker( bool bChangeBanker );

	//����ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//������Ϸ״̬ʱ��
	void SetGameTime(int iTime, int iType);
	//��ɿ��ƶ���
	void FinishOpenAnimation(bool bFlash);
	//ȡ�õ�ǰ״̬ʣ��ʱ��
	BYTE GetGameTime() {return m_cbGameTime;}
	//�����ױҶ���
	void BeginThrowCoin(bool bStart);

	//��Ϸ��ע�����
private:
	//��ȡ����
	BYTE GetJettonArea(CCPoint &ClickPoint);

	//����
public:
	//���Ʒ���
	void DrawUserScore();
private:
	//���Ƴɼ�
	void DrawUserMark();
	//�����������
	void DrawTableScore();
	//����ׯ����Ϣ
	void DrawBankerInfo();
	//������ׯ�б�
	void DrawBankerList();
};

#endif