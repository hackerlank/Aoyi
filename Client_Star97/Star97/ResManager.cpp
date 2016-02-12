#include "stdafx.h"
#include "ResManager.h"

namespace GameStar97
{
CResManager::CResManager(CGameDirector* pDirector)
	: CResourceManager(pDirector)
	, m_nSpriteIDSeed(1)
{

}

CResManager::~CResManager()
{

}

CConfigHelper* CResManager::CreateConfigHelper()
{
	if (NULL == m_pConfigHelp)
	{
		m_pConfigHelp = new CConfigHelper();
	}
	else
	{
		ERROR_CHECK;
	}

	return m_pConfigHelp;
}

CConfigHelper* CResManager::GetConfigHelper()
{
	return m_pConfigHelp;
}

void CResManager::Clear(bool bClearLobby)
{
	CResourceManager::Clear(bClearLobby);
}

void CResManager::LoadConfigFromFile()
{
	CResourceManager::LoadConfigFromFile();

}

CGameSprite* CResManager::GenerateGameSprite(unsigned int nTypeID)
{
	unsigned int nUserID = 0;
	nUserID = m_pGameDirector->m_nMeUserID;

	unsigned int nSpriteID = m_nSpriteIDSeed++;
	return GenerateGameSprite(nTypeID, nSpriteID, nUserID);
}


CGameSprite* CResManager::GenerateGameSprite(unsigned int nTypeID, unsigned nSpriteID, unsigned nCreatorID)
{
	if (0 == nSpriteID)
	{
		return NULL;
	}

	CGameSprite* pRet = NULL;

	switch (nTypeID)
	{
	case eSpriteType_Base:
		{
			pRet = new CGameSprite(nTypeID, this);
		}
		break;
	case eSpriteType_Cfg:
		{
			pRet = NULL;
		}
		break;
	default:
		{
			ERROR_CHECK;
			pRet = NULL;
		}
		break;
	}

	if (NULL == pRet)
	{
		return NULL;
	}

	//pRet->SetResourceManager(this);
	//pRet->m_nSpriteTypeID = nTypeID;

	//ÉèÖÃID
	pRet->SetSpriteID(nSpriteID);
	pRet->SetCreatorID(nCreatorID);

	return pRet;
}

const std::vector<tagAudioCfg*>& CResManager::GetAudioCfgs()
{
	return m_pConfigHelp->m_vAudioCfgs;
}

}