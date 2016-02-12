#include "stdafx.h"
#include "Star97RollNode.h"
#include "ResourceDef.h"
#include "ResManager.h"
#include "LayerBase.h"
#include "Star97GameConstant.h"

namespace GameStar97
{
static const float g_sfIconObjHeight = 93.0f;	//物品图标高度
static const float g_sfIconStarHeight = 153.0f;	//明星图标高度
static const float g_sfIconLayerHeight = ICON_SPRITE_COUNT * g_sfIconObjHeight;	//图标图层高度
static const float g_sfRollSpeed = 30.0f;	//转动速度
const CCSize CStar97RollNode::m_contentSize(202.0f, 153.0f);

CStar97RollNode * CStar97RollNode::create(CResManager * pResManager)
{
	CStar97RollNode * pRet = new CStar97RollNode(pResManager);
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

CStar97RollNode::CStar97RollNode(CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pResManager(pResManager)
	,m_pStarLayer(NULL)
	,m_pIconLayer1(NULL)
	,m_pIconLayer2(NULL)
	,m_nResultTypeID(0)
	,m_fRollEndLength(0.0f)
	,m_fRollSumLength(0.0f)
	,m_fLayer1BeginPosY(0.0f)
	,m_fLayer2BeginPosY(0.0f)
	,m_bIsRolling(false)
	,m_bCanStopSound(false)
	,m_eRollType(RollType_Normal)
	,m_fEndAcceler(0.0f)
	,m_nEndTime(0)
	,m_nRandBIndex(0)
{
	memset(m_pIconSprite1, 0, sizeof(m_pIconSprite1));
	memset(m_pIconSprite2, 0, sizeof(m_pIconSprite2));
	memset(m_nTypePos, 0, sizeof(m_nTypePos));
	memset(m_nTypeCount, 0, sizeof(m_nTypeCount));
}

CStar97RollNode::~CStar97RollNode()
{

}

bool CStar97RollNode::init()
{
	CC_ASSERT(ICON_SPRITE_COUNT == CountArray(g_vSortIconResList) / 2 && ICON_SPRITE_COUNT >= 4);	//要实现顺序转动，必须满足此条件

	//初始化滚动图层1
	m_pIconLayer1 = CCLayer::create();
	m_pIconLayer1->setPosition(ccp(0, g_sfIconStarHeight / 2 - g_sfIconObjHeight * 1.5));
	addChild(m_pIconLayer1, 1);

	//创建精灵集合类
	std::string sPath = m_pResManager->GetConfigHelper()->m_sLoadingPak+"images/pak/common.png";
	CCSpriteBatchNode * batchNode = CCSpriteBatchNode::create(sPath.c_str(), ICON_SPRITE_COUNT);
	m_nRandBIndex = rand() % Sort_Icon_Count;	//起始位置
	for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
	{
		CCSprite * pSprite = NULL;

		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vSortIconResList[(m_nRandBIndex + i) % Sort_Icon_Count], pSprite))
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
		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vSortIconResList[(m_nRandBIndex + ICON_SPRITE_COUNT + i) % Sort_Icon_Count], pSprite))
			continue;

		pSprite->setAnchorPoint(ccp(0,0));
		pSprite->setPosition(ccp(0, g_sfIconObjHeight * i));
		batchNode->addChild(pSprite, ICON_SPRITE_COUNT - i);

		m_pIconSprite2[i] = pSprite;
	}
	m_pIconLayer2->addChild(batchNode);

	//初始化明星图层
	m_pStarLayer = CCLayer::create();
	m_pStarLayer->setPosition(CCPointZero);
	m_pStarLayer->setVisible(false);
	addChild(m_pStarLayer, 2);

	//计算各物体对应位置
	memset(m_nTypePos, 0, sizeof(m_nTypePos));
	memset(m_nTypeCount, 0, sizeof(m_nTypeCount));
	for (int i = 0; i < Sort_Icon_Count; ++i)
	{
		BYTE cbType = g_vSortIconResList[i] - Star97_CARD_Leech;
		CC_ASSERT(cbType + 1 >= OTypeLeech && cbType + 1 <= OTypeNum7);
		m_nTypePos[cbType][m_nTypeCount[cbType]] = i;
		++m_nTypeCount[cbType];
	}
#ifdef _DEBUG
	for (int i = 0; i < OBJECT_TYPE; ++i)
	{
		CC_ASSERT(m_nTypeCount[i] <= CountArray(m_nTypePos[0]));
	}
