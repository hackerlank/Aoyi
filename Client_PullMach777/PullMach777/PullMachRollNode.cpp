#include "stdafx.h"
#include "PullMachRollNode.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{
static const float g_sfIconObjHeight = 55.0f;	//物品图标高度
static const float g_sfIconLayerHeight = ICON_SPRITE_COUNT * g_sfIconObjHeight;	//图标图层高度
static const float g_sfRollSpeed = 40.0f;	//转动速度
const CCSize CPullMachRollNode::m_contentSize(122.0f, 165.0f);

CPullMachRollNode * CPullMachRollNode::create(CPMResManager * pResManager)
{
	CPullMachRollNode * pRet = new CPullMachRollNode(pResManager);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return NULL;
}

CPullMachRollNode::CPullMachRollNode(CPMResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pIconLayer1(NULL)
	,m_pIconLayer2(NULL)
	,m_fRollEndLength(0.0f)
	,m_fRollSumLength(0.0f)
	,m_fLayer1BeginPosY(0.0f)
	,m_fLayer2BeginPosY(0.0f)
	,m_bIsRolling(false)
{
	memset(m_pIconSprite1, 0, sizeof(m_pIconSprite1));
	memset(m_pIconSprite2, 0, sizeof(m_pIconSprite2));
	memset(m_pResultSprite, 0, sizeof(m_pResultSprite));
	memset(m_cbResult, 0, sizeof(m_cbResult));
}

CPullMachRollNode::~CPullMachRollNode()
{

}

bool CPullMachRollNode::init()
{
	CC_ASSERT(ICON_SPRITE_COUNT >= 4);

	//初始化滚动图层1
	m_pIconLayer1 = CCLayer::create();
	m_pIconLayer1->setPosition(CCPointZero);
	addChild(m_pIconLayer1, 1);

	//创建精灵集合类
	std::string sPath = m_pResManager->GetConfigHelper()->m_sLoadingPak+"images/pak/common.pvr.ccz";
	CCSpriteBatchNode * batchNode = CCSpriteBatchNode::create(sPath.c_str(), ICON_SPRITE_COUNT);
	for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
	{
		CCSprite * pSprite = NULL;

		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vObjCardResList[rand() % OBJECT_TYPE], pSprite))
			continue;

		pSprite->setAnchorPoint(ccp(0,0));
		pSprite->setPosition(ccp(0, g_sfIconObjHeight * i));
		batchNode->addChild(pSprite, ICON_SPRITE_COUNT - i);

		m_pIconSprite1[i] = pSprite;
	}
	m_pIconLayer1->addChild(batchNode);

	//初始化滚动图层2
	m_pIconLayer2 = CCLayer::create();
	m_pIconLayer2->setPosition(ccp(0, g_sfIconLayerHeight + m_pIconLayer1->getPositionY()));
	addChild(m_pIconLayer2, 1);

	batchNode = CCSpriteBatchNode::create(sPath.c_str(), ICON_SPRITE_COUNT);
	for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
	{
		CCSprite * pSprite = NULL;
		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vObjCardResList[rand() % OBJECT_TYPE], pSprite))
			continue;

		pSprite->setAnchorPoint(ccp(0,0));
		pSprite->setPosition(ccp(0, g_sfIconObjHeight * i));
		batchNode->addChild(pSprite, ICON_SPRITE_COUNT - i);

		m_pIconSprite2[i] = pSprite;
	}
	m_pIconLayer2->addChild(batchNode);

	return true;
}

void CPullMachRollNode::visit()
{
	if (!isVisible())
		return;

	if (!m_pChildren || m_pChildren->count() == 0)
		return;

	kmGLPushMatrix();

	transform();
	beforeDraw();

	ccArray *arrayData = m_pChildren->data;
	for(unsigned int i = 0 ; i < arrayData->num; i++ )
	{
		CCNode* pNode = (CCNode*) arrayData->arr[i];

		if (pNode)
		{
			pNode->visit();
		}
	}

	afterDraw();

	kmGLPopMatrix();
}

