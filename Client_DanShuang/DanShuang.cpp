#include "DanShuang.h"
#include "DanShuangView.h"
#include "SimpleAudioEngine.h"

CDanShuangGame::CDanShuangGame(CGameDirector *pDirector):m_pDirector(pDirector)
{
	//������Ϣ
	m_lMeMaxScore = 0;
	m_lAllMaxScore = 0;
	m_lAreaLimitScore = 0;
	m_lApplyBankerCondition = 0;

	//��ע��Ϣ
	memset(m_lMeScore, 0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0, sizeof(m_lAllScore));

	//ׯ����Ϣ
	m_wCurrentBanker = INVALID_CHAIR;
	m_lBankerScore = 0;
	m_bEnableSysBanker = true;

	//״̬����
	m_bMeApplyBanker=false;
	m_bMePlaceJetton = false;

	//��������
	m_nGameStatus = GS_INVALIDE;

	m_pDanShuangGameView = NULL;

	return;
}

CDanShuangGame::~CDanShuangGame()
{
	if (m_pDanShuangGameView)
	{
		m_pDanShuangGameView->removeFromParent();
		m_pDanShuangGameView = NULL;
	}
}

bool CDanShuangGame::Init()
{
	//������Ϣ
	m_lMeMaxScore = 0;
	m_lAllMaxScore = 0;

	//��ע��Ϣ
	memset(m_lMeScore, 0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0, sizeof(m_lAllScore));

	//״̬����
	m_bMePlaceJetton = false;

	return true;
}

void CDanShuangGame::Uninit()
{

}
bool CDanShuangGame::IsInGame()
{
	if(m_pDanShuangGameView && m_pDanShuangGameView->isVisible()) return true;
	else return false;
}

bool CDanShuangGame::EnterGame()
{
	///////////////////////////////////////
	//���ͷ��Թ���Ϣ�������ͻᱻ��Ϊ�Թ�
	//��������
	CMD_GF_Info Info;
	//ZeroMemory(&Info,sizeof(Info));
	memset(&Info,0,sizeof(Info));
	//��������
	Info.bAllowLookon = FALSE;
	//��������
	m_pDirector->SendRoomData(MDM_GF_FRAME,SUB_GF_INFO,&Info,sizeof(Info));
	///////////////////////////////////////

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	if (m_pDanShuangGameView)
	{
		m_pDanShuangGameView->removeFromParent();
	}

	m_pDanShuangGameView = CDanShuangGameView::create(this);
	m_pDirector->m_pGameScene->addChild(m_pDanShuangGameView);
	CCDirector::sharedDirector()->replaceScene(m_pDirector->m_pGameScene);
	string strPath = SOUND_PATH;
	strPath += "BACK_GROUND.mp3";
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(strPath.c_str(),true);

	return true;
}

void CDanShuangGame::LeaveGame()
{
}

void CDanShuangGame::OnGB2312Message(const char* sMsg, int nType)
{
	std::string sUTF8 = CStringHelper::GB2312ToUTF8(sMsg);
	OnUTF8Message(sUTF8.c_str(), nType);
}

void CDanShuangGame::OnUTF8Message(const char* sMsg, int nType)
{
	if (0 != (SMT_INFO & nType))
	{
		m_pDanShuangGameView->ShowMessage(sMsg, false);
	}
	else if (0 != (SMT_EJECT & nType))
	{
		if (m_pDanShuangGameView)
		{
			m_pDanShuangGameView->unschedule(schedule_selector(CDanShuangGameView::OnDisconnected));
			m_pDirector->ShowNotify(sMsg, eNotifyFlag_OK, m_pDanShuangGameView->getParent(), 0, false, NULL);
		}
	}
}

