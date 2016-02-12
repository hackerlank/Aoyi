#ifndef SPARROW_XL_LAYER_BASE_H
#define SPARROW_XL_LAYER_BASE_H

#include "ResourceManager.h"

namespace GameStar97
{

class CLayerBase
{
public:

	CLayerBase(CResourceManager* m_pResManager);
	virtual ~CLayerBase();

protected:
	//取得资源精灵帧
	CCSpriteFrame * getSpriteFrameByCfgId(int cfgid);
	//重置精灵纹理
	void resetSpriteTexture(CCSprite * pSprite, int cfgid);
	//通过资源ID创建精灵
	CCSprite* createSpriteByResID(int nResID);
	//创建按钮
	CCControlButton* createControlButtonByResID(int nNormalResID, int nSelectResID);
	CCMenuItemSprite* createMenuItemSpriteByResID(int nNormalResID, int nSelectResID);
	CCMenuItemSprite* createMenuItemSpriteByResID(int nNormalResID, int nSelectResID, int nDisabledResID);

protected:
	CResourceManager* m_pResManager;
};

}
#endif	//SPARROW_XL_LAYER_BASE_H
