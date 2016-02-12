#ifndef _OX_CJ4_LOADING_LAYER_H_
#define _OX_CJ4_LOADING_LAYER_H_

namespace GamePullMach777
{
class CPMResManager;

class CPullMachLoadingLayer : public CGameLayer
{
public:
	CPullMachLoadingLayer(CPMResManager* pResManager);
	virtual ~CPullMachLoadingLayer();

	virtual void OnLayerIn(CGameLayer* pOutLayer, ELayerAni enAniType, float fDuration = 0.5f);
	virtual void OnLayerOut(CGameLayer* pInLayer, ELayerAni enAniType, float fDuration = 0.5f);
	virtual void OnLayerIn(float fDuration);
	virtual void OnLayerOut(float fDuration);
	void OnLayerInInit();
	virtual void OnLayerRemove();

	virtual void OnInit();
	virtual void OnLoadResFinish(CCObject* pObj); 

private:
	int m_AddCount;

	CCSize csize;

	CCLayerColor *m_pLayer;
	CPMResManager* m_pGameResManager;
};
}


#endif	//_OX_CJ4_LOADING_LAYER_H_
