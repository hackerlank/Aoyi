#ifndef _STAR97_PRIZE_LAYER_H_
#define _STAR97_PRIZE_LAYER_H_

#include "CMD_Game.h"

namespace GameStar97
{

class CResManager;

class CStar97PrizeLayer : public CCLayer
{
public:
	static CStar97PrizeLayer * create(CResManager * pResManager);

	CStar97PrizeLayer(CResManager * pResManager);
	~CStar97PrizeLayer();

	virtual bool init();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

public:
	//���´󽱴���
	void updatePrizeCount(const tagBigPrizeCount & prizeCount);
	//�������桢��Ӯ
	void updateAllJettonWin(LONGLONG lJetton, LONGLONG lWin);
	//���ö��⽱��������Ϣ
	void setExtraScoreInfo(const tagExtraScoreInfo & extraInfo);

private:
	CResManager * m_pResManager;

	CCSprite * m_pPrize;	//�󽱱���

	//������ʾ
	CCLabelAtlas * m_pFontAnyBar;	//ȫ��bar
	CCLabelAtlas * m_pFontOrange;	//ȫ����
	CCLabelAtlas * m_pFontPawpaw;	//ȫľ��
	CCLabelAtlas * m_pFontBell;	//ȫ����
	CCLabelAtlas * m_pFontTsama;	//ȫ����
	CCLabelAtlas * m_pFontBarY;	//ȫ��bar
	CCLabelAtlas * m_pFontLeech;	//ȫ��֦
	CCLabelAtlas * m_pFontBarR;	//ȫ��bar
	CCLabelAtlas * m_pFontBarB;	//ȫ��bar
	CCLabelAtlas * m_pFont9Num7;	//9��7
	CCLabelAtlas * m_pFont8Num7;	//8��7
	CCLabelAtlas * m_pFont7Num7;	//7��7

	CCLabelAtlas * m_pFontExtraAllHalf;	//����Χ��ը����÷�
	CCLabelAtlas * m_pFontExtraAllWhole;	//����Χ��ը����÷�
	CCLabelAtlas * m_pFontExtra9Num7;	//9��7����÷�
	CCLabelAtlas * m_pFontExtra8Num7;	//8��7����÷�
	CCLabelAtlas * m_pFontExtra7Num7;	//7��7����÷�
	CCLabelAtlas * m_pFontExtraAnyBar;	//ȫ��bar����÷�

	CCLabelAtlas * m_pFontAllJetton;	//����ע
	CCLabelAtlas * m_pFontAllWin;	//��Ӯ��
};

}

#endif	//_STAR97_PRIZE_LAYER_H_