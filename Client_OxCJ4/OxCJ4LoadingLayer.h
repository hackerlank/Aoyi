#ifndef _OX_CJ4_LOADING_LAYER_H_
#define _OX_CJ4_LOADING_LAYER_H_

namespace GameOxCJ4
{
class CResManager;

class COxCJ4LoadingLayer : public CGameLayer
{
public:
	COxCJ4LoadingLayer(CResManager * pResManager);
	virtual ~COxCJ4LoadingLayer();

	virtual void OnInit();
	void OnLoadResFinish(CCObject *pObj);
	void DisplaySelf(CCNode *pNode);
	void DisappearSelf(CCNode *pNode);
	int m_AddCount;
	CCSize screenSize;
	CResManager * m_pResManager;

};
}


#endif	//_OX_CJ4_LOADING_LAYER_H_
