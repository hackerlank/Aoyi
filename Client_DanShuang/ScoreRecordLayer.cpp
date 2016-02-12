#include "stdafx.h"
#include "ScoreRecordLayer.h"
#include <curl/curl.h>

#define EXCHANGE_TAG 1

CScoreRecordLayer::CScoreRecordLayer()
{
	m_pBk = NULL;
	m_ptableView=NULL;
	m_vScoreRecord.clear();
}

CScoreRecordLayer::~CScoreRecordLayer()
{

}

void CScoreRecordLayer::OnInit()
{
	EnableModalMode(true);
	CCSize csize = getContentSize();
	CCSize bkSize ;

	//背景
	CCSprite *pHistoryRecord = CCSprite::create("DanShuang/scorerecord.png");
	if(pHistoryRecord)
	{
		pHistoryRecord->setPosition(ccp(csize.width / 2, csize.height / 2));
		pHistoryRecord->setZOrder(1);
		m_pBk = pHistoryRecord;
		addChild(pHistoryRecord);

		bkSize = m_pBk->getContentSize();
	}

	CCSize tableSize = CCSizeMake(863,351);
	{
		CCTableView* tableView = CCTableView::create(this, tableSize);
		tableView->setDirection(kCCScrollViewDirectionVertical);
		tableView->setPosition(ccp(17, 81));
		tableView->setDelegate(this);
		tableView->setVerticalFillOrder(kCCTableViewFillBottomUp);
		tableView->setTag(EXCHANGE_TAG);
		m_pBk->addChild(tableView,1);
		m_ptableView = tableView;
		tableView->reloadData();

		m_vModalModeTouchChildren.push_back(tableView);
	}

	//分割线条
	CCSprite *pLine = CCSprite::create("DanShuang/scorerecord_line.png");
	if(pLine)
	{
		pLine->setPosition(ccp(bkSize.width/2,bkSize.height/2));
		m_pBk->addChild(pLine,3);
	}

	//等待动画
	CCSprite *pLoading = CCSprite::create("DanShuang/Loading.png");
	if(pLoading)
	{
		pLoading->setPosition(ccp(bkSize.width/2,bkSize.height/2));
		m_pBk->addChild(pLoading,4);
		m_pLoading = pLoading;
	}

	//关闭按钮
	do
	{
		CCLabelTTF *titleButton = CCLabelTTF::create("", "Arial", 30);
		if (NULL == titleButton)
		{
			ERROR_CHECK;
			break;
		}
		titleButton->setColor(ccc3(159, 168, 176));

		CCScale9Sprite *backgroundButton = CCScale9Sprite::create("DanShuang/close_1.png");
		if (!backgroundButton)
		{
			ERROR_CHECK;
			break;
		}

		CCControlButton* pButton = CCControlButton::create(titleButton, backgroundButton);
		if (NULL == pButton)
		{
			ERROR_CHECK;
			break;
		}

		pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CScoreRecordLayer::touchCancel), CCControlEventTouchDown);

		pButton->setContentSize(CCSizeMake(157.f, 85.f));
		pButton->setPosition(ccp(csize.width / 2 + 410, csize.height / 2 + 222));
		pButton->setAdjustBackgroundImage(false);
		pButton->setZoomOnTouchDown(false);
		pButton->setZOrder(2);
		addChild(pButton);
		m_vModalModeChildren.push_back(pButton);

	}while(false);

}

void CScoreRecordLayer::OnLayerIn(float fDuration)
{
	//重置tableview
	m_vScoreRecord.clear();
	for(int i=0; i<10; i++)
	{
		tagLocalScoreRecord Record;
		Record.bvisible=false;
		m_vScoreRecord.push_back(Record);
	}

	m_ptableView->reloadData();
	m_ptableView->setContentOffset(m_ptableView->minContainerOffset());

	if (m_pLoading)
	{
		m_pLoading->setVisible(true);
		m_pLoading->runAction(CCRepeatForever::create(
			CCRotateBy::create(1.2f, 360.f)));
	}

	//RequestWeb();

	setTouchEnabled(true);
	setKeypadEnabled(true);

	CGameLayer::OnLayerIn(NULL,eLayerAni_Null,fDuration);
}