bool CDanShuangGame::OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (m_nGameStatus)
	{
	case GS_GAME_FREE:	//����״̬
	case GS_GAME_ANIMATION:	//����״̬
		{
			//Ч������
			CC_ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize != sizeof(CMD_S_StatusFree))
				return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pBuffer;

			//ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser, pStatusFree->lBankerScore);
			m_pDanShuangGameView->SetBankerScore(pStatusFree->wBankerTime, pStatusFree->lBankerWinScore);
			m_bEnableSysBanker = pStatusFree->bEnableSysBanker;

			//������Ϣ
			m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			char szApplyBankerCondition[256];
			sprintf(szApplyBankerCondition,"��ׯ����:%lld��",m_lApplyBankerCondition);
			//m_pDanShuangGameView->ShowMessage(szApplyBankerCondition);

			//��Ϸ��¼
			m_pDanShuangGameView->SetGameHistory(pStatusFree->cbOpenRecord);

			//����ʱ��
			m_pDanShuangGameView->SetGameTime(pStatusFree->cbTimeLeave, m_nGameStatus);

			if (m_nGameStatus == GS_GAME_FREE)
			{
				//��������
				m_pDanShuangGameView->SetWinnerSide(0xFF);
				m_pDanShuangGameView->ClearAllJetton();
			}
			else if (m_nGameStatus == GS_GAME_ANIMATION)
			{
				//ֹͣ�ױҶ���
				m_pDanShuangGameView->BeginThrowCoin(false);
			}

			//���¿���
			UpdateButtonControl(false);

			return true;
		}
	case GS_GAME_START:	//��Ϸ״̬
		{
			//Ч������
			CC_ASSERT(wDataSize == sizeof(CMD_S_StatusJetton));
			if (wDataSize != sizeof(CMD_S_StatusJetton))
				return false;

			//��Ϣ����
			CMD_S_StatusJetton * pStatusJetton = (CMD_S_StatusJetton *)pBuffer;

			//��ע��Ϣ
			WORD wMeChairID=m_pDirector->GetMeData()->nUserChair;
			for (BYTE i =0; i < AREA_COUNT; i++)
			{
				m_lAllScore[i] += pStatusJetton->lAllScore[i];
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusJetton->lAllScore[i] - pStatusJetton->lUserScore[i], INVALID_CHAIR);
				m_lMeScore[i] += pStatusJetton->lUserScore[i];
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusJetton->lUserScore[i], wMeChairID);
				if (m_lMeScore[i] > 0)
				{
					m_bMePlaceJetton = true;
				}
			}

			//��һ���
			m_lMeMaxScore = pStatusJetton->lUserMaxScore;
			m_lAllMaxScore = pStatusJetton->lAllMaxScore;

			//������Ϣ
			m_lApplyBankerCondition = pStatusJetton->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusJetton->lAreaLimitScore;
			char szApplyBankerCondition[256];
			sprintf(szApplyBankerCondition,"��ׯ����:%lld��",m_lApplyBankerCondition);
			//m_pDanShuangGameView->ShowMessage(szApplyBankerCondition);

			//ׯ����Ϣ
			SetBankerInfo(pStatusJetton->wBankerUser, pStatusJetton->lBankerScore);
			m_pDanShuangGameView->SetBankerScore(pStatusJetton->wBankerTime, pStatusJetton->lBankerWinScore);
			m_bEnableSysBanker = pStatusJetton->bEnableSysBanker;

			//��Ϸ��¼
			m_pDanShuangGameView->SetGameHistory(pStatusJetton->cbOpenRecord);

			//����ʱ��
			m_pDanShuangGameView->SetGameTime(pStatusJetton->cbTimeLeave, m_nGameStatus);

			//ֹͣ�ױҶ���
			m_pDanShuangGameView->BeginThrowCoin(false);

			//���¿���
			UpdateButtonControl(true);
			return true;
		}
	case GS_GAME_END:	//����״̬
		{
			//Ч������
			CC_ASSERT(wDataSize == sizeof(CMD_S_StatusEnd));
			if (wDataSize != sizeof(CMD_S_StatusEnd))
				return false;

			//��Ϣ����
			CMD_S_StatusEnd * pStatusEnd = (CMD_S_StatusEnd *)pBuffer;

			//��ע��Ϣ
			WORD wMeChairID=m_pDirector->GetMeData()->nUserChair;
			for (BYTE i =0; i < AREA_COUNT; i++)
			{
				memcpy(m_lAllScore, pStatusEnd->lAllScore, sizeof(pStatusEnd->lAllScore));
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusEnd->lAllScore[i] - pStatusEnd->lUserScore[i], INVALID_CHAIR);
				memcpy(m_lMeScore, pStatusEnd->lUserScore, sizeof(pStatusEnd->lUserScore));
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusEnd->lUserScore[i], wMeChairID);
			}

			//������Ϣ
			m_lApplyBankerCondition = pStatusEnd->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusEnd->lAreaLimitScore;
			char szApplyBankerCondition[256];
			sprintf(szApplyBankerCondition,"��ׯ����:%lld��",m_lApplyBankerCondition);
			//m_pDanShuangGameView->ShowMessage(szApplyBankerCondition);

			//ׯ����Ϣ
			SetBankerInfo(pStatusEnd->wBankerUser, pStatusEnd->lBankerScore);
			m_pDanShuangGameView->SetBankerScore(pStatusEnd->wBankerTime, pStatusEnd->lBankerWinScore);
			m_bEnableSysBanker = pStatusEnd->bEnableSysBanker;

			//��Ϸ��¼
			m_pDanShuangGameView->SetGameHistory(pStatusEnd->cbOpenRecord);

			//������Ϣ
			m_pDanShuangGameView->SetCardInfo((enOpenCardType)pStatusEnd->cbOpenArea, false);
			m_pDanShuangGameView->FinishOpenAnimation(false);
			m_pDanShuangGameView->SetCurGameScore(pStatusEnd->lEndUserScore, pStatusEnd->lEndUserReturnScore,
				pStatusEnd->lEndBankerScore, pStatusEnd->lEndRevenue);

			//����ʱ��
			m_pDanShuangGameView->SetGameTime(pStatusEnd->cbTimeLeave, m_nGameStatus);

			//���¿���
			UpdateButtonControl(false);
			return true;
		}
	}
	return true;
}

