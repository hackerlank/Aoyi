#ifndef _JETTON_MEN_SPRITE_H_
#define _JETTON_MEN_SPRITE_H_

class CJettonMenuSprite: public CCMenuItemSprite
{
public:
	CJettonMenuSprite();
	virtual ~CJettonMenuSprite();

	//������ť
	static CJettonMenuSprite * create(CCSprite* normalSprite, CCSprite* selectedSprite, CCSprite* disabledSprite, CCObject* target, SEL_MenuHandler selector);
	//������ʾ����
	void setDisplayView(bool bEnable);

private:
	CCSprite * m_pNormalNode;
	CCSprite * m_pSelectedNode;
	CCSprite * m_pDisabledNode;
	bool m_bIsRetain;
	bool m_bIsEnable;
};

#endif