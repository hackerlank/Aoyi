#ifndef _STAR97_UI_LAYER_H_
#define _STAR97_UI_LAYER_H_

#include "LayerBase.h"

namespace GameStar97
{

class CStar97GameLayer;
class CResManager;
class CLayerBase;

//��Ϸ��ťtag
enum enGameBtnTag
{
	GBTag_AutoStart = 10,	//�Զ���ʼ
	GBTag_CancelAuto ,	//ȡ���Զ�
	GBTag_CreditScore ,	//�Ϸ�
	GBTag_Start ,	//��ʼ
	GBTag_BetScore ,	//Ѻע
	GBTag_LookPrize ,	//�鿴��
	GBTag_Exit ,	//�˳�
	GBTag_Setting,	//����
};

class CStar97UILayer : public CCLayer, public CLayerBase
{
public:
	static CStar97UILayer * create(CStar97GameLayer * pGameLayer, CResManager * pResManager);

	CStar97UILayer(CStar97GameLayer * pGameLayer, CResManager * pResManager);
	~CStar97UILayer();

	virtual bool init();

public:
	//������ҷ���
	void setUserScore(LONGLONG lUserScore);
	//���ð�ť��ʾ
	void setBtnVisible(enGameBtnTag eBtnTag, bool bVisible);
	//���ð�ť״̬
	void setBtnEnable(enGameBtnTag eBtnTag, bool bEnable);

	//��ť�ص�
public:
	void OnTouchAutoStart(CCObject* pObject);
	void OnTouchCancelAuto(CCObject* pObject);
	void OnTouchCreditScore(CCObject* pObject);
	void OnTouchStart(CCObject* pObject);
	void OnTouchBetScore(CCObject* pObject);
	void OnTouchLookPrize(CCObject* pObject);
	void OnTouchExit(CCObject* pObject);
	void OnTouchSetting(CCObject* pObject);

	//���̰���
	void ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag);
	//����̧��
	void ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag);

private:
	CStar97GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	//��ť���
	CCMenu * m_pMenuLayer;
	CCMenuItemSprite * m_pBtnAutoStart;	//�Զ���ʼ��ť
	CCMenuItemSprite * m_pBtnCancelAuto;	//ȡ���Զ���ť
	CCMenuItemSprite * m_pBtnCreditScore;	//�Ϸְ�ť
	CCMenuItemSprite * m_pBtnStart;	//��ʼ��ť
	CCMenuItemSprite * m_pBtnBetScore;	//Ѻ�ְ�ť
	CCMenuItemSprite * m_pBtnLookPrize;	//�鿴�󽱰�ť
	CCMenuItemSprite * m_pBtnExit;	//�˳���ť
	CCMenuItemSprite * m_pBtnSetting;	//���ð�ť

	//��ʾ
	CCLabelAtlas * m_pFontUserScore;

};

}

#endif	//_STAR97_UI_LAYER_H_