bool CDanShuangGame::OnGameMessage(WORD wMainCmd, WORD wSubCmd, const void * pBuffer, WORD wDataSize)
{
	CCLOG("GameMsg:%d",wSubCmd);
	switch(wSubCmd)
	{
	case SUB_S_GAME_STATUS:					//��Ϸ״̬
		{
			//Ч������
			CC_ASSERT(wDataSize==sizeof(CMD_S_GameStatus));
			if (wDataSize!=sizeof(CMD_S_GameStatus))
				return false;

			CMD_S_GameStatus * pGameStatus = (CMD_S_GameStatus *)pBuffer;
			m_nGameStatus = pGameStatus->cbGameStatus;

			return true;
		}
	case SUB_S_GAME_FREE:					//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_ANIMATION:				//��Ϸ����
		{
			return OnSubGameAnimation(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:					//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:				//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize,true);
		}
	case SUB_S_GAME_END:					//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:				//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:				//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:				//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CLEAN:						//ȡ����ע
		{
			return OnSubClearJetton(pBuffer,wDataSize);
		}
	case SUB_S_SCORE_RECORD:				//�鿴������¼
		{
			return OnSubScoreRecord(pBuffer,wDataSize);
		}
	default:break;
	}

	CC_ASSERT(false);
	return false;
}

//����ׯ��
void CDanShuangGame::SetBankerInfo(WORD wBanker, LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	m_pDanShuangGameView->SetBankerInfo(m_wCurrentBanker,m_lBankerScore);
}

//��Ϸ����
bool CDanShuangGame::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	m_pDanShuangGameView->SetGameTime(pGameFree->cbTimeLeave, GS_GAME_FREE);

	//��������
	m_pDanShuangGameView->SetWinnerSide(0xFF);
	m_pDanShuangGameView->ClearAllJetton();

	//��ʼ������
	this->Init();
	m_nGameStatus = GS_GAME_FREE;

	//���¿ؼ�
	UpdateButtonControl(false);

	return true;
}

