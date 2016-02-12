#ifndef _STAR_97_GAME_H_
#define _STAR_97_GAME_H_

class CGameDirector;

namespace GameStar97
{
class CResManager;
class CStar97LoadingLayer;
class CStar97GameLayer;

class CStar97Game : public CGameBase
{
public:
	CStar97Game(CGameDirector * pDirector);
	virtual ~CStar97Game();

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

	virtual void OnTouchOption();
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
	CStar97LoadingLayer * m_pLoadingLayer;
	CStar97GameLayer * m_pMainLayer;

	bool m_bInGame;
};

}

#endif	//_STAR_97_GAME_H_
