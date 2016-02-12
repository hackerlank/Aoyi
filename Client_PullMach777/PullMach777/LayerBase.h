#ifndef SPARROW_XL_LAYER_BASE_H
#define SPARROW_XL_LAYER_BASE_H

#include "ResourceManager.h"

namespace GamePullMach777
{

class CLayerBase
{
public:

	CLayerBase(CResourceManager* m_pResManager);
	virtual ~CLayerBase();

protected:
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
