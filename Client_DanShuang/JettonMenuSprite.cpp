#include "stdafx.h"
#include "JettonMenuSprite.h"

CJettonMenuSprite::CJettonMenuSprite():m_pNormalNode(NULL), m_pSelectedNode(NULL), m_pDisabledNode(NULL), m_bIsRetain(false), m_bIsEnable(false)
{

}

CJettonMenuSprite::~CJettonMenuSprite()
{
	if (m_bIsRetain)
	{
		m_pNormalNode->release();
		m_pSelectedNode->release();
		m_pDisabledNode->release();
	}
}

CJettonMenuSprite * CJettonMenuSprite::create(CCSprite* normalSprite, CCSprite* selectedSprite, CCSprite* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{
	if (normalSprite == NULL || selectedSprite == NULL || disabledSprite == NULL)
	{
		CC_ASSERT(false);
		return NULL;
	}

	CJettonMenuSprite * pRet = new CJettonMenuSprite();
	if (pRet && pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector))
	{
		pRet->autorelease();
		pRet->m_pNormalNode = normalSprite;
		pRet->m_pSelectedNode = selectedSprite;
		pRet->m_pDisabledNode = disabledSprite;
		normalSprite->retain();
		selectedSprite->retain();
		disabledSprite->retain();
		pRet->m_bIsRetain = true;
		pRet->m_bIsEnable = true;
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void CJettonMenuSprite::setDisplayView(bool bEnable)
{
	if (m_bIsEnable == bEnable)
		return ;

	if (bEnable == true)
	{
		setNormalImage(CCSprite::createWithTexture(m_pNormalNode->getTexture(), m_pNormalNode->getTextureRect()));
		setSelectedImage(CCSprite::createWithTexture(m_pSelectedNode->getTexture(), m_pSelectedNode->getTextureRect()));
		m_bIsEnable = bEnable;
	}
	else
	{
		setNormalImage(CCSprite::createWithTexture(m_pDisabledNode->getTexture(), m_pDisabledNode->getTextureRect()));
		setSelectedImage(CCSprite::createWithTexture(m_pDisabledNode->getTexture(), m_pDisabledNode->getTextureRect()));
		m_bIsEnable = bEnable;
	}
}
