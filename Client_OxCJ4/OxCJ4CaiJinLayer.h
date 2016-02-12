#ifndef _OX_CJ4_CAI_JIN_LAYER_H_
#define _OX_CJ4_CAI_JIN_LAYER_H_

#include "LayerBase.h"
#include "OxCJ4GameLayer.h"
#include "ResManager.h"

namespace GameOxCJ4
{

class COxCJ4CaiJinLayer : public CLayerBase
{
public:
	static COxCJ4CaiJinLayer * create(COxCJ4GameLayer * pGameLayer, CResManager * pResManger);

	COxCJ4CaiJinLayer(COxCJ4GameLayer * pGameLayer, CResManager * pResManger);
	~COxCJ4CaiJinLayer();

	virtual bool init();
	void SetCaiJinChi(const CMD_S_Winnings * pWinnings);
	void SetMeWeekWin(LONGLONG lScore);
	void OnTouchXiangQing(CCObject* pObject);
	void OnTouchJiLu(CCObject* pObject);

private:
	COxCJ4GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	CCSprite * m_pCaiJinChi;
	CCSprite * m_pJiLu;
	CCSprite * m_pXiangQing;
	CCLabelAtlas * m_pWinningsPool;
	CCLabelAtlas * m_pMeWeekWin;
	CCLabelTTF * m_pName[10];
	CCLabelTTF * m_pCardType[10];
	CCLabelTTF * m_pGetScore[10];

	CCMenuItemSprite * m_pBtnXiangQing;
	CCMenuItemSprite * m_pBtnJiLu;
};

}

#endif	//_OX_CJ4_CAI_JIN_LAYER_H_
