#ifndef _OX_CJ4_LOADING_LAYER_H_
#define _OX_CJ4_LOADING_LAYER_H_

namespace GameStar97
{
class CResManager;

class CStar97LoadingLayer : public CGameLayer
{
public:
	CStar97LoadingLayer(CResManager * pResManager);
	virtual ~CStar97LoadingLayer();

	virtual void OnInit();
	void OnLoadResFinish(CCObject *pObj);
	void animatReady();

private:
	int m_AddCount;
	CCSize screenSize;
	CResManager * m_pResManager;
	//bool m_bResLoadReady;	//资源加载完成
	//bool m_bAnimatReady;	//动画播放完成
};
}


#endif	//_OX_CJ4_LOADING_LAYER_H_
