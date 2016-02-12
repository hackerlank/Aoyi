#ifndef _OX_CJ4_TOUCH_CARD_H_
#define _OX_CJ4_TOUCH_CARD_H_

#include "ResManager.h"

namespace GameOxCJ4
{

class COxCJ4GameLayer;

class COxCJ4TouchCard : public CCSprite, public CCTargetedTouchDelegate
{
public:
	static COxCJ4TouchCard * createWithCfgID(COxCJ4GameLayer * pGameLayer, CResManager * pResManager, int nType, int nCfgId);

	COxCJ4TouchCard();
	~COxCJ4TouchCard();

public:
	bool initWithCfgID(COxCJ4GameLayer * pGameLayer, CResManager * pResManager, int nType, int nCfgId);
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch * touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	void SetTouchEnable(bool bCanTouch){m_bCanTouch = bCanTouch;}
	bool IsJump() {return m_bIsJump;}
	void SetJump(bool bCanJump);
private:
	bool containsTouchLocation(CCTouch * touch);

private:
	COxCJ4GameLayer * m_pGameLayer;
	bool m_bCanTouch;
	bool m_bIsJump;
	bool m_bTouchEnd;
};

}

#endif	//_OX_CJ4_TOUCH_CARD_H_
