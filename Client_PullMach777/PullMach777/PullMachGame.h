#ifndef _STAR_97_GAME_H_
#define _STAR_97_GAME_H_

class CGameDirector;

namespace GamePullMach777
{
class CPMResManager;
class CPullMachLoadingLayer;
class CPullMachGameLayer;

class CPullMachGame : public CGameBase
{
public:
	CPullMachGame(CGameDirector * pDirector);
	virtual ~CPullMachGame();

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

	virtual void ShowPrize();
	virtual void insertChatMsg(tagLocalChatMsg* pLocalChatMsg);
	virtual void ReceliveBox(DWORD dwCountDown, unsigned int nScore);
	virtual void updateBoxInfo(DWORD dwCountDown, BYTE cbRecelive);

	///////////////////////////////////////////////////////////////////////
	virtual bool LoadGameCfg();

public:
	CGameDirector* GetGameDirector() { return m_pGameDirector; }
	CPMResManager* GetResManager() { return m_pResManager; }
	void RemoveLoadingLayer();
	void LoadLoadingLayer();
	void RemoveLayer();
	void LoadLayer();

private:
	CGameDirector * m_pGameDirector;

	CPMResManager * m_pResManager;
	CPullMachLoadingLayer * m_pLoadingLayer;
	CPullMachGameLayer * m_pMainLayer;

	bool m_bInGame;
};

}

#endif	//_STAR_97_GAME_H_
