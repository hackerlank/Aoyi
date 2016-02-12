#ifndef _OX_CJ4_UILAYER_H_
#define _OX_CJ4_UILAYER_H_

#include <map>
#include "LayerBase.h"
#include "OxCJ4GameLayer.h"
#include "ResManager.h"

namespace GameOxCJ4
{

enum enMenuItemID
{
	Menu_Item_ID_Start = 0,	//开始按钮
	Menu_Item_ID_CallBanker,	//叫庄按钮
	Menu_Item_ID_NoCall,	//不叫按钮
	Menu_Item_ID_Jetton0,	//下注0按钮
	Menu_Item_ID_Jetton1,	//下注1按钮
	Menu_Item_ID_Jetton2,	//下注2按钮
	Menu_Item_ID_Jetton3,	//下注3按钮
	Menu_Item_ID_SortCard,	//排序按钮
	Menu_Item_ID_ShowCard,	//摊牌按钮
	Menu_Item_ID_TiShi,	//提示按钮
	Menu_Item_ID_TuoGuan,	//托管按钮
	Menu_Item_ID_NoTuoGuan,	//取消托管按钮
	Menu_Item_ID_Out,	//退出按钮
	Menu_Item_ID_Option,	//设置按钮
};

class COxCJ4UILayer : public CLayerBase
{
public:
	static COxCJ4UILayer * create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);

	COxCJ4UILayer(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);
	~COxCJ4UILayer();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	//事件
public:
	CCMenuItem * GetMItemById(int nItemId);
	void RestAllMenu();
	void SetJettonValue(int nCount, LONGLONG lJetton[], bool bEnable[]);

	//按钮回调
private:
	void OnTouchStart(CCObject* pObject);
	void OnTouchCallBanker(CCObject* pObject);
	void OnTouchNoCall(CCObject* pObject);
	void OnTouchJetton(CCObject* pObject);
	void OnTouchSortCard(CCObject* pObject);
	void OnTouchShowCard(CCObject* pObject);
	void OnTouchTiShi(CCObject* pObject);
	void OnTouchTuoGuan(CCObject* pObject);
	void OnTouchNoTuoGuan(CCObject* pObject);
	void OnTouchOut(CCObject* pObject);
	void OnTouchOption(CCObject* pObject);

	//辅助函数
private:
	void InitMenu();
	WORD GetUserChairID(WORD wChairID);

private:
	COxCJ4GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	//按钮相关
	CCMenuItemSprite * m_pBtnStart;
	CCMenuItemSprite * m_pBtnCallBanker;
	CCMenuItemSprite * m_pBtnNoCall;
	CCMenuItemSprite * m_pBtnJetton[4];
	int m_nJetBeginTag;
	CCMenuItemSprite * m_pBtnSortCard;
	CCMenuItemSprite * m_pBtnShowCard;
	CCMenuItemSprite * m_pBtnTiShi;
	CCMenuItemSprite * m_pBtnTuoGuan;
	CCMenuItemSprite * m_pBtnNoTuoGuan;
	CCMenuItemSprite * m_pBtnOut;
	CCMenuItemSprite * m_pBtnOption;
	std::map<int, CCMenuItem *> m_MenuArray;

	CCLabelAtlas * m_pJettonValue[4];
};

}

#endif	//_OX_CJ4_UILAYER_H_
