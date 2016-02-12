#ifndef _PullMach_RESULT_LAYER_H_
#define _PullMach_RESULT_LAYER_H_

#include "CMD_Game.h"

namespace GamePullMach777
{

class CPullMachGameLayer;
class CPMResManager;
class CPullMachRollNode;
class CPullMachBgSceneryNode;
class CGraduallyShowSprite;
class CContinueUpdateLabelAtlas;
class CPullMachPrizeRemainFontNode;

class CPullMachResultLayer : public CCLayer
{
public:
	static CPullMachResultLayer * create(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);

	CPullMachResultLayer(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);
	~CPullMachResultLayer();

	virtual bool init();

public:
	//���������Ϣ
	void setUserInfo(const tagLocalUser * pUserData);
	//���ù������
	void setRollResult(BYTE cbResult[CELL_NUM]);
	//�����н���
	void setWinLineId(int nLineId);
	//���ø�ͼ���������
	void setRollCount(int nIndex, int nCount);
	//ֹͣ����ͼ��ת��
	void stopAllIconRoll();
	//ֹͣ���ֹ���,�����ң�0��1��2
	void stopIconRoll(int nIndex);

	//����Ϊ��ģʽ
	void setBigPrizeMode(bool bPrizeMode);
	//����logo��Ŀ
	bool setLogoCount(const int nLogoCount[VER_LINE_NUM]);

	//������ҷ���
	void setUserScore(LONGLONG lScore);
	//�����������Ӯ
	void setAllWinLoseScore(LONGLONG lScore);
	//�����Ϸַ���
	void setCreditScore(LONGLONG lScore, bool bDirectShow);
	//���Ѻע
	void setBetScore(int nBetScore);
	//�������Ӯ��
	void setWinScore(LONGLONG lScore, bool bDirectShow);
	//���ô�ʣ�����
	void setBigPrizeRemainScore(LONGLONG lRemainScore, bool bDirectShow);
	 
	//������������:�󽱷�������������
	void openGivePrizeAni(LONGLONG lPrizeScore, int nPrizeTimes);
	//�رշ�������
	void closeGivePrizeAni();
	bool isOpenGivePrizeAni() { return m_bOpenGivePrizeAni;}
	//������7����
	void openBlue7Ani(bool bOpen);
	//������7����
	void openRed7Ani(bool bOpen);
	//����logo����
	void openLogoAni(bool bOpen);
	//�������䶯��
	void openBoxPrizeAni(bool bOpen);

private:
	//ͼ�������ʱ��
	void rollSchedule(float dt);
	//��7�߿����˸��ʱ��
	void flashBlue7FrameSchedule(float dt);

private:
	CPullMachGameLayer * m_pGameLayer;
	CPMResManager * m_pResManager;

	CPullMachRollNode * m_pRollNode[VER_LINE_NUM];	//����ͼ��
	CPullMachBgSceneryNode * m_pBgScenery;	//����ͼƬ
	CGraduallyShowSprite * m_pRemainPrizeTip;	//ʣ�����ʾ
	CCSprite * m_pRollBgLight;	//������������
	CCSprite * m_pBetCoin[3];	//��ע���

	CCSprite * m_pPrizeTimesTip;	//�󽱱�����ʾ
	CCSprite * m_pBlueLightFrame;	//����߿�
	CCSprite * m_pTreasureBox;	//����
	CCSprite * m_pFlyGirl;	//����Ů��
	CCSprite * m_pScroll;	//���ᱳ��

	//��������
	CCLabelAtlas * m_pFontUserScore;	//��ҷ���
	CCLabelAtlas * m_pFontAllWinLose;	//����Ӯ
	CContinueUpdateLabelAtlas * m_pFontCreditScore;	//�Ϸַ���
	CContinueUpdateLabelAtlas * m_pFontWinScore;	//Ӯ�÷���
	CPullMachPrizeRemainFontNode * m_pPrizeRemainScore;	//��ʣ�����
	CCLabelAtlas * m_pFontPrizeTimes;	//�󽱱���
	CCLabelAtlas * m_pFontLogoNum[VER_LINE_NUM];	//����logo��

	//�����Ϣ
	CCSprite * m_pUserFace;	//���ͷ��
	CCPoint m_FacePos;	//ͷ��λ��
	CCLabelTTF * m_pUserName;	//�������
	CCLabelTTF * m_pUserID;	//���ID

	//��Ϸ����
	bool m_bBigPrizeMode;	//�Ƿ��ģʽ
	bool m_bOpenGivePrizeAni;	//�Ƿ�����������
	int m_nLogoCount[VER_LINE_NUM];	//��ת���ѿ���logo��Ŀ
	int m_nRollCount[VER_LINE_NUM];	//��ת�ֹ�������
	bool m_bStopRoll[VER_LINE_NUM];	//�Ƿ�������Ӧת��ֹͣת��
	int m_nLightFrameIndex;	//��ǰ����������
};

}
#endif	//_PullMach_RESULT_LAYER_H_
