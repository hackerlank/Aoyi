#ifndef _PullMach_UI_LAYER_H_
#define _PullMach_UI_LAYER_H_

#include "LayerBase.h"

namespace GamePullMach777
{

class CPullMachGameLayer;
class CPMResManager;
class CLayerBase;
class CPullMachMenu;

//��Ϸ��ťtag
enum enGameBtnTag
{
	GBTag_Start = 10,	//��ʼ
	GBTag_AutoStart,	//�Զ���ʼ
	GBTag_CancelAuto,	//ȡ���Զ�
	GBTag_CreditScore,	//�Ϸ�
	GBTag_BetScore,	//Ѻע
	GBTag_StopLeft,	//ͣ��
	GBTag_StopMid,	//ͣ��
	GBTag_StopRight,	//ͣ��
};

class CPullMachUILayer : public CCLayer, public CLayerBase
{
public:
	static CPullMachUILayer * create(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);

	CPullMachUILayer(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);
	~CPullMachUILayer();

	virtual bool init();

public:
	//���ð�ť��ʾ
	void setBtnVisible(enGameBtnTag eBtnTag, bool bVisible);
	//���ð�ť״̬
	void setBtnEnable(enGameBtnTag eBtnTag, bool bEnable);

	//��ť�ص�
public:
	//��ʼ��ť
	void OnTouchStart(CCObject* pObject);
	//�Զ���ʼ��ť
	void OnTouchAutoStart(CCObject* pObject);
	//ȡ���Զ���ť
	void OnTouchCancelAuto(CCObject* pObject);
	//�Ϸְ�ť
	void OnTouchCreditScore(CCObject* pObject);
	//Ѻע��ť
	void OnTouchBetScore(CCObject* pObject);
	//����ֹͣ��ť
	void OnTouchStopLeft(CCObject* pObject);
	//����ֹͣ��ť
	void OnTouchStopMid(CCObject* pObject);
	//����ֹͣ��ť
	void OnTouchStopRight(CCObject* pObject);
	//����˳�
	void OnTouchQuit(CCObject *sender, CCControlEvent controlEvent);
	//�������
	void OnTouchHelp(CCObject *sender, CCControlEvent controlEvent);
	//�������
	void OnTouchStorage(CCObject *sender, CCControlEvent controlEvent);

	//���̰���
	void ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag);
	//����̧��
	void ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag);

private:
	CPullMachGameLayer * m_pGameLayer;
	CPMResManager * m_pResManager;

	//��ť���
	CPullMachMenu * m_pMenuLayer;
	CCMenuItemSprite * m_pBtnStart;	//��ʼ��ť
	CCMenuItemSprite * m_pBtnAutoStart;	//�Զ���ʼ��ť
	CCMenuItemSprite * m_pBtnCancelAuto;	//ȡ���Զ�
	CCMenuItemSprite * m_pBtnCreditScore;	//�Ϸְ�ť
	CCMenuItemSprite * m_pBtnBetScore;	//��ע��ť
	CCMenuItemSprite * m_pBtnStopLeft;	//ͣ���ְ�ť
	CCMenuItemSprite * m_pBtnStopMid;	//ͣ���ְ�ť
	CCMenuItemSprite * m_pBtnStopRight;	//ͣ���ְ�ť
	CCControlButton * m_pBtnStorage;	//���а�ť
	CCControlButton * m_pBtnQuitGame;	//�˳���Ϸ
	CCControlButton * m_pBtnHelp;		//������ť
};

}

#endif	//_PullMach_UI_LAYER_H_