void CPullMachRollNode::beforeDraw()
{
	CCRect frame = getViewRect();
	glEnable(GL_SCISSOR_TEST);
	CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

void CPullMachRollNode::afterDraw()
{
	glDisable(GL_SCISSOR_TEST);
}


CCRect CPullMachRollNode::getViewRect()
{
	CCPoint screenPos = this->convertToWorldSpace(CCPointZero);

	float scaleX = this->getScaleX();
	float scaleY = this->getScaleY();

	for (CCNode *p = m_pParent; p != NULL; p = p->getParent()) {
		scaleX *= p->getScaleX();
		scaleY *= p->getScaleY();
	}

	CCSize viewSize(m_contentSize.width * scaleX, m_contentSize.height * scaleY);

	if(scaleX<0.f) {
		screenPos.x += viewSize.width;
		scaleX = -scaleX;
	}
	if(scaleY<0.f) {
		screenPos.y += viewSize.height;
		scaleY = -scaleY;
	}

	return CCRectMake(screenPos.x, screenPos.y, viewSize.width, viewSize.height);
}

//随机图标图层结果
void CPullMachRollNode::randIconLayerResult(int nLayerID)
{
	//参数校验
	CC_ASSERT(nLayerID >= 1 && nLayerID <= 2);
	if (nLayerID < 1 || nLayerID > 2)
		return ;

	if (nLayerID == 1)
	{
		for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[i], g_vObjCardResList[rand() % OBJECT_TYPE]);
		}
	}
	else if (nLayerID == 2)
	{
		for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[i], g_vObjCardResList[rand() % OBJECT_TYPE]);
		}
	}
}

void CPullMachRollNode::beginRollByResult(BYTE cbResult[3])
{
	if (m_bIsRolling)
		return ;
	memcpy(m_cbResult, cbResult, sizeof(cbResult[0]) * 3);

	//初始化转动参数
	m_fRollEndLength = 0.0f;
	m_fRollSumLength = 0.0f;
	m_fLayer1BeginPosY = 0.0f;
	m_fLayer2BeginPosY = 0.0f;
	m_bIsRolling = true;

	//启动正常转动定时器
	schedule(schedule_selector(CPullMachRollNode::normalRollSchedule), 0.03f);
}

void CPullMachRollNode::endRollIconLayer()
{
	if (!m_bIsRolling)
		return ;

	//停止正常转动
	unschedule(schedule_selector(CPullMachRollNode::normalRollSchedule));

	//设置停止转动参数
	m_fRollEndLength = computeEndRollLength();
	CC_ASSERT(m_fRollEndLength > g_sfRollSpeed);
	CC_ASSERT(m_fRollEndLength < g_sfIconLayerHeight);

	m_fRollSumLength = 0.0f;
	m_fLayer1BeginPosY = m_pIconLayer1->getPositionY();
	m_fLayer2BeginPosY = m_pIconLayer2->getPositionY();

	//启动结束转动定时器
	schedule(schedule_selector(CPullMachRollNode::finishRollSchedule), 0.03f);
}

void CPullMachRollNode::flashIcon(int nPos)
{
	//参数校验
	CC_ASSERT(nPos >= 0 && nPos < 3);
	if (nPos < 0 || nPos >= 3)
		return ;

	//闪烁图标
	CCActionInterval * pTintTo = CCTintTo::create(0.3f, 50, 50, 50);
	CCActionInterval * pTintRev = CCTintTo::create(0.3f, 255, 255, 255);
	m_pResultSprite[nPos]->runAction(CCRepeat::create(
		CCSequence::create(pTintTo, pTintRev, NULL), -1
		));
	m_pResultSprite[nPos]->runAction(CCSequence::create(
		CCDelayTime::create(1.8f),
		CCCallFuncN::create(this, callfuncN_selector(CPullMachRollNode::stopFlashCallBack)),
		NULL
		));
}

void CPullMachRollNode::stopFlashCallBack(CCNode * pSender)
{
	pSender->stopAllActions();
	dynamic_cast<CCSprite *>(pSender)->setColor(ccc3(255, 255, 255));
}

void CPullMachRollNode::normalRollSchedule(float dt)
{
	m_pIconLayer1->setPositionY(m_pIconLayer1->getPositionY() - g_sfRollSpeed);
	m_pIconLayer2->setPositionY(m_pIconLayer2->getPositionY() - g_sfRollSpeed);

	//循环判断
	checkIconLayerPos();
}