//��Ϸ�ױ�
bool CDanShuangGame::OnSubGameAnimation(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameAnimation));
	if (wDataSize!=sizeof(CMD_S_GameAnimation)) return false;

	//��Ϣ����
	CMD_S_GameAnimation * pGameAnimation=(CMD_S_GameAnimation *)pBuffer;

	//����ʱ��
	m_pDanShuangGameView->SetGameTime(pGameAnimation->cbTimeLeave, GS_GAME_ANIMATION);

	//����״̬
	m_nGameStatus = GS_GAME_ANIMATION;

	//�����ױҶ���
	m_pDanShuangGameView->BeginThrowCoin(true);

	//���¿ؼ�
	UpdateButtonControl(false);

	return true;
}

//��Ϸ��ʼ
bool CDanShuangGame::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	memset(m_lMeScore,0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0,sizeof(m_lAllScore));

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_lAllMaxScore = pGameStart->lAllMaxScore;

	//����ʱ��
	m_pDanShuangGameView->SetGameTime(pGameStart->cbTimeLeave,GS_GAME_START);

	//ֹͣ�ױҶ���
	m_pDanShuangGameView->BeginThrowCoin(false);

	//����״̬
	m_nGameStatus = GS_GAME_START;

	//���¿���
	UpdateButtonControl(true);

	//��������
	string strPath = SOUND_PATH;
	strPath += "GAME_START.ogg";
	SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());

	return true;
}

//�û���ע
bool CDanShuangGame::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if ((pPlaceJetton->cbJettonArea < JA_RED ) || (pPlaceJetton->cbJettonArea > JA_BLUE) || (pPlaceJetton->lJettonScore <= 0))
	{
		return true;
	}

	//�������
	BYTE cbJettonIndex = pPlaceJetton->cbJettonArea - JA_RED;	//��ע������

	//������ע��Ϣ
	m_lAllScore[cbJettonIndex] += pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pDirector->m_nMeChairID)
	{
		m_lMeScore[cbJettonIndex] += pPlaceJetton->lJettonScore;
		m_bMePlaceJetton = true;
	}

	//���½���
	m_pDanShuangGameView->PlaceUserJetton(pPlaceJetton->cbJettonArea, pPlaceJetton->lJettonScore, pPlaceJetton->wChairID);

	//���¿ؼ�
	UpdateButtonControl();

	//��������
	if (pPlaceJetton->wChairID != m_pDirector->m_nMeChairID)
	{
		if (pPlaceJetton->lJettonScore == 5000000)
		{
			string strPath = SOUND_PATH;
			strPath += "ADD_GOLD_EX.ogg";
			SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
		}
		else
		{
			string strPath = SOUND_PATH;
			strPath += "ADD_GOLD.ogg";
			SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
		}
	}

	return true;
}

//��Ϸ����
bool CDanShuangGame::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd))
		return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//���ÿ������
	m_pDanShuangGameView->SetCardInfo((enOpenCardType)pGameEnd->cbOpenArea, true);

	//����ׯ�ҷ���
	m_pDanShuangGameView->SetBankerScore(pGameEnd->wBankerTime, pGameEnd->lBankerTotalScore);

	//���óɼ���Ϣ
	m_pDanShuangGameView->SetCurGameScore(pGameEnd->lUserScore, pGameEnd->lUserReturnScore, pGameEnd->lBankerNowScore, pGameEnd->lRevenue);

	//����ʱ��
	m_pDanShuangGameView->SetGameTime(pGameEnd->cbTimeLeave, GS_GAME_END);

	//������Ϸ״̬
	m_nGameStatus = GS_GAME_END;

	//���¿ؼ�
	UpdateButtonControl(false);

	return true;
}

//������ׯ
bool CDanShuangGame::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * m_pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//�������
	if (m_wCurrentBanker != m_pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.wChairID = m_pApplyBanker->wApplyUser;
		ApplyUser.strUserName = m_pDirector->GetTableUserData(m_pApplyBanker->wApplyUser)->sUserName;
		m_pDanShuangGameView->AddApplyBanker(ApplyUser);
	}

	//�Լ��ж�
	if (m_pDirector->m_nMeChairID == m_pApplyBanker->wApplyUser)
		m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonControl();

	return true;
}

