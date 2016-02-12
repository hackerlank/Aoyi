#ifndef _OX_CJ4_USER_INFO_H_
#define _OX_CJ4_USER_INFO_H_

#include "LayerBase.h"
#include "OxCJ4GameLayer.h"
#include "ResManager.h"

namespace GameOxCJ4
{

class COxCJ4UserInfo : public CLayerBase
{
public:
	static COxCJ4UserInfo * create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);

	COxCJ4UserInfo(COxCJ4GameLayer * pGameLayer, CResManager * pResManager);
	~COxCJ4UserInfo();

	virtual bool init();

public:
	void SetUserInfo(WORD wChairID, const tagLocalUser * pUserInfo);
	void SetUserScore(WORD wChairID, LONGLONG lScore);
	void SetBanker(WORD wChairID);
	CCPoint GetUserCoinWordPos(WORD wChairID);

private:
	WORD GetUserChairID(WORD wChairID);
	CCSprite * GetUserHeadSprite(int faceid, int gender);

	//∏®÷˙¿‡
private:
	class CUserInfoNode : public CCNode
	{
	public:
		CUserInfoNode();
		~CUserInfoNode();

		virtual bool init();
		CREATE_FUNC(CUserInfoNode);

	public:
		CCSprite * m_pFace;
		CCLabelTTF * m_pName;
		CCSprite * m_pCoin;
		CCLabelAtlas * m_pScore;
	};

private:
	COxCJ4GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	//ΩÁ√Ê
	CCSprite * m_pUserInfoBk[4];
	CUserInfoNode * m_UserInfo[4];
	CCSprite * m_pBanker;
};

}

#endif	//_OX_CJ4_USER_INFO_H_
