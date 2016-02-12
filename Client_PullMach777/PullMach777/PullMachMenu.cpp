#include "stdafx.h"
#include "PullMachMenu.h"

namespace GamePullMach777
{

CPullMachMenu * CPullMachMenu::create()
{
	CPullMachMenu * pRet = new CPullMachMenu();
	if (pRet && pRet->initWithArray(NULL))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

CPullMachMenu::CPullMachMenu()
	:CCMenu()
{

}

CPullMachMenu::~CPullMachMenu()
{

}

bool CPullMachMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CC_UNUSED_PARAM(event);
	if (m_eState != kCCMenuStateWaiting || ! m_bVisible || !isEnabled())
	{
		return false;
	}

	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	m_pSelectedItem = this->itemForTouchPM(touch);
	if (m_pSelectedItem)
	{
		m_eState = kCCMenuStateTrackingTouch;
		m_pSelectedItem->selected();
		return true;
	}
	return false;
}

CCMenuItem* CPullMachMenu::itemForTouchPM(CCTouch * touch)
{
	CCPoint touchLocation = touch->getLocation();

	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCMenuItem* pChild = dynamic_cast<CCMenuItem*>(pObject);
			if (pChild && pChild->isVisible() && pChild->isEnabled())
			{
				CCPoint local = pChild->convertToNodeSpace(touchLocation);
				CCRect r;
				r.size = pChild->getContentSize();
				r.origin.x = r.size.width * 0.2f;
				r.size.width *= 0.6f;
				r.origin.y = r.size.height * 0.2f;
				r.size.height *= 0.8f;

				if (r.containsPoint(local))
				{
					return pChild;
				}
			}
		}
	}

	return NULL;
}

}