void CPullMachRollNode::finishRollSchedule(float dt)
{
	//计算转动速度
	float fSpeed = g_sfRollSpeed;
	m_fRollSumLength += fSpeed;

	//设置位置
	if (m_fRollEndLength - m_fRollSumLength <= 1.0f)
	{
		m_pIconLayer1->setPositionY(m_fLayer1BeginPosY - m_fRollEndLength);
		m_pIconLayer2->setPositionY(m_fLayer2BeginPosY - m_fRollEndLength);

		unschedule(schedule_selector(CPullMachRollNode::finishRollSchedule));
		m_bIsRolling = false;
	}
	else
	{
		m_pIconLayer1->setPositionY(m_pIconLayer1->getPositionY() - fSpeed);
		m_pIconLayer2->setPositionY(m_pIconLayer2->getPositionY() - fSpeed);
	}

	//循环判断
	checkIconLayerPos();

	return ;
}

//计算停止长度
float CPullMachRollNode::computeEndRollLength(int nEndCount /*= 4*/)
{
	//参数校验
	if (nEndCount < 4 || nEndCount > ICON_SPRITE_COUNT)
	{
		CC_ASSERT(false);
		nEndCount = 4;
	}

	//计算图标图层1、2，当前处在显示框中间位置的，相应图标索引
	CCPoint worldPos = this->convertToWorldSpace(CCPointMake(0, m_contentSize.height / 2));
	int nIndex1 = computeIndexAtWorldPos(worldPos, 1);
	int nIndex2 = computeIndexAtWorldPos(worldPos, 2);

	//计算开奖结果对应类型
	int nChangeType[3] = {0};
	for(int i = 0; i < 3; ++i)
	{
		nChangeType[i] = g_vObjCardResList[m_cbResult[i] - 1];
	}
	memset(m_pResultSprite, 0, sizeof(m_pResultSprite));

	//计算长度
	float fLength = 0.0f;
	if (nIndex1 >= 0)
	{
		CCSprite * pSprite = m_pIconSprite1[nIndex1];
		CCPoint spriteWorldPos = pSprite->convertToWorldSpace(CCPointZero);
		fLength = nEndCount * g_sfIconObjHeight + spriteWorldPos.y + g_sfIconObjHeight / 2 - worldPos.y;

		//更改结果位置，对应图标类型
		int nDestIndex = nIndex1 + nEndCount;	//目标索引
		if (nDestIndex < ICON_SPRITE_COUNT)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nDestIndex], nChangeType[1]);
			m_pResultSprite[1] = m_pIconSprite1[nDestIndex];
		}
		else
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nDestIndex % ICON_SPRITE_COUNT], nChangeType[1]);
			m_pResultSprite[1] = m_pIconSprite2[nDestIndex % ICON_SPRITE_COUNT];
		}

		//替换上下2图标
		if (nDestIndex == 0)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nDestIndex + 1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[ICON_SPRITE_COUNT - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite1[nDestIndex + 1];
			m_pResultSprite[2] = m_pIconSprite2[ICON_SPRITE_COUNT - 1];
		}
		else if (nDestIndex < ICON_SPRITE_COUNT - 1)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nDestIndex + 1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nDestIndex - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite1[nDestIndex + 1];
			m_pResultSprite[2] = m_pIconSprite1[nDestIndex - 1];
		}
		else if (nDestIndex == ICON_SPRITE_COUNT - 1)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[0], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nDestIndex - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite2[0];
			m_pResultSprite[2] = m_pIconSprite1[nDestIndex - 1];
		}
		else if (nDestIndex == ICON_SPRITE_COUNT)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[ICON_SPRITE_COUNT - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite2[1];
			m_pResultSprite[2] = m_pIconSprite1[ICON_SPRITE_COUNT - 1];
		}
		else if (nDestIndex > ICON_SPRITE_COUNT)
		{
			int nRealIndex = nDestIndex % ICON_SPRITE_COUNT;
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nRealIndex + 1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nRealIndex - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite2[nRealIndex + 1];
			m_pResultSprite[2] = m_pIconSprite2[nRealIndex - 1];
		}
	}
	else if (nIndex2 >= 0)
	{
		CCSprite * pSprite = m_pIconSprite2[nIndex2];
		CCPoint spriteWorldPos = pSprite->convertToWorldSpace(CCPointZero);
		fLength = nEndCount * g_sfIconObjHeight + spriteWorldPos.y + g_sfIconObjHeight / 2 - worldPos.y;

		//更改结果位置，对应图标类型
		int nDestIndex = nIndex2 + nEndCount;	//目标索引
		if (nDestIndex < ICON_SPRITE_COUNT)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nDestIndex], nChangeType[1]);
			m_pResultSprite[1] = m_pIconSprite2[nDestIndex];
		}
		else
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nDestIndex % ICON_SPRITE_COUNT], nChangeType[1]);
			m_pResultSprite[1] = m_pIconSprite1[nDestIndex % ICON_SPRITE_COUNT];
		}

		//替换上下2图标
		if (nDestIndex == 0)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nDestIndex + 1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[ICON_SPRITE_COUNT - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite2[nDestIndex + 1];
			m_pResultSprite[2] = m_pIconSprite1[ICON_SPRITE_COUNT - 1];
		}
		else if (nDestIndex < ICON_SPRITE_COUNT - 1)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nDestIndex + 1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nDestIndex - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite2[nDestIndex + 1];
			m_pResultSprite[2] = m_pIconSprite2[nDestIndex - 1];
		}
		else if (nDestIndex == ICON_SPRITE_COUNT - 1)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[0], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[nDestIndex - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite1[0];
			m_pResultSprite[2] = m_pIconSprite2[nDestIndex - 1];
		}
		else if (nDestIndex == ICON_SPRITE_COUNT)
		{
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite2[ICON_SPRITE_COUNT - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite1[1];
			m_pResultSprite[2] = m_pIconSprite2[ICON_SPRITE_COUNT - 1];
		}
		else if (nDestIndex > ICON_SPRITE_COUNT)
		{
			int nRealIndex = nDestIndex % ICON_SPRITE_COUNT;
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nRealIndex + 1], nChangeType[0]);
			m_pResManager->resetSpriteFrameTexture(m_pIconSprite1[nRealIndex - 1], nChangeType[2]);
			m_pResultSprite[0] = m_pIconSprite1[nRealIndex + 1];
			m_pResultSprite[2] = m_pIconSprite1[nRealIndex - 1];
		}
	}
	else
	{
		CC_ASSERT(false);
		return 0.0f;
	}

	return fLength;
}

