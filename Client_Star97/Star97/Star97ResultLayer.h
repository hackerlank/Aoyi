#ifndef _STAR97_RESULT_LAYER_H_
#define _STAR97_RESULT_LAYER_H_

#include "CMD_Game.h"
#include "Star97RollNode.h"
#include "LayerBase.h"

namespace GameStar97
{

class CStar97GameLayer;
class CResManager;
class CStar97RollNode;
class CLayerBase;

//��Ϸ״̬
enum enGameStatusInfo
{
	GSInfo_Bet = 0,	//�Ϸ�״̬
	GSInfo_Roll,	//����״̬
	GSInfo_GameOver,	//ʧ��
	GSInfo_YouWin,	//ʤ��
};

class CStar97ResultLayer : public CCLayer, public CLayerBase
{
public:
	static CStar97ResultLayer * create(CStar97GameLayer * pGameLayer, CResManager * pResManager);

	CStar97ResultLayer(CStar97GameLayer * pGameLayer, CResManager * pResManager);
	~CStar97ResultLayer();

	virtual bool init();

public:
	//�����Ϸַ���
	void setCreditScore(LONGLONG lScore);
	//���Ѻע
	void betScore(int nBetScore, int nLineBet);
	//������Ȼ�÷�
	void setNormalScore(LONGLONG lScore);
	//���òʽ�÷�
	void setHandselScore(LONGLONG lScore, BYTE cbHandselRatio);
	//ȡ��������Ч����
	int getStarCount()
	{
		return m_nStarCount;
	}

	//���ù������
	void setRollResult(BYTE cbResult[CELL_NUM], BYTE cbStarRatio);
	//���ø�ͼ���������
	void setRollCount(int nIndex, int nCount);
	//ֹͣ����ͼ��ת��
	void stopAllIconRoll();
	//ֹͣͼ�����
	void stopIconRoll(const bool bStop[CELL_NUM]);

	//������˸����ʾ
	void setFlashFrameVisible(bool bFlashFrameVisible[CELL_NUM]);
	//�����н�����ʾ
	void setLinesVisible(bool bLinesVisible[LINE_NUM]);
	//���õ�ǰ״̬��Ϣ
	void setStatusInfo(enGameStatusInfo eStatus);
	//���òʽ�ط���
	void setHandselPoolScore(LONGLONG lScore);
	//��λ�÷���Ϣ
	void resetScoreInfo();
	//�������Ǵ���
	void updateStarCount();
	//�����Զ���Ϸ��ʾ
	void setAutoTipsVisible(bool bVisible);

	//���������н�����
	void setStarWinScore(LONGLONG lWinScore, int nStarLevel, int nStarCount);
	//������ͨ�н�����
	void setNormalWinScore(const tagWinScore & winScore, int nCellScore, BYTE cbHandselRatio);

private:
	//ͼ�������ʱ��
	void rollSchedule(float dt);
	//���²ʽ�÷�,�ȸ��²ʽ�֣�����еĻ������ٸ�����ͨ�ķ֣������¶���÷�
	void handelScoreSchedule(float dt);
	//������Ȼ�÷�
	void normalScoreSchedule(float dt);
	//���¶���÷�
	void extraScoreSchedule(float dt);
	//�������Ƶ�ʷ���
	void computeUpdateRate(const LONGLONG lDestScore, LONGLONG & lRateScore);
	//���ö��⽱��ʾ����
	void setExtraFontScore(LONGLONG lScore);
	//���������Ӧÿλ����
	void computeScoreNum(LONGLONG lScore, int nNum[], int nCout);

private:
	CStar97GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	CStar97RollNode * m_pRollNode[CELL_NUM];	//����ͼ��
	CCSprite * m_pFlashFrame[CELL_NUM];	//��˸��
	CCSprite * m_pWinLine[LINE_NUM];	//�н���
	CCSprite * m_pAutoTips;	//�Զ�����ʾ
	CCSprite * m_pHaleSprite;	//1/2ͼ��
	CCSprite * m_pBonusSprite;	//bonusͼ��
	CCSprite * m_pExtraBg;	//���⽱������
	CCSprite * m_pStatusTips;	//��Ϸ״̬��ʾ

	//��������
	CCLabelAtlas * m_pFontHandselPool;	//�ʽ�ط���
	CCLabelAtlas * m_pFontExtraScore[5];	//����÷�
	CCLabelAtlas * m_pFontHandselScore;	//�ʽ�÷�
	CCLabelAtlas * m_pFontNormalScore;	//��Ȼ�÷�
	CCLabelAtlas * m_pFontCreditScore;	//�Ϸַ���
	CCLabelAtlas * m_pFontBetScore;	//��ע����
	CCLabelAtlas * m_pFontLineScore[LINE_NUM];	//ÿ����ע����
	CCLabelAtlas * m_pFontStarRatio;	//���Ǳ���
	CCLabelAtlas * m_pFontStarCount;	//���Ǵ���

	//��������
	LONGLONG m_lHandselPool;	//�ʽ�ط���
	LONGLONG m_lExtraScore;	//����÷�
	LONGLONG m_lHandselScore;	//�ʽ�÷�
	LONGLONG m_lNormalScore;	//��Ȼ�÷�
	LONGLONG m_lCreditScore;	//�Ϸַ���
	int m_nBetScore;	//Ѻע����
	LONGLONG m_lUpdateRate;	//����Ƶ�ʣ�ÿ�θ��¶��ٷ�
	int m_nStarLevel;	//���ǵȼ�
	int m_nStarCount;	//���Ǵ���

	//�������
	enRollType m_eRollType;	//��ǰ��������
	int m_nRollCount[CELL_NUM];	//��ͼ���������
	bool m_bStopRoll[CELL_NUM];	//�Ƿ�������Ӧͼ��ֹͣת��


};

}
#endif	//_STAR97_RESULT_LAYER_H_
