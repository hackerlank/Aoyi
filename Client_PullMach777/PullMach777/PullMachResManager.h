#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_

#include "ResourceManager.h"
#include "PullMachResourceDef.h"

class CGameDirector;

namespace GamePullMach777
{

//����ID
enum enGameAnimationId
{
	GameAni_BoxRed7 = 0,	//�����7����
	GameAni_Red7Swing,	//��7����ҡ��
	GameAni_BoxLight,	//����󽱶���
	GameAni_GirlFly,	//СŮ������

};

class CPMResManager : public CResourceManager
{
public:
	CPMResManager(CGameDirector* pDirector);
	virtual ~CPMResManager();

	virtual CConfigHelper* CreateConfigHelper();
	virtual CConfigHelper* GetConfigHelper();
	virtual void Clear(bool bClearLobby);
	virtual void LoadConfigFromFile();

	virtual CGameSprite* GenerateGameSprite(unsigned int nTypeID);
	virtual CGameSprite* GenerateGameSprite(unsigned int nTypeID, unsigned nSpriteID, unsigned nCreatorID);
	CCControlButton* CreateControlButton(int nNormalSpriteID, int nHighlightedSpriteID, int nDisabledSpriteID = -1);

	const std::vector<tagAudioCfg*>& GetAudioCfgs();

	//ͨ��IDȡ�ö�������
	const char * getAnimationNameById(enGameAnimationId id);
	//ȡ����Դ����֡
	CCSpriteFrame * getSpriteFrameByCfgId(int cfgid);
	//���þ���֡����
	void resetSpriteFrameTexture(CCSprite * pSprite, int cfgid);
	//���þ�������
	void resetSpriteTexture(CCSprite * pSprite, int cfgid);

	//��������Ϸ����
	int m_nSpriteIDSeed;
};

}

#endif	//_RES_MANAGER_H_