int CPullMachRollNode::computeIndexAtWorldPos(const CCPoint & worldPos, int nLayerID)
{
	//参数校验
	CC_ASSERT(nLayerID >= 1 && nLayerID <= 2);
	if (nLayerID < 1 || nLayerID > 2)
		return false;

	//取得图标数组
	CCSprite ** spriteArray = 0;
	CCLayer * layer = 0;
	if (nLayerID == 1)
	{
		spriteArray = m_pIconSprite1;
		layer = m_pIconLayer1;
	}
	else if (nLayerID == 2)
	{
		spriteArray = m_pIconSprite2;
		layer = m_pIconLayer2;
	}

	int nIndex = -1;
	for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
	{
		//计算图标对应世界坐标
		CCSprite * pSprite = spriteArray[i];
		CCPoint pos = layer->convertToWorldSpace(pSprite->getPosition());

		//位置判断
		if ((pos.y <= worldPos.y) && (pos.y >= worldPos.y - g_sfIconObjHeight))
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

void CPullMachRollNode::checkIconLayerPos()
{
	if (m_pIconLayer1->getPositionY() + g_sfIconLayerHeight < -g_sfRollSpeed)
	{
		m_pIconLayer1->setPositionY(m_pIconLayer2->getPositionY() + g_sfIconLayerHeight);
		randIconLayerResult(1);
	}

	if (m_pIconLayer2->getPositionY() + g_sfIconLayerHeight < -g_sfRollSpeed)
	{
		m_pIconLayer2->setPositionY(m_pIconLayer1->getPositionY() + g_sfIconLayerHeight);
		randIconLayerResult(2);
	}
}

}