#endif

	return true;
}

void CStar97RollNode::visit()
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

void CStar97RollNode::beforeDraw()
{
	CCRect frame = getViewRect();
	glEnable(GL_SCISSOR_TEST);
	CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

void CStar97RollNode::afterDraw()
{
	glDisable(GL_SCISSOR_TEST);
}


CCRect CStar97RollNode::getViewRect()
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

	//return CCRectMake(screenPos.x - viewSize.width / 2, screenPos.y - viewSize.height / 2, viewSize.width, viewSize.height);
	return CCRectMake(screenPos.x, screenPos.y, viewSize.width, viewSize.height);
}

//随机图标图层结果
void CStar97RollNode::randIconLayerResult(int nLayerID)
{
	//参数校验
	CC_ASSERT(nLayerID >= 1 && nLayerID <= 2);
	if (nLayerID < 1 || nLayerID > 2)
		return ;

	if (nLayerID == 1)
	{
		for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
		{
			resetSpriteTexture(m_pIconSprite1[i], g_vSortIconResList[(m_nRandBIndex + i) % Sort_Icon_Count]);
		}
	}
	else if (nLayerID == 2)
	{
		for (int i = 0; i < ICON_SPRITE_COUNT; ++i)
		{
			resetSpriteTexture(m_pIconSprite2[i], g_vSortIconResList[(m_nRandBIndex + ICON_SPRITE_COUNT + i) % Sort_Icon_Count]);
		}
	}
}

void CStar97RollNode::beginRollIconLayer(int nTypeID)
{
	//参数校验
	if (nTypeID < 0 || nTypeID >= CountArray(g_vObjCardResList))
	{
		CC_ASSERT(false);
		return ;
	}

	m_nResultTypeID = nTypeID;

	//处理明星图层
	if (m_pStarLayer->getChildrenCount() > 0)
	{
		m_pStarLayer->removeAllChildren();
		m_pStarLayer->setVisible(false);

		//重置图标图层1、2
		m_pIconLayer1->setPosition(ccp(0, g_sfIconStarHeight / 2 - g_sfIconObjHeight * 1.5));
		m_pIconLayer2->setPosition(ccp(0, g_sfIconLayerHeight + m_pIconLayer1->getPositionY()));
		randIconLayerResult(1);
		randIconLayerResult(2);
	}

	if (m_bIsRolling)
		return ;

	//初始化转动参数
	m_fRollEndLength = 0.0f;
	m_fRollSumLength = 0.0f;
	m_fLayer1BeginPosY = 0.0f;
	m_fLayer2BeginPosY = 0.0f;
	m_bIsRolling = true;
	m_bCanStopSound = false;
	m_eRollType = RollType_Normal;

	//启动正常转动定时器
	schedule(schedule_selector(CStar97RollNode::normalRollSchedule), 0.03f);
}

void CStar97RollNode::endRollIconLayer()
{
	if (!m_bIsRolling || m_eRollType != RollType_Normal)
		return ;

	//停止正常转动
	unschedule(schedule_selector(CStar97RollNode::normalRollSchedule));

	//设置停止转动参数
	m_fRollEndLength = computeEndRollLength();
	CC_ASSERT(m_fRollEndLength > g_sfRollSpeed);
	CC_ASSERT(m_fRollEndLength < g_sfIconLayerHeight);

	m_fRollSumLength = 0.0f;
	m_fLayer1BeginPosY = m_pIconLayer1->getPositionY();
	m_fLayer2BeginPosY = m_pIconLayer2->getPositionY();

	//启动结束转动定时器
	m_fEndAcceler  = g_sfRollSpeed * g_sfRollSpeed / 2 / m_fRollEndLength;
	m_nEndTime = 0;
	schedule(schedule_selector(CStar97RollNode::finishRollSchedule), 0.03f);
}

