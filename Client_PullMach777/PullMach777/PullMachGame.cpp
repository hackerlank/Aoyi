#include "stdafx.h"
#include "PullMachResManager.h"
#include "PullMachResourceDef.h"
#include "PullMachLoadingLayer.h"
#include "PullMachGameLayer.h"
#include "PullMachGame.h"

namespace GamePullMach777
{

CPullMachGame::CPullMachGame(CGameDirector * pDirector)
	:m_pGameDirector(pDirector)
	,m_pResManager(NULL)
	,m_pLoadingLayer(NULL)
	,m_pMainLayer(NULL)
	,m_bInGame(false)
{
	m_pResManager = new CPMResManager(m_pGameDirector);
	m_pResManager->CreateConfigHelper();
}

CPullMachGame::~CPullMachGame()
{
	RemoveLoadingLayer();
	RemoveLayer();

	if (m_pResManager)
	{
		m_pResManager->Clear(false);
	}
	CC_SAFE_DELETE(m_pResManager);
	m_pResManager = NULL;
}

bool CPullMachGame::Init()
{
	return true;
}

void CPullMachGame::Uninit()
{

}

bool CPullMachGame::IsInGame()
{
	if (!m_bInGame)
		return false;

	if (m_pMainLayer && m_pMainLayer->IsInit())
	{
		return true;
	}

	return false;
}

CRoomSprite CPullMachGame::CreateRoomSprite()
{
	CRoomSprite room;

	return room;
}

CTableSprite CPullMachGame::CreateTableSprite()
{
	CTableSprite tableSprite;

	return tableSprite;
}

bool CPullMachGame::EnterGame()
{
	if (m_bInGame)
	{
		return true;
	}
	m_bInGame = true;
	CCLOG("====[PullMach]enter game");

	LoadGameCfg();

	RemoveLoadingLayer();
	LoadLoadingLayer();

	RemoveLayer();
	LoadLayer();

	OnAudioChange();

	m_pGameDirector->SwitchLayerWithFade(eLayerType_LoadingPullMachGame, 0.01f);

	return true;
}

void CPullMachGame::LeaveGame()
{
	m_bInGame = false;
}

void CPullMachGame::RemoveLoadingLayer()
{
	if (m_pLoadingLayer)
	{
		m_pLoadingLayer->Clear();

		if (m_pLoadingLayer->getParent())
		{
			m_pLoadingLayer->getParent()->removeChild(m_pLoadingLayer);
		}

		m_pGameDirector->RemoveGameLayer(m_pLoadingLayer->GetLayerID());

		m_pLoadingLayer = NULL;
	}
}

void CPullMachGame::LoadLoadingLayer()
{
	m_pLoadingLayer = new CPullMachLoadingLayer(m_pResManager);
	m_pLoadingLayer->autorelease();

	CGameLayer* pSubLayer = m_pLoadingLayer;
	pSubLayer->SetTypeID(eLayerType_LoadingPullMachGame);
	pSubLayer->SetGameDirector(m_pGameDirector);
	pSubLayer->setContentSize(CCSizeMake(WINDOW_WIDTH, CONTENTS_HEIGHT));
	pSubLayer->setPosition(ccp(0, BUTTOM_HEIGHT));
	pSubLayer->setZOrder(4);
	pSubLayer->OnLayerOut(NULL, eLayerAni_Null);
	pSubLayer->setVisible(false);
	//pSubLayer->SetResManager(m_pResManager);

	m_pGameDirector->AddGameLayer(pSubLayer);
	m_pGameDirector->GetLobbyScene()->addChild(pSubLayer);
}

void CPullMachGame::RemoveLayer()
{
	if (m_pMainLayer)
	{
		m_pMainLayer->Clear();

		if (m_pMainLayer->getParent())
		{
			m_pMainLayer->getParent()->removeChild(m_pMainLayer);
		}

		m_pMainLayer = NULL;
	}
}

void CPullMachGame::LoadLayer()
{
	m_pMainLayer = new CPullMachGameLayer(this, m_pResManager);
	m_pMainLayer->autorelease();

	CGameLayer* pSubLayer = m_pMainLayer;
	pSubLayer->SetResManager(m_pResManager);
	pSubLayer->SetGameDirector(m_pGameDirector);
	pSubLayer->setContentSize(CCSizeMake(WINDOW_WIDTH, CONTENTS_HEIGHT));
	pSubLayer->SetOrginPos(ccp(0, CONTENTS_YPOS));
	pSubLayer->setZOrder(1);
	pSubLayer->OnLayerIn(NULL, eLayerAni_Null);
	m_pGameDirector->GetGameScene()->addChild(pSubLayer);
}

void CPullMachGame::OnAudioChange()
{
	if(!IsInGame())
	{
		return;
	}

	if (m_pGameDirector->m_bCfgEnableBackgroundMusic)
	{
		if (m_bInGame)
		{
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void CPullMachGame::OnUserSit(unsigned short wChairID)
{
	if(!IsInGame())
	{
		return;
	}

	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(wChairID);
	if (pUser == NULL)
		return;

	if (m_pMainLayer)
	{
		m_pMainLayer->updateUserScore(pUser->nUserScore);
	}
}

void CPullMachGame::OnUserStateChange(unsigned short wChairID, int nOldState, int nNewState)
{

}

void CPullMachGame::OnUserLeave(unsigned short wChairID)
{

}

void CPullMachGame::OnTouchClose()
{
	m_pGameDirector->End();
}

bool CPullMachGame::OnEventGameOption(BYTE cbGameStatus, bool bAllowLookon)
{
	if (m_pMainLayer)
		m_pMainLayer->SetGameStatus(cbGameStatus);

	return true;
}

bool CPullMachGame::OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	if(!IsInGame())
	{
		CCLOG("fp OnEventGameScene erroe");
		return true;
	}

	if (m_pMainLayer)
		m_pMainLayer->OnEventGameScene(bLookonOther, pBuffer, wDataSize);

	return true;
}

bool CPullMachGame::OnGameMessage(WORD wMainCmd, WORD wSubCmd, const void * pBuffer, WORD wDataSize)
{
	if (!IsInGame())
	{
		return true;
	}

	if (wMainCmd == MDM_GF_GAME && m_pMainLayer)
		m_pMainLayer->OnGameMessage(wSubCmd, pBuffer, wDataSize);

	return true;
}

void CPullMachGame::OnGB2312Message(const char* sMsg, int nType)
{
	std::string sUTF8 = CStringHelper::GB2312ToUTF8(sMsg);
	OnUTF8Message(sUTF8.c_str(), nType);
}

void CPullMachGame::OnUTF8Message(const char* sMsg, int nType)
{
	if(!IsInGame())
	{
		return;
	}

	if (m_pMainLayer)
	{
		//m_pMainLayer->ShowGameMessage(sMsg);
	}
}

void CPullMachGame::OnMeScoreChange(int nScore)
{
	if(!IsInGame() || !m_pMainLayer)
	{
		return;
	}

	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(0);
	if (pUser != NULL)
	{
		m_pMainLayer->updateUserScore(pUser->nUserScore);
	}
}

///////////////////////////////////////////////////////////////////////

bool CPullMachGame::LoadGameCfg()
{
	CConfigHelper* configHelper = m_pResManager->GetConfigHelper();
	if (configHelper == NULL)
	{
		ERROR_CHECK;
		return false;
	}

	if (m_pGameDirector->m_bUseIPC)
	{
		if (m_pGameDirector->m_bCfgCfgPacketLoading)
		{
			configHelper->m_sLoadingPak = "|";
			configHelper->m_sLoadingPak +=  m_pGameDirector->m_sCfgLocalLoadingFix;
			configHelper->m_sLoadingPak += ".pak|";
		}
		else
		{
			configHelper->m_sLoadingPak = m_pGameDirector->m_sCfgLocalLoadingFix;
		}

		if (m_pGameDirector->m_bCfgResPacketLoading)
		{
			configHelper->m_sResPak = "|";
			configHelper->m_sResPak +=  m_pGameDirector->m_sCfgLocalResFix;
			configHelper->m_sResPak += ".pak|";
		}
		else
		{
			configHelper->m_sResPak = m_pGameDirector->m_sCfgLocalResFix;
		}
	}
	else
	{
		for (std::list<tagKindCfg*>::iterator iter = m_pGameDirector->m_pResManager->GetConfigHelper()->m_vGameKindCfgs.begin(); iter != m_pGameDirector->m_pResManager->GetConfigHelper()->m_vGameKindCfgs.end(); iter++)
		{
			tagKindCfg* pKind = *iter;

			if (m_pGameDirector->m_nCurKindID == pKind->nID)
			{
				configHelper->m_sLoadingPak = pKind->sLoadingFix;
				configHelper->m_sResPak = pKind->sResFix;
				break;
			}
		}

	}

	{
		if (m_pGameDirector->m_bCfgNoNetworkMode)
		{
			m_pResManager->GetConfigHelper()->m_bIgnoreServerOnlyFlag = true;
			CCFileUtils::sharedFileUtils()->setPopupNotify(false);
		}
		else
		{
			CCFileUtils::sharedFileUtils()->setPopupNotify(m_pGameDirector->m_nCfgDebugMode > 0);
		}

		m_pResManager->LoadConfigFromFile();
	}

	if (m_pGameDirector->m_nCfgDebugMode > 0)
	{
		cocos2d::CCLog("start loading no network(%d), loading pak(%s), res pak(%s), debug(%d) size(%.2f, %.2f, %.2f) freq(%.2f) \r\n", 
			m_pGameDirector->m_bCfgNoNetworkMode?1:0,
			m_pResManager->GetConfigHelper()->m_sLoadingPak.c_str(),
			m_pResManager->GetConfigHelper()->m_sResPak.c_str(),
			m_pGameDirector->m_nCfgDebugMode,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			m_pGameDirector->m_fCfgZoom,
			1/m_pGameDirector->m_fFreq);
	}

	return true;
}


void CPullMachGame::ShowPrize()
{
	if(!IsInGame())
	{
		return;
	}

	if (m_pMainLayer)
	{
		m_pMainLayer->m_pGamePrizeLayer->Show(NULL);
	}
}

void CPullMachGame::insertChatMsg(tagLocalChatMsg* pLocalChatMsg)
{
	if(!IsInGame())
	{
		return;
	}

	if (m_pMainLayer)
	{
		m_pMainLayer->m_pChatViewLayer->insertChatMsg(pLocalChatMsg);
	}
}

void CPullMachGame::ReceliveBox(DWORD dwCountDown, unsigned int nScore)
{
	if(!IsInGame())
	{
		return;
	}

	if (m_pMainLayer)
	{
		m_pMainLayer->m_pBoxLayer->ReceliveBox(dwCountDown, nScore);
	}
}

void CPullMachGame::updateBoxInfo(DWORD dwCountDown, BYTE cbRecelive)
{
	if(!IsInGame())
	{
		return;
	}

	if (m_pMainLayer)
	{
		m_pMainLayer->m_pBoxLayer->updateBoxInfo(dwCountDown, cbRecelive);
	}
}

}
