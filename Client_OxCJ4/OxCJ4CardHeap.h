#ifndef _OX_CJ4_CARD_HEAP_H_
#define _OX_CJ4_CARD_HEAP_H_

#include "ResManager.h"

namespace GameOxCJ4
{

class COxCJ4CardHeap : public CCNode
{
public:
	static COxCJ4CardHeap * create(CResManager * pResManager);

	COxCJ4CardHeap(CResManager * pResManager);
	virtual ~COxCJ4CardHeap();

	virtual bool init();

public:
	void SetCardData(const BYTE cbCardData[], BYTE cbCardCount, bool bIsOxType, float fScale = 0.5f);
	void ClearAllCard();

private:
	CCSpriteBatchNode * m_pBatchNode;
	CResManager * m_pResManager;
};

}


#endif	//_OX_CJ4_CARD_HEAP_H_
