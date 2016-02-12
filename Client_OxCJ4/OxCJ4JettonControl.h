#ifndef _OX_CJ4_JETTON_CONTROL_H_
#define _OX_CJ4_JETTON_CONTROL_H_

#include "ResManager.h"
#include "CMD_OxCJ4.h"
namespace GameOxCJ4
{

class COxCJ4GameLayer;
class CLayerBase;

class COxCJ4JettonControl : public CCLayer
{
public:
	static COxCJ4JettonControl * create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);

	COxCJ4JettonControl(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);
	~COxCJ4JettonControl();

	virtual bool init();

public:
	void RemoveAllJetton();
	void UserPlaceJetton(WORD wChairID, BYTE cbJettonCount, LONGLONG lJettonValue);
	void GivePlaceJetton(WORD wGiveUser, WORD wAcceptUser);
	void GiveHoldJetton(WORD wGiveUser, WORD wAcceptUser, BYTE cbJettonCount);
	void GiveWinningsJetton(WORD wChairID, BYTE cbJettonCount);
	WORD GetUserChairID(WORD wChairID);
	void RemoveSelf(CCNode * pRemNode);
	void SetNodeVertexZ(CCNode * pNode);

private:
	COxCJ4GameLayer * m_pGameLayer;
	CResManager * m_pResManager;
	CCSpriteBatchNode * m_pPlaceBatchNode[GAME_PLAYER];
	CCSpriteBatchNode * m_pHoldBatchNode;

	int m_nJetAreaRadius;
	CCPoint m_PlaceJetPoint[GAME_PLAYER];
	CCPoint m_HoldJetPoint[GAME_PLAYER];
	CCLabelAtlas * m_pLabelValue[GAME_PLAYER];
};

}

#endif	//_OX_CJ4_JETTON_CONTROL_H_
