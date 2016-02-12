#ifndef _STAR97_SETTING_LAYER_H_
#define _STAR97_SETTING_LAYER_H_

#include "LayerBase.h"

namespace GameStar97
{

class CResManager;

class CStar97SettingLayer : public CCLayer, public CLayerBase
{
public:
	static CStar97SettingLayer * create(CResManager * pResManager);

	CStar97SettingLayer(CResManager * pResManager);
	~CStar97SettingLayer();

	virtual bool init();

public:
	//点击关闭
	void OnTouchClose(CCObject* pObject);
	//更新音乐音量
	void SliderChangeMusic(CCObject *sender, CCControlEvent controlEvent);
	//更新音效音量
	void SliderChangeEffect(CCObject *sender, CCControlEvent controlEvent);
	//更新静音与否
	void SliderChangeQuiet(CCObject *sender, CCControlEvent controlEvent);

private:
	CResManager * m_pResManager;

	CCControlSlider * m_pMusicSlider;
	CCControlSlider * m_pEffectSlider;

	float m_fMusicValue;	//音乐音量
	float m_fEffectValue;	//音效音量
};

}
#endif	//_STAR97_SETTING_LAYER_H_