void CStar97RollNode::setStarPrize(int nStarLevel, int nStarIndex)
{
	//参数校验
	if (nStarLevel < 2 || nStarLevel > 4 || nStarIndex < 0 || nStarIndex > 8)
	{
		CC_ASSERT(false);
		return ;
	}

	if (m_bIsRolling)
		return ;

	//构建明星图层
	CCSprite * pSprite = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Card_Back_Null, pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(CCPointZero);
		m_pStarLayer->addChild(pSprite);
	}

	int nResId = (nStarIndex == 1) ? g_vRateResList[nStarLevel - 2] : Star97_Card_Back_Null;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, nResId, pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(0, g_sfIconStarHeight));
		m_pStarLayer->addChild(pSprite);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vStarResList[nStarLevel - 2][nStarIndex], pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(0, g_sfIconStarHeight * 2));
		m_pStarLayer->addChild(pSprite);
	}

	m_pStarLayer->setVisible(true);
	m_pStarLayer->setPosition(ccp(0,  g_sfIconStarHeight / 2 + g_sfIconObjHeight * 2.5));

	//设置滚动参数
	m_fRollEndLength = m_pStarLayer->getPositionY();
	m_fRollSumLength = 0.0f;
	m_bIsRolling = true;
	m_eRollType = RollType_Star;

	//滚动图层
	schedule(schedule_selector(CStar97RollNode::starRollSchedule), 0.03f);
}

void CStar97RollNode::normalRollSchedule(float dt)
{
	m_pIconLayer1->setPositionY(m_pIconLayer1->getPositionY() - g_sfRollSpeed);
	m_pIconLayer2->setPositionY(m_pIconLayer2->getPositionY() - g_sfRollSpeed);

	//循环判断
	checkIconLayerPos();
}

