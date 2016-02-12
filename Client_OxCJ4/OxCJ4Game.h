#ifndef _OX_CJ4_GAME_H_
#define _OX_CJ4_GAME_H_

class CGameDirector;

namespace GameOxCJ4
{
class CResManager;
class COxCJ4LoadingLayer;
class COxCJ4GameLayer;

class COxCJ4Game : public CGameBase
{
public:
	COxCJ4Game(CGameDirector * pDirector);
	virtual ~COxCJ4Game();

	///////////////////////////////////////////////////////////////////////
	virtual bool Init();
	virtual void Uninit();
	virtual bool IsInGame();

	virtual CRoomSprite CreateRoomSprite();
	virtual CTableSprite CreateTableSprite();

	virtual bool EnterGame();
	virtual void LeaveGame();

	virtual void OnAudioChange();

	virtual void OnUserSit(unsigned short wChairID);
	virtual void OnUserStateChange(unsigned short wChairID, int nOldState, int nNewState);
	virtual void OnUserLeave(unsigned short wChairID);

	virtual void OnTouchOption(){};
	virtual void OnTouchClose();

	virtual bool OnEventGameOption(BYTE cbGameStatus, bool bAllowLookon);
	virtual bool OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize);
	virtual bool OnGameMessage(WORD wMainCmd, WORD wSubCmd, const void * pBuffer, WORD wDataSize);
	virtual void OnGB2312Message(const char* sMsg, int nType);
	virtual void OnUTF8Message(const char* sMsg, int nType);
	virtual void OnMeScoreChange(int nScore);
	///////////////////////////////////////////////////////////////////////
	virtual bool LoadGameCfg();

public:
	CGameDirector* GetGameDirector() { return m_pGameDirector; }
	CResManager* GetResManager() { return m_pResManager; }

private:
	CGameDirector * m_pGameDirector;

	CResManager * m_pResManager;
	COxCJ4LoadingLayer * m_pLoadingLayer;
	COxCJ4GameLayer * m_pMainLayer;

	bool m_bInGame;
};

}

#endif	//_OX_CJ4_GAME_H_
