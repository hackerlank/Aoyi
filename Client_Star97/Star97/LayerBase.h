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
	//ȡ����Դ����֡
	CCSpriteFrame * getSpriteFrameByCfgId(int cfgid);
	//���þ�������
	void resetSpriteTexture(CCSprite * pSprite, int cfgid);
	//ͨ����ԴID��������
	CCSprite* createSpriteByResID(int nResID);
	//������ť
	CCControlButton* createControlButtonByResID(int nNormalResID, int nSelectResID);
	CCMenuItemSprite* createMenuItemSpriteByResID(int nNormalResID, int nSelectResID);
	CCMenuItemSprite* createMenuItemSpriteByResID(int nNormalResID, int nSelectResID, int nDisabledResID);

protected:
	CResourceManager* m_pResManager;
};

}
#endif	//SPARROW_XL_LAYER_BASE_H