void CStar97RollNode::finishRollSchedule(float dt)
{
	//计算转动速度
	float fSpeed = g_sfRollSpeed - m_fEndAcceler * m_nEndTime;
	m_fRollSumLength += fSpeed;
	++m_nEndTime;

	//设置音效停止
	if (m_fRollEndLength - m_fRollSumLength <= 5.0f)
	{
		m_bCanStopSound = true;
	}

	//设置位置
	if (m_fRollEndLength - m_fRollSumLength <= 0.1f)
	{
		m_pIconLayer1->setPositionY(m_fLayer1BeginPosY - m_fRollEndLength);
		m_pIconLayer2->setPositionY(m_fLayer2BeginPosY - m_fRollEndLength);

		unschedule(schedule_selector(CStar97RollNode::finishRollSchedule));
		m_bIsRolling = false;
		m_fEndAcceler = 0.0f;
		m_nEndTime = 0;
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

void CStar97RollNode::starRollSchedule(float dt)
{
	//计算滚动距离
	float fRollLength = g_sfRollSpeed;
	if (m_fRollSumLength + g_sfRollSpeed > m_fRollEndLength)
		fRollLength = m_fRollEndLength - m_fRollSumLength;

	//更新位置
	m_pIconLayer1->setPositionY(m_pIconLayer1->getPositionY() - fRollLength);
	m_pIconLayer2->setPositionY(m_pIconLayer2->getPositionY() - fRollLength);
	m_pStarLayer->setPositionY(m_pStarLayer->getPositionY() - fRollLength);

	//循环判断
	checkIconLayerPos();

	//停止滚动判断
	m_fRollSumLength += fRollLength;
	if (m_fRollSumLength - m_fRollEndLength >= -0.0001f && m_fRollSumLength - m_fRollEndLength <= 0.0001f)
	{
		unschedule(schedule_selector(CStar97RollNode::starRollSchedule));

		//设置明星图层动作
		CCAction * action1 = CCSequence::create(CCMoveBy::create(1.0f, ccp(0, -g_sfIconStarHeight)),
			CCDelayTime::create(0.5f), CCMoveBy::create(1.0f, ccp(0, -g_sfIconStarHeight)),
			CCCallFunc::create(this, callfunc_selector(CStar97RollNode::setEndMarkRool)), NULL);
		m_pStarLayer->runAction(action1);
	}

	return ;
}

//计算停止长度
float CStar97RollNode::computeEndRollLength(const int nEndCount /*= 4*/)
{
	//参数校验
	if (nEndCount < 4 || nEndCount > ICON_SPRITE_COUNT)
	{
		CC_ASSERT(false);
		return 0.0f;
	}

	//计算图标图层1、2，当前处在显示框中间位置的，相应图标索引
	CCPoint worldPos = this->convertToWorldSpace(CCPointMake(0, m_contentSize.height / 2));
	int nIndex1 = computeIndexAtWorldPos(worldPos, 1);
	int nIndex2 = computeIndexAtWorldPos(worldPos, 2);

	//计算上下2图标类型
	int nChangeType[2] = {0};
	{
		int nTempIndex = m_nTypePos[m_nResultTypeID][rand() % m_nTypeCount[m_nResultTypeID]];	//随机1个目标类型对应索引

		if (nTempIndex == Sort_Icon_Count - 1)
		{
			nChangeType[0] = g_vSortIconResList[0];
			nChangeType[1] = g_vSortIconResList[nTempIndex - 1];
		}
		else if (nTempIndex == 0)
		{
			nChangeType[0] = g_vSortIconResList[nTempIndex + 1];
			nChangeType[1] = g_vSortIconResList[Sort_Icon_Count - 1];
		}
		else
		{
			nChangeType[0] = g_vSortIconResList[nTempIndex + 1];
			nChangeType[1] = g_vSortIconResList[nTempIndex - 1];
		}
	}

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
			resetSpriteTexture(m_pIconSprite1[nDestIndex], g_vObjCardResList[m_nResultTypeID]);
		}
		else
		{
			resetSpriteTexture(m_pIconSprite2[nDestIndex % ICON_SPRITE_COUNT], g_vObjCardResList[m_nResultTypeID]);
		}

		//替换上下2图标
		if (nDestIndex == 0)
		{
			resetSpriteTexture(m_pIconSprite1[nDestIndex + 1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite2[ICON_SPRITE_COUNT - 1], nChangeType[1]);
		}
		else if (nDestIndex < ICON_SPRITE_COUNT - 1)
		{
			resetSpriteTexture(m_pIconSprite1[nDestIndex + 1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite1[nDestIndex - 1], nChangeType[1]);
		}
		else if (nDestIndex == ICON_SPRITE_COUNT - 1)
		{
			resetSpriteTexture(m_pIconSprite2[0], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite1[nDestIndex - 1], nChangeType[1]);
		}
		else if (nDestIndex == ICON_SPRITE_COUNT)
		{
			resetSpriteTexture(m_pIconSprite2[1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite1[ICON_SPRITE_COUNT - 1], nChangeType[1]);
		}
		else if (nDestIndex > ICON_SPRITE_COUNT)
		{
			int nRealIndex = nDestIndex % ICON_SPRITE_COUNT;
			resetSpriteTexture(m_pIconSprite2[nRealIndex + 1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite2[nRealIndex - 1], nChangeType[1]);
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
			resetSpriteTexture(m_pIconSprite2[nDestIndex], g_vObjCardResList[m_nResultTypeID]);
		}
		else
		{
			resetSpriteTexture(m_pIconSprite1[nDestIndex % ICON_SPRITE_COUNT], g_vObjCardResList[m_nResultTypeID]);
		}

		//替换上下2图标
		if (nDestIndex == 0)
		{
			resetSpriteTexture(m_pIconSprite2[nDestIndex + 1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite1[ICON_SPRITE_COUNT - 1], nChangeType[1]);
		}
		else if (nDestIndex < ICON_SPRITE_COUNT - 1)
		{
			resetSpriteTexture(m_pIconSprite2[nDestIndex + 1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite2[nDestIndex - 1], nChangeType[1]);
		}
		else if (nDestIndex == ICON_SPRITE_COUNT - 1)
		{
			resetSpriteTexture(m_pIconSprite1[0], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite2[nDestIndex - 1], nChangeType[1]);
		}
		else if (nDestIndex == ICON_SPRITE_COUNT)
		{
			resetSpriteTexture(m_pIconSprite1[1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite2[ICON_SPRITE_COUNT - 1], nChangeType[1]);
		}
		else if (nDestIndex > ICON_SPRITE_COUNT)
		{
			int nRealIndex = nDestIndex % ICON_SPRITE_COUNT;
			resetSpriteTexture(m_pIconSprite1[nRealIndex + 1], nChangeType[0]);
			resetSpriteTexture(m_pIconSprite1[nRealIndex - 1], nChangeType[1]);
		}
	}
	else
	{
		CC_ASSERT(false);
		return 0.0f;
	}

	return fLength;
}

int CStar97RollNode::computeIndexAtWorldPos(const CCPoint & worldPos, int nLayerID)
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
		CC_ASSERT((worldPos.x - pos.x > -0.0001f) && (worldPos.x - pos.x < 0.0001f));

		//位置判断
		float fIcoHeight = 0.0f;
#ifndef ANDROID_GAME
		fIcoHeight =  g_sfIconObjHeight * g_fPcScale;
#else
		fIcoHeight = g_sfIconObjHeight;
#endif
		if ((pos.y <= worldPos.y) && (pos.y >= worldPos.y - fIcoHeight))
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

void CStar97RollNode::checkIconLayerPos()
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

void CStar97RollNode::setEndMarkRool()
{
	m_bIsRolling = false;
}

}