void CScoreRecordLayer::OnLayerOut(float fDuration)
{
	setTouchEnabled(false);
	setKeypadEnabled(false);

	CGameLayer::OnLayerOut(NULL,eLayerAni_Null,fDuration);
}

void CScoreRecordLayer::OnLayerIn(CGameLayer* pInLayer, ELayerAni enAniType, float fDuration)
{
	//重置tableview
	m_vScoreRecord.clear();
	for(int i=0; i<10; i++)
	{
		tagLocalScoreRecord Record;
		Record.bvisible=false;
		m_vScoreRecord.insert(m_vScoreRecord.begin(),Record);
	}

	m_ptableView->reloadData();
	m_ptableView->setContentOffset(m_ptableView->minContainerOffset());
	
	if (m_pLoading)
	{
		m_pLoading->setVisible(true);
		m_pLoading->runAction(CCRepeatForever::create(
			CCRotateBy::create(1.2f, 360.f)));
	}

	RequestWeb();

	setTouchEnabled(true);
	setKeypadEnabled(true);

	CGameLayer::OnLayerIn(pInLayer, enAniType, fDuration);
}

void CScoreRecordLayer::OnLayerOut(CGameLayer* pInLayer, ELayerAni enAniType, float fDuration)
{
	setTouchEnabled(false);
	setKeypadEnabled(false);

	CGameLayer::OnLayerOut(pInLayer, enAniType, fDuration);
}

void CScoreRecordLayer::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell)
{

}

cocos2d::CCSize CScoreRecordLayer::cellSizeForTable(cocos2d::extension::CCTableView *table)
{
	if (NULL == table)
	{
		ERROR_CHECK;
		return CCSizeZero;
	}

	CCSize cellSize = CCSizeMake(863,39);

	return cellSize;
}

cocos2d::extension::CCTableViewCell* CScoreRecordLayer::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
	if (NULL == table)
	{
		ERROR_CHECK;
		return NULL;
	}
	int nTag = table->getTag();
	CCSize cellsize = cellSizeForTable(table);
	CCSize szViewSize = table->getViewSize();

	if(idx==m_vScoreRecord.size())
	{
		CCTableViewCell *pCell = new CCTableViewCell();
		pCell->autorelease();
		pCell->setAnchorPoint(CCPointZero);
		return pCell;
	}

	//好友列表
	for (int i=0;i<(int)m_vScoreRecord.size();i++)
	{
		tagLocalScoreRecord& ExChange = m_vScoreRecord[i];

		if (i != (int)idx)
		{
			continue;
		}
		CCTableViewCell *pCell =ExChange.pCell;
		if(pCell)
		{
			return pCell;
		}

		pCell = new CCTableViewCell();
		pCell->autorelease();
		pCell->setAnchorPoint(CCPointZero);

		//------------------------------------------------------
		//背景
		CCSprite* pSprite = NULL;
		{
			pSprite = CCSprite::create("DanShuang/scorerecordlist_bk.png");

			if(!pSprite)
			{
				ERROR_CHECK;
				return pCell;
			}
			//pSprite->setAnchorPoint(CCPointZero);
			pSprite->setPosition(ccp(cellsize.width/2, cellsize.height/2));

			if(i%2==1)
			{
				CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(18,113,144,255));
				pLayerColor->setPosition(CCPointZero);
				pLayerColor->setContentSize(CCSizeMake(cellsize.width,cellsize.height));
				pSprite->addChild(pLayerColor);
			}
		}
		if(ExChange.bvisible)
		{
			{
				//时间
				CCLabelTTF *pTitle=CCLabelTTF::create(ExChange.sTime.c_str(), FONT_NAME, 22);
				pTitle->setDimensions(CCSize(215,39));
				pTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
				pTitle->setAnchorPoint(ccp(0.f, 0.5f));
				pTitle->setColor(ccc3(255,255,255));
				pTitle->setPosition(ccp(0,cellsize.height/2));
				pSprite->addChild(pTitle);
			}

			{
				//总分
				char szBuffer[33];
				sprintf(szBuffer,"%ld",ExChange.nAllScore);
				CCLabelTTF *pTitle=CCLabelTTF::create(szBuffer, FONT_NAME, 30);
				pTitle->setDimensions(CCSize(215,39));
				pTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
				pTitle->setAnchorPoint(ccp(0.f, 0.5f));
				pTitle->setColor(ccc3(255,255,255));
				pTitle->setPosition(ccp(232-15, cellsize.height/2));
				pSprite->addChild(pTitle);
			}

			{
				//变化分数
				char szBuffer[33];
				sprintf(szBuffer,"%ld",ExChange.nChangeScore);
				CCLabelTTF *pTitle=CCLabelTTF::create(szBuffer, FONT_NAME, 30);
				pTitle->setDimensions(CCSize(215,39));
				pTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
				pTitle->setAnchorPoint(ccp(0.f, 0.5f));
				pTitle->setColor(ccc3(255,255,0));
				pTitle->setPosition(ccp(450-15, cellsize.height/2));
				pSprite->addChild(pTitle);
			}

			{
				//操作者
				CCLabelTTF *pTitle=CCLabelTTF::create(ExChange.sUser.c_str(), FONT_NAME, 30);
				pTitle->setDimensions(CCSize(215,39));
				pTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
				pTitle->setAnchorPoint(ccp(0.f, 0.5f));
				pTitle->setColor(ccc3(255,255,0));
				pTitle->setPosition(ccp(667-15, cellsize.height/2));
				pSprite->addChild(pTitle);
			}

		}

		pCell->setTag(i);
		pCell->addChild(pSprite);
		ExChange.pCell=pCell;
		return pCell;
	}


	return NULL;
}

