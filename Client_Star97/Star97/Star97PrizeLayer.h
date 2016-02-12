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
	//更新大奖次数
	void updatePrizeCount(const tagBigPrizeCount & prizeCount);
	//更新总玩、总赢
	void updateAllJettonWin(LONGLONG lJetton, LONGLONG lWin);
	//设置额外奖励分数信息
	void setExtraScoreInfo(const tagExtraScoreInfo & extraInfo);

private:
	CResManager * m_pResManager;

	CCSprite * m_pPrize;	//大奖背景

	//分数显示
	CCLabelAtlas * m_pFontAnyBar;	//全杂bar
	CCLabelAtlas * m_pFontOrange;	//全橙子
	CCLabelAtlas * m_pFontPawpaw;	//全木瓜
	CCLabelAtlas * m_pFontBell;	//全铃铛
	CCLabelAtlas * m_pFontTsama;	//全西瓜
	CCLabelAtlas * m_pFontBarY;	//全黄bar
	CCLabelAtlas * m_pFontLeech;	//全荔枝
	CCLabelAtlas * m_pFontBarR;	//全红bar
	CCLabelAtlas * m_pFontBarB;	//全蓝bar
	CCLabelAtlas * m_pFont9Num7;	//9个7
	CCLabelAtlas * m_pFont8Num7;	//8个7
	CCLabelAtlas * m_pFont7Num7;	//7个7

	CCLabelAtlas * m_pFontExtraAllHalf;	//后外围半炸额外得分
	CCLabelAtlas * m_pFontExtraAllWhole;	//后外围整炸额外得分
	CCLabelAtlas * m_pFontExtra9Num7;	//9个7额外得分
	CCLabelAtlas * m_pFontExtra8Num7;	//8个7额外得分
	CCLabelAtlas * m_pFontExtra7Num7;	//7个7额外得分
	CCLabelAtlas * m_pFontExtraAnyBar;	//全杂bar额外得分

	CCLabelAtlas * m_pFontAllJetton;	//总下注
	CCLabelAtlas * m_pFontAllWin;	//总赢分
};

}

#endif	//_STAR97_PRIZE_LAYER_H_