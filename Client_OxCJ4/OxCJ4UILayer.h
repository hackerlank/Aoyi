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
	Menu_Item_ID_Start = 0,	//��ʼ��ť
	Menu_Item_ID_CallBanker,	//��ׯ��ť
	Menu_Item_ID_NoCall,	//���а�ť
	Menu_Item_ID_Jetton0,	//��ע0��ť
	Menu_Item_ID_Jetton1,	//��ע1��ť
	Menu_Item_ID_Jetton2,	//��ע2��ť
	Menu_Item_ID_Jetton3,	//��ע3��ť
	Menu_Item_ID_SortCard,	//����ť
	Menu_Item_ID_ShowCard,	//̯�ư�ť
	Menu_Item_ID_TiShi,	//��ʾ��ť
	Menu_Item_ID_TuoGuan,	//�йܰ�ť
	Menu_Item_ID_NoTuoGuan,	//ȡ���йܰ�ť
	Menu_Item_ID_Out,	//�˳���ť
	Menu_Item_ID_Option,	//���ð�ť
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

	//�¼�
public:
	CCMenuItem * GetMItemById(int nItemId);
	void RestAllMenu();
	void SetJettonValue(int nCount, LONGLONG lJetton[], bool bEnable[]);

	//��ť�ص�
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

	//��������
private:
	void InitMenu();
	WORD GetUserChairID(WORD wChairID);

private:
	COxCJ4GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	//��ť���
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