//�л�ׯ��
bool CDanShuangGame::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	WORD wPrevBanker = m_wCurrentBanker;	//�����ϸ�ׯ��

	//�Լ��ж�
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID && pChangeBanker->wBankerUser != m_pDirector->m_nMeChairID) 
	{
		m_bMeApplyBanker=false;
	}
	else if (pChangeBanker->wBankerUser == m_pDirector->m_nMeChairID)
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����Ϣ
	SetBankerInfo(pChangeBanker->wBankerUser, pChangeBanker->lBankerScore);

	//��ʾͼƬ
	m_pDanShuangGameView->ShowChangeBanker(wPrevBanker != pChangeBanker->wBankerUser);
	m_pDanShuangGameView->SetBankerScore(0, 0);

	//ɾ����ׯ�б����
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagApplyUser ApplyUser;
		ApplyUser.wChairID = m_wCurrentBanker;
		m_pDanShuangGameView->CancelApplyBanker(ApplyUser);
	}

	//���¿ؼ�
	UpdateButtonControl();

	return true;
}

//ȡ����ׯ
bool CDanShuangGame::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	CC_ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//ɾ�����
	tagApplyUser ApplyUser;
	ApplyUser.wChairID = pCancelBanker->wCancelUser;
	m_pDanShuangGameView->CancelApplyBanker(ApplyUser);

	//�Լ��ж�
	if (m_pDirector->m_nMeChairID == pCancelBanker->wCancelUser)
		m_bMeApplyBanker = false;

	//���¿ؼ�
	UpdateButtonControl();

	return true;
}

//�鿴������¼
bool CDanShuangGame::OnSubScoreRecord(const void * pBuffer, WORD wDataSize)
{
	//У������
	CC_ASSERT(false);

	//CC_ASSERT(wDataSize==sizeof(CMD_S_HisInfo));
	//if(wDataSize!=sizeof(CMD_S_HisInfo)) return false;
	//CMD_S_HisInfo * pData = (CMD_S_HisInfo *)pBuffer;

	//m_pSicBoGameView->SetHistoryInfo(*pData);

	return true;
}

//ȡ����ע
bool CDanShuangGame::OnSubClearJetton(const void * pBuffer, WORD wDataSize)
{
	//У������
	CC_ASSERT(wDataSize==sizeof(CMD_S_ClearJetton));
	if(wDataSize!=sizeof(CMD_S_ClearJetton)) return false;

	//��Ϣ����
	CMD_S_ClearJetton * pClearJetton = (CMD_S_ClearJetton *)pBuffer;

	//������ע��Ϣ
	memcpy(m_lAllScore, pClearJetton->lAllJettonScore, sizeof(pClearJetton->lAllJettonScore));
	if (m_pDirector->m_nMeChairID == pClearJetton->wChairID)
	{
		memset(m_lMeScore, 0, sizeof(m_lMeScore));
		m_bMePlaceJetton = false;
	}

	//��������ע
	m_pDanShuangGameView->ClearUserJetton(pClearJetton->wChairID, pClearJetton->lAllJettonScore);

	//���¿ؼ�
	UpdateButtonControl();

	return true;
}

//��ע��Ϣ
bool CDanShuangGame::OnPlaceJetton(BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//��ע�ж�
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID || lJettonScore <= 0 || m_nGameStatus != GS_GAME_START)
		return true;

	//�����ж�
	LONGLONG lMaxPlayerScore = GetMaxPlaceJetton(cbJettonArea);	//���������ע

	if (lJettonScore > lMaxPlayerScore)
	{
		return true;
	}

	//�������
	CMD_C_PlaceJetton PlaceJetton;
	memset(&PlaceJetton, 0, sizeof(PlaceJetton));
	PlaceJetton.cbJettonArea = cbJettonArea;
	PlaceJetton.lJettonScore = lJettonScore;

	//������Ϣ
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonControl();

	//������Ч
	if (lJettonScore==5000000)
	{
		string strPath = SOUND_PATH;
		strPath += "ADD_GOLD_EX.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}
	else
	{
		string strPath = SOUND_PATH;
		strPath += "ADD_GOLD.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}

	return 0;
}

//ȡ����ע
bool CDanShuangGame::OnClearJetton()
{
	//ȡ���ж�
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID || m_nGameStatus != GS_GAME_START)
		return true;

	//�ж�����Ƿ���ע
	bool bIsJetton = false;	//�Ƿ�����ע
	for (BYTE i = 0; i < AREA_COUNT; ++i)
	{
		if (m_lMeScore[i] > 0)
		{
			bIsJetton = true;
			break;
		}
	}
	if (bIsJetton == false)
		return true;

	//������Ϣ
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_CLEAR_JETTON);

	//���°�ť
	UpdateButtonControl();

	return true;
}

