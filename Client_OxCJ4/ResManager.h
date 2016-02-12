#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_

#include "ResourceManager.h"
#include "ResourceDef.h"

class CGameDirector;

namespace GameOxCJ4
{

class CResManager : public CResourceManager
{
public:
	CResManager(CGameDirector* pDirector);
	virtual ~CResManager();

	virtual CConfigHelper* CreateConfigHelper();
	virtual CConfigHelper* GetConfigHelper();
	virtual void Clear(bool bClearLobby);
	virtual void LoadConfigFromFile();

	virtual CGameSprite* GenerateGameSprite(unsigned int nTypeID);
	virtual CGameSprite* GenerateGameSprite(unsigned int nTypeID, unsigned nSpriteID, unsigned nCreatorID);

	const std::vector<tagAudioCfg*>& GetAudioCfgs();

	//∑˛ŒÒ∆˜”Œœ∑≈‰÷√
	int m_nSpriteIDSeed;
};

}

#endif	//_RES_MANAGER_H_