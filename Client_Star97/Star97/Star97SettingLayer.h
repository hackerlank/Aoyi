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
	//����ر�
	void OnTouchClose(CCObject* pObject);
	//������������
	void SliderChangeMusic(CCObject *sender, CCControlEvent controlEvent);
	//������Ч����
	void SliderChangeEffect(CCObject *sender, CCControlEvent controlEvent);
	//���¾������
	void SliderChangeQuiet(CCObject *sender, CCControlEvent controlEvent);

private:
	CResManager * m_pResManager;

	CCControlSlider * m_pMusicSlider;
	CCControlSlider * m_pEffectSlider;

	float m_fMusicValue;	//��������
	float m_fEffectValue;	//��Ч����
};

}
#endif	//_STAR97_SETTING_LAYER_H_