//������ׯ
bool CDanShuangGame::OnApplyBanker()
{
	//�����ж�
	if (m_bMeApplyBanker == true)
		return true;
	LONGLONG lUserScore = m_pDirector->GetMeData()->nUserScore;	//��ҿ��÷���
	for (BYTE i =0; i < AREA_COUNT; i++)
	{
		lUserScore -= m_lMeScore[i];
	}
	if (lUserScore < m_lApplyBankerCondition)
	{
		CC_ASSERT(false);
		return true;
	}

	//������Ϣ
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_APPLY_BANKER);

	//���°�ť
	UpdateButtonControl();

	return true;
}

//ȡ����ׯ
bool CDanShuangGame::OnCancelBanker()
{
	//ȡ���ж�
	if (m_bMeApplyBanker == false)
		return true;
	if ((m_wCurrentBanker == m_pDirector->m_nMeChairID) && (m_nGameStatus != GS_GAME_FREE))
		return true;

	//������Ϣ
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_CANCEL_BANKER);

	//���°�ť
	UpdateButtonControl();

	return true;
}

//���¿���
void CDanShuangGame::UpdateButtonControl(bool bEnablePlaceJetton /*= true*/)
{
	//��������ж�
	if (bEnablePlaceJetton)
	{
		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
			bEnablePlaceJetton = false;
		if (m_nGameStatus != GS_GAME_START)
			bEnablePlaceJetton = false;
		if (m_wCurrentBanker == m_pDirector->m_nMeChairID)
			bEnablePlaceJetton = false;
	}

	//����ȡ������ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�����������������
		LONGLONG lCanUserMaxScore = 0;
		for (int i=0; i<AREA_COUNT; ++i)
		{
			LONGLONG lLeaveScoreTemp = GetMaxPlaceJetton(i + JA_RED);
			if (lLeaveScoreTemp > lCanUserMaxScore)
			{
				lCanUserMaxScore = lLeaveScoreTemp;
			}
		}

		m_pDanShuangGameView->m_pJetton0->setEnabled(true);
		m_pDanShuangGameView->m_pJetton1->setEnabled(true);
		m_pDanShuangGameView->m_pJetton2->setEnabled(true);
		m_pDanShuangGameView->m_pJetton3->setEnabled(true);

		//���ó��밴ť״̬
		m_pDanShuangGameView->m_pJetton0->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[0] ? true : false);
		m_pDanShuangGameView->m_pJetton1->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[1] ? true : false);
		m_pDanShuangGameView->m_pJetton2->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[2] ? true : false);
		m_pDanShuangGameView->m_pJetton3->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[3] ? true : false);

		//����ȡ����ť״̬
		if (m_nGameStatus == GS_GAME_START && m_bMePlaceJetton == true && m_pDanShuangGameView->GetGameTime() > 5)
		{
			m_pDanShuangGameView->m_pClearJetton->setEnabled(true);
		}
		else
		{
			m_pDanShuangGameView->m_pClearJetton->setEnabled(false);
		}
	}
	else
	{
		//��ֹ��ť
		m_pDanShuangGameView->m_pJetton0->setEnabled(false);
		m_pDanShuangGameView->m_pJetton1->setEnabled(false);
		m_pDanShuangGameView->m_pJetton2->setEnabled(false);
		m_pDanShuangGameView->m_pJetton3->setEnabled(false);
		m_pDanShuangGameView->m_pClearJetton->setEnabled(false);
	}

	//������ׯ����ׯ��ť��ʾ
	if (m_bMeApplyBanker == true)
	{
		m_pDanShuangGameView->m_pApplyBanker->setVisible(false);
		m_pDanShuangGameView->m_pCancelBanker->setVisible(true);
	}
	else
	{
		m_pDanShuangGameView->m_pApplyBanker->setVisible(true);
		m_pDanShuangGameView->m_pCancelBanker->setVisible(false);
	}

	//������ׯ����ׯ��ť״̬
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID)
	{
		if (m_nGameStatus == GS_GAME_FREE)
		{
			m_pDanShuangGameView->m_pCancelBanker->setEnabled(true);
		}
		else
		{
			m_pDanShuangGameView->m_pCancelBanker->setEnabled(false);
		}
	}
	else if (m_bMeApplyBanker == true)
	{
		m_pDanShuangGameView->m_pCancelBanker->setEnabled(true);
	}
	else
	{
		//�жϿɷ���ׯ
		LONGLONG lUserScore = m_pDirector->GetMeData()->nUserScore;	//��ҿ��÷���
		for (BYTE i =0; i < AREA_COUNT; i++)
		{
			lUserScore -= m_lMeScore[i];
		}

		if (lUserScore < m_lApplyBankerCondition)
		{
			m_pDanShuangGameView->m_pApplyBanker->setEnabled(false);
		}
		else
		{
			m_pDanShuangGameView->m_pApplyBanker->setEnabled(true);
		}
	}

	return;
}

