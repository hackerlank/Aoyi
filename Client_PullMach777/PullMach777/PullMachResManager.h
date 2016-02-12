#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_

#include "ResourceManager.h"
#include "PullMachResourceDef.h"

class CGameDirector;

namespace GamePullMach777
{

//动画ID
enum enGameAnimationId
{
	GameAni_BoxRed7 = 0,	//宝箱红7动画
	GameAni_Red7Swing,	//红7上下摇摆
	GameAni_BoxLight,	//宝箱大奖动画
	GameAni_GirlFly,	//小女孩飞舞

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

	//通过ID取得动画名称
	const char * getAnimationNameById(enGameAnimationId id);
	//取得资源精灵帧
	CCSpriteFrame * getSpriteFrameByCfgId(int cfgid);
	//重置精灵帧纹理
	void resetSpriteFrameTexture(CCSprite * pSprite, int cfgid);
	//重置精灵纹理
	void resetSpriteTexture(CCSprite * pSprite, int cfgid);

	//服务器游戏配置
	int m_nSpriteIDSeed;
};

}

#endif	//_RES_MANAGER_H_