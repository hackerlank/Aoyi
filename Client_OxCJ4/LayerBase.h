#ifndef SPARROW_XL_LAYER_BASE_H
#define SPARROW_XL_LAYER_BASE_H

namespace GameOxCJ4
{

class CLayerBase : public CCLayer
{
public:

	CLayerBase(CResourceManager* m_pResManager);
	virtual ~CLayerBase();

	virtual bool init();

	static WORD ServerTableIndexToLocal(WORD wDesChair, WORD wMyChair);
protected:
	void addToRoot(CCNode* pNode, int zOrde = 0);
	void removeFromRoot(CCNode* pNode);

	CCSprite* createSpriteByResID(int nResID);
	CCControlButton* createControlButtonByResID(int nNormalResID, int nSelectResID);
	CCMenuItemSprite* createMenuItemSpriteByResID(int nNormalResID, int nSelectResID);
	CCMenuItemSprite* createMenuItemSpriteByResID(int nNormalResID, int nSelectResID, int nDisabledResID);

protected:
	CResourceManager* m_pResManager;
	CCNode* m_pRootNode;
};

}
#endif	//SPARROW_XL_LAYER_BASE_H
