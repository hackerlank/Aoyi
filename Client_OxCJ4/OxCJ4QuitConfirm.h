#ifndef _OX_CJ4_QUIT_CONFIRM_H_
#define _OX_CJ4_QUIT_CONFIRM_H_

#include "OxCJ4GameLayer.h"
#include "ResManager.h"
#include "LayerBase.h"

namespace GameOxCJ4
{

class CLayerBase;

class COxCJ4QuitConfirm : public CLayerBase
{
public:
	static COxCJ4QuitConfirm * create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);

	COxCJ4QuitConfirm(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);
	~COxCJ4QuitConfirm();

	virtual bool init();
	void OnTouchQuit(CCObject* pObject);
	void OnTouchCancel(CCObject* pObject);

private:
	COxCJ4GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	CCMenuItemSprite * m_pBtnQuit;
	CCMenuItemSprite * m_pBtnCancel;
};

}

#endif	//_OX_CJ4_QUIT_CONFIRM_H_