//ȡ�����������ע
LONGLONG CDanShuangGame::GetMaxPlaceJetton(BYTE cbArea)
{
	//����У��
	CC_ASSERT(cbArea >= JA_RED && cbArea <= JA_BLUE);
	if (cbArea < JA_RED || cbArea > JA_BLUE)
		return 0;

	//�������
	LONGLONG lUserAllScore = 0;	//�������ע��
	LONGLONG lAllUserAllScore = 0;	//�����������ע��
	LONGLONG lAreaAllLossScore = m_lAllScore[cbArea - JA_RED] * m_GameLogic.GetAreaLossPercent(cbArea);	//������ׯ������ע��

	for (BYTE i = 0; i < AREA_COUNT; ++i)
	{
		lUserAllScore += m_lMeScore[i];
		lAllUserAllScore += m_lAllScore[i];
	}

	//��ȡע������
	LONGLONG lAllMaxScore = m_lAllMaxScore;	//ׯ�ҿ���ע��
	LONGLONG lUserMaxScore = m_lMeMaxScore;	//��ҿ���ע��

	//����ׯ�ҿ���ע��
	lAllMaxScore = lAllMaxScore + lAllUserAllScore - lAreaAllLossScore ;
	if (lAllMaxScore <= 0)
	{
		return 0;
	}
	lAllMaxScore = lAllMaxScore / ((std::max)((WORD)2, m_GameLogic.GetAreaLossPercent(cbArea)) - 1);
	if (lAllMaxScore <= 0)
	{
		return 0;
	}

	//������ҿ���ע��
	lUserMaxScore -= lUserAllScore;
	if (lUserMaxScore<=0)
	{
		return 0;
	}

	//������������
	LONGLONG lWithAreaLimitScore = m_lAreaLimitScore - m_lAllScore[cbArea - JA_RED];
	if (lWithAreaLimitScore <= 0)
	{
		return 0;
	}

	//���㷵�ط���
	LONGLONG lReturnScore = min(lAllMaxScore, lUserMaxScore);
	lReturnScore = min(lReturnScore, lWithAreaLimitScore);

	return lReturnScore;

}

void CDanShuangGame::OnMeScoreChange(LONGLONG lScore)
{
	m_pDanShuangGameView->DrawUserScore();

	return ;
	//char szScore[256];
	//sprintf(szScore,"%lld",lScore);
	//m_pDanShuangGameView->m_pLabelUserScore->setString(szScore);
}

void CDanShuangGame::OnMeInsureScoreChange(LONGLONG lScore)
{
	return ;
	//char szScore[256];
	//sprintf(szScore,"%lld",lScore);
	//m_pDanShuangGameView->m_pSubLayer->m_pBankStorageScore->setString(szScore);
	//m_pDanShuangGameView->m_pSubLayer->m_pGiftStorageScore->setString(szScore);
}