unsigned int CScoreRecordLayer::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
	return m_vScoreRecord.size();
}

void CScoreRecordLayer::touchCancel(CCObject *sender, CCControlEvent controlEvent)
{
	Hide();
}

void CScoreRecordLayer::Hide()
{
	setTouchEnabled(false);
	setKeypadEnabled(false);

	OnLayerOut(NULL,eLayerAni_DownIn,0.5f);
}

void CScoreRecordLayer::RequestWeb()
{
	std::string retstr;
	string url = "http://www.jcbygame.net/BankInfo.aspx";
	char szUrl[256];
	sprintf(szUrl,"?uid=%d",m_pGameDirector->m_nMeUserID);
	retstr += url;
	retstr += szUrl;

	//m_pResManager->GetConfigHelper()->m_vHttpMsgs.clear();
	//m_pGameDirector->m_nHttpMsgIndex = 0;
	CCLOG("RequestWeb:: %s",retstr.c_str());
	if (retstr.length() > 0)
	{
		CCHttpRequest* request = new CCHttpRequest();
		request->setUrl(retstr.c_str());
		request->setRequestType(CCHttpRequest::kHttpGet);
		request->setResponseCallback(this, callfuncND_selector(CScoreRecordLayer::onHttpRequestCompleted));
		CCHttpClient::getInstance()->send(request);
		request->release();
	}

	////////std::string szFile="DanShuang/record.xml";/*CCFileUtils::sharedFileUtils()->getWriteablePath()+xmlfile;*/
	////////const char* _path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(szFile.c_str()); 
	////////TiXmlDocument* xmldoc = new TiXmlDocument(_path);
	////////bool bLoad = xmldoc->LoadFile();

	////////if (!bLoad)
	////////{
	////////	printf("load xml (%s) fail\r\n", _path);
	////////	delete xmldoc;
	////////	return ;
	////////}
	////////if (NULL == xmldoc)
	////////{
	////////	ERROR_CHECK;
	////////	return ;
	////////}

	//////////m_sEmailName=xmlfile;

	////////ParseScoreRecordXML(xmldoc);

	////////delete xmldoc;
}

void CScoreRecordLayer::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
	if (m_pLoading)
	{
		m_pLoading->stopAllActions();
		m_pLoading->setVisible(false);
		//m_pLoading->setRotation(0.f);
	}

	char sUT8Buff[200];
	sUT8Buff[0] = 0;
	const char* sFormat = "http error %d";

	CCHttpResponse *response = (CCHttpResponse*)data;

	if (!response)
	{
		//sprintf(sUT8Buff, sFormat, 0);
		//m_pGameDirector->m_nHttpNotifyID = m_pGameDirector->ShowNotify(sUT8Buff, eNotifyFlag_OK, getParent(), m_pGameDirector->m_nHttpNotifyID, false);
		return;
	}

	int statusCode = response->getResponseCode();
	if (200 != statusCode)
	{
		//sprintf(sUT8Buff, sFormat, statusCode);
		//m_pGameDirector->m_nHttpNotifyID = m_pGameDirector->ShowNotify(sUT8Buff, eNotifyFlag_OK, getParent(), m_pGameDirector->m_nHttpNotifyID, false);
		return;
	}

	if (!response->isSucceed())
	{
		//sprintf(sUT8Buff, sFormat, 1);
		//m_pGameDirector->m_nHttpNotifyID = m_pGameDirector->ShowNotify(sUT8Buff, eNotifyFlag_OK, getParent(), m_pGameDirector->m_nHttpNotifyID, false);
		//CCLog("http error: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char>* buffer = response->getResponseData();

	TiXmlDocument resXML;
	resXML.Parse(buffer->data());
	if (resXML.Error())
	{
		sprintf(sUT8Buff, sFormat, 2);
		m_pGameDirector->m_nHttpNotifyID = m_pGameDirector->ShowNotify(sUT8Buff, eNotifyFlag_OK, getParent(), m_pGameDirector->m_nHttpNotifyID, false);
	}

	//m_vScoreRecord.clear();

	ParseScoreRecordXML(&resXML);
	//m_pResManager->GetConfigHelper()->ParseHttpSystemMessageXML(&resXML);
	CCLog("m_vScoreRecord.size:%d",m_vScoreRecord.size());

	////重置tableview
	//for(int i=0; i<10-m_vScoreRecord.size(); i++)
	//{
	//	tagLocalScoreRecord Record;
	//	Record.bvisible=false;
	//	m_vScoreRecord.push_back(Record);
	//}

	m_ptableView->reloadData();
	m_ptableView->setContentOffset(m_ptableView->minContainerOffset());
	

}

void CScoreRecordLayer::ParseScoreRecordXML(TiXmlDocument* xmldoc)
{
	if(!xmldoc)
	{
		ERROR_CHECK;
		return;
	}

	TiXmlElement* pelement = xmldoc->RootElement();
	std::string nodename = pelement->Value();

	if (0 != nodename.compare("scorerecord"))
	{
		ERROR_CHECK;
		return ;
	}

	TiXmlElement* pChildEle = pelement->FirstChildElement();

	while(NULL != pChildEle)
	{
		nodename = pChildEle->Value();
		TiXmlElement* pSpriteElement = pChildEle->FirstChildElement();
		if (0 == nodename.compare("record"))
		{
			ParseScoreRecordXML(pSpriteElement, true);
		}

		pChildEle = pChildEle->NextSiblingElement();
	}
}

void CScoreRecordLayer::ParseScoreRecordXML(TiXmlElement* pEnumElement, bool bAllNext)
{
	while(NULL != pEnumElement)
	{
		TiXmlElement* pCurrentElement = pEnumElement;
		if (bAllNext)
		{
			pEnumElement = pEnumElement->NextSiblingElement();
		}
		else
		{
			pEnumElement = NULL;
		}

		std::string namevalue = pCurrentElement->Value();
		const char* pvalue = NULL;

		if (0 != namevalue.compare("item"))
		{
			continue;
		}

		//date
		pvalue = pCurrentElement->Attribute("date");
		if (NULL == pvalue)
		{
			continue;
		}

		tagLocalScoreRecord ScoreRecord;
		ScoreRecord.sTime = pvalue;

		//score
		pvalue = pCurrentElement->Attribute("score");
		if (NULL != pvalue)
		{
			ScoreRecord.nAllScore = atoi(pvalue);
		}

		//changescore
		pvalue = pCurrentElement->Attribute("changescore");
		if (NULL != pvalue)
		{
			ScoreRecord.nChangeScore = atoi(pvalue);
		}

		//control
		pvalue = pCurrentElement->Attribute("control");
		if (NULL != pvalue)
		{
			ScoreRecord.sUser = pvalue;
		}

		m_vScoreRecord.push_back(ScoreRecord);
	}
}