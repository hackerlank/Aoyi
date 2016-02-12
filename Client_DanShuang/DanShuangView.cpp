#include "DanShuangView.h"
#include "DanShuang.h"
#include "SimpleAudioEngine.h"

///////////////////////////////////////////////////////////////////////////
static const float g_fRecordDistance = 70.0f;	//��ʷ��¼���

//��������
CDanShuangGameView * CDanShuangGameView::create(CDanShuangGame *pSicBoGame)
{
	CDanShuangGameView *pDanShuangGameView = new CDanShuangGameView(pSicBoGame);
	if (pDanShuangGameView && pDanShuangGameView->init())
	{
		pDanShuangGameView->autorelease();
		return pDanShuangGameView;
	}
	CC_SAFE_DELETE(pDanShuangGameView);
	return NULL;
}

//���캯��
CDanShuangGameView::CDanShuangGameView(CDanShuangGame *pSicBoGame) : m_pDanShuangGame(pSicBoGame)
{
	//��ע��Ϣ
	memset(m_lMeScore, 0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0, sizeof(m_lAllScore));

	//������Ϣ
	m_enNowOpenType = CT_NULL;

	//��ʷ��Ϣ
	m_lMeStatisticScore = 0;
	memset(m_enHistoryRecord, 0, sizeof(m_enHistoryRecord));
	m_cbHistoryRecordCount = 0;
	m_ApplyUserArray.clear();

	//״̬����
	m_lCurrentJetton = 0;

	//ׯ����Ϣ
	m_wBankerUser = INVALID_CHAIR;		
	m_wBankerTime = 0;
	m_lBankerScore = 0;	
	m_lBankerWinScore = 0;
	m_bEnableSysBanker = true;

	//���ֳɼ�
	m_lMeCurGameScore = 0;	
	m_lMeCurGameReturnScore = 0;
	m_lBankerCurGameScore = 0;
	m_lGameRevenue = 0;
}

//��������
CDanShuangGameView::~CDanShuangGameView()
{
}

//��ʼ��
bool CDanShuangGameView::init()
{
	if(!CCLayer::init())
		return false;

	m_cbGameTime = 0;
	m_screenSize = CCDirector::sharedDirector()->getWinSize();

	//�������
	float fScreenMidX = m_screenSize.width / 2;	//��Ļ���е�����
	float fScreenMidY = m_screenSize.height / 2;	//��Ļ���е�����

	//����
	CCSprite * pBackView = CCSprite::create("DanShuang/tableback.jpg");
	pBackView->setPosition(ccp(fScreenMidX, fScreenMidY));
	addChild(pBackView);

	//ׯ����Ϣͼ��
	m_pLayerBankerInfo = CCLayer::create();
	CCSprite *BankerInfomation = CCSprite::create("DanShuang/bankerinformation.png");
	BankerInfomation->setPosition(CCPointMake(fScreenMidX, fScreenMidY));
	m_pLayerBankerInfo->addChild(BankerInfomation);

	CCMenuItemImage *pCloseBankerInfo = CCMenuItemImage::create("DanShuang/close_1.png",
		"DanShuang/close_2.png", "DanShuang/close_3.png", this, menu_selector(CDanShuangGameView::OnClose));
	pCloseBankerInfo->setPosition(CCPointMake(fScreenMidX + 265, fScreenMidY + 122));
	CCMenu *pMenuBankerInfo = CCMenu::create(pCloseBankerInfo, NULL);
	pMenuBankerInfo->setPosition(CCPointZero);
	m_pLayerBankerInfo->addChild(pMenuBankerInfo);

	string str;
	str = CStringHelper::GB2312ToUTF8("ϵͳ��ׯ");
	m_pBankerUser = CCLabelTTF::create(str.c_str(),"����",26);
	m_pBankerUser->setColor(ccc3(255,214,43));
	m_pBankerUser->setAnchorPoint(CCPointZero);
	m_pBankerUser->setPosition(CCPointMake(fScreenMidX - 175, fScreenMidY + 29));
	m_pLayerBankerInfo->addChild(m_pBankerUser);

	m_pBankerScore = CCLabelAtlas::create("0123456789-","DanShuang/userscorenum_s.png", 18, 27, '0');
	m_pBankerScore->setAnchorPoint(CCPointZero);
	m_pBankerScore->setPosition(CCPointMake(fScreenMidX - 175, fScreenMidY - 16));
	m_pBankerScore->setString("0");
	m_pLayerBankerInfo->addChild(m_pBankerScore);

	m_pBankerTime = CCLabelAtlas::create("0123456789-","DanShuang/userscorenum_s.png", 18, 27, '0');
	m_pBankerTime->setAnchorPoint(CCPointZero);
	m_pBankerTime->setPosition(CCPointMake(fScreenMidX - 175, fScreenMidY - 63));
	m_pBankerTime->setString("0");
	m_pLayerBankerInfo->addChild(m_pBankerTime);

	m_pBankerMark = CCLabelAtlas::create("0123456789-","DanShuang/userscorenum_s.png", 18, 27, '0');
	m_pBankerMark->setAnchorPoint(CCPointZero);
	m_pBankerMark->setPosition(CCPointMake(fScreenMidX - 175, fScreenMidY - 111));
	m_pBankerMark->setString("0");
	m_pLayerBankerInfo->addChild(m_pBankerMark);

	m_pLayerBankerInfo->setVisible(false);
	addChild(m_pLayerBankerInfo,10);

	//��ׯ�б�ͼ��
	m_pLayerBankerList = CCLayer::create();
	CCSprite *pApplyBankerList = CCSprite::create("DanShuang/applybankerlist.png");
	pApplyBankerList->setPosition(CCPointMake(fScreenMidX, fScreenMidY));
	m_pLayerBankerList->addChild(pApplyBankerList);

	CCMenuItemImage *pCloseBankerList = CCMenuItemImage::create("DanShuang/close_1.png",
		"DanShuang/close_2.png", "DanShuang/close_3.png", this, menu_selector(CDanShuangGameView::OnClose));
	pCloseBankerList->setPosition(CCPointMake(fScreenMidX + 265, fScreenMidY + 122));
	CCMenu *pMenuBankerList = CCMenu::create(pCloseBankerList,NULL);
	pMenuBankerList->setPosition(CCPointZero);
	m_pLayerBankerList->addChild(pMenuBankerList);

	for (int i=0; i<SHOW_APPLY_BANKER; ++i)
	{
		m_pLabelApplyBankerUser[i] = CCLabelTTF::create("","����",26);
		m_pLabelApplyBankerUser[i]->setColor(ccc3(255,214,43));
		m_pLabelApplyBankerUser[i]->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pLabelApplyBankerUser[i]->setPosition(ccp(fScreenMidX, fScreenMidY + 66 - 48 * i));
		m_pLayerBankerList->addChild(m_pLabelApplyBankerUser[i]);
	}

	m_pLayerBankerList->setVisible(false);
	addChild(m_pLayerBankerList,10);

	//������¼ͼ��
	//////////m_pLayerScoreRecord = CCLayer::create();
	//////////CCSprite *pHistoryRecord = CCSprite::create("DanShuang/scorerecord.png");
	//////////pHistoryRecord->setPosition(CCPointMake(fScreenMidX, fScreenMidY));
	//////////m_pLayerScoreRecord->addChild(pHistoryRecord);

	//////////CCMenuItemImage *pCloseHistory = CCMenuItemImage::create("DanShuang/close_1.png",
	//////////	"DanShuang/close_2.png","DanShuang/close_3.png", this, menu_selector(CDanShuangGameView::OnClose));
	//////////pCloseHistory->setPosition(CCPointMake(fScreenMidX + 414, fScreenMidY + 216));
	//////////CCMenu *pMenuHistory = CCMenu::create(pCloseHistory,NULL);
	//////////pMenuHistory->setPosition(CCPointZero);
	//////////m_pLayerScoreRecord->addChild(pMenuHistory);

	//////////for (int i=0; i<11; ++i)
	//////////{
	//////////	//m_pLabelStatistics[i] = CCLabelTTF::create("","����",30);
	//////////	//m_pLabelStatistics[i]->setColor(ccc3(255,255,0));
	//////////	//m_pLabelStatistics[i]->setPosition(CCPointMake(fScreenMidX - 540,fScreenMidY + 215-54*i));
	//////////	//m_pLabelStatistics[i]->setAnchorPoint(CCPointZero);
	//////////	//m_pLabelResult[i] = CCLabelTTF::create("","����",30);
	//////////	//m_pLabelResult[i]->setColor(ccc3(255,255,0));
	//////////	//m_pLabelResult[i]->setPosition(CCPointMake(fScreenMidX - 170,fScreenMidY + 215-54*i));
	//////////	//m_pLabelResult[i]->setAnchorPoint(CCPointZero);
	//////////	//m_pLabelRanking[i] = CCLabelTTF::create("","����",30);
	//////////	//m_pLabelRanking[i]->setColor(ccc3(255,255,0));
	//////////	//m_pLabelRanking[i]->setPosition(CCPointMake(fScreenMidX + 200,fScreenMidY + 215-54*i));
	//////////	//m_pLabelRanking[i]->setAnchorPoint(CCPointZero);
	//////////	//m_pLayerHistory->addChild(m_pLabelStatistics[i]);
	//////////	//m_pLayerHistory->addChild(m_pLabelResult[i]);
	//////////	//m_pLayerHistory->addChild(m_pLabelRanking[i]);
	//////////}
	//////////m_pLayerScoreRecord->setVisible(false);
	//////////addChild(m_pLayerScoreRecord,10);

	//����
	{
		CScoreRecordLayer* pSubLayer = new CScoreRecordLayer();
		pSubLayer->SetTypeID(eLayerType_ScoreRecord);
		pSubLayer->autorelease();
		pSubLayer->SetGameDirector(m_pDanShuangGame->m_pDirector);
		pSubLayer->SetResManager(m_pDanShuangGame->m_pDirector->m_pResManager);
		pSubLayer->setContentSize(CCSizeMake(WINDOW_WIDTH, CONTENTS_HEIGHT));
		pSubLayer->SetOrginPos(CCPointZero);
		pSubLayer->setZOrder(10);
		pSubLayer->OnLayerOut(NULL, eLayerAni_Null);
		m_pLayerScoreRecord = pSubLayer;
		addChild(pSubLayer);
	}

	//��ʷ��¼ͼ��
	m_pLayerHistoryRecord = CCLayer::create();
	addChild(m_pLayerHistoryRecord, 4);

	//��ʷ��¼���ֲ�
	{
		m_pRecordClipNode = CCClippingNode::create();

		m_pRecordClipNode->setContentSize(CCSizeMake(700, 45));
		m_pRecordClipNode->setAnchorPoint(ccp(0, 0));
		m_pRecordClipNode->setPosition(ccp(390, 602));

		CCDrawNode *stencil = CCDrawNode::create();	//�ü�ģ��
		CCPoint rectangle[4];
		rectangle[0] = ccp(0, 0);
		rectangle[1] = ccp(m_pRecordClipNode->getContentSize().width, 0);
		rectangle[2] = ccp(m_pRecordClipNode->getContentSize().width, m_pRecordClipNode->getContentSize().height);
		rectangle[3] = ccp(0, m_pRecordClipNode->getContentSize().height);
		ccColor4F white = {1, 1, 1, 1};
		stencil->drawPolygon(rectangle, 4, white, 1, white);
		m_pRecordClipNode->setStencil(stencil);
		m_pRecordClipNode->setInverted(false);
	}
	m_pLayerHistoryRecord->addChild(m_pRecordClipNode, 1);

	//Ӳ�Ҷ���ͼ��
	m_pLayerCoinAnimation = CCLayer::create();
	addChild(m_pLayerCoinAnimation, 5);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("DanShuang/animation_frame.plist", "DanShuang/animation_frame.png");
	CCAnimationCache::sharedAnimationCache()->addAnimationsWithFile("DanShuang/all_animation.plist");
	m_pSpriteHand = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hand_1.png"));
	m_pSpriteHand->setPosition(ccp(640, 317));
	m_pSpriteCoin = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("coin_1.png"));
	m_pSpriteCoin->setPosition(ccp(704, 402));
	m_pLayerCoinAnimation->addChild(m_pSpriteHand, 10);
	m_pLayerCoinAnimation->addChild(m_pSpriteCoin, 20);
	m_pLayerCoinAnimation->setVisible(false);
	m_wCoinAnimateTime = 0;

	//�����þ��鼯����
	m_pJettonBatchNode = GameJettonBatchNode::create("DanShuang/jetton.png");
	addChild(m_pJettonBatchNode,1);
	CCSpriteFrameCache * pShareFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pShareFrameCache->addSpriteFramesWithFile("DanShuang/jetton.plist", "DanShuang/jetton.png");

	//����ͼ��
	m_pLayerFlashWinner=CCLayer::create();
	addChild(m_pLayerFlashWinner,3);

	//�˳�ͼ��
	m_pLayerQuit=CCLayer::create();

	CCSprite * pBlack1 = CCSprite::create("DanShuang/black.png");
	pBlack1->setPosition(CCPointMake(fScreenMidX, fScreenMidY));
	m_pLayerQuit->addChild(pBlack1);

	CCSprite * pQuitBackSprite = CCSprite::create("DanShuang/quitback.png");
	pQuitBackSprite->setPosition(CCPointMake(fScreenMidX, fScreenMidY));
	m_pLayerQuit->addChild(pQuitBackSprite);

	CCLabelTTF * pQuitWarnInfo = CCLabelTTF::create(CStringHelper::GB2312ToUTF8(
		"������������Ϸ�У��˳���۳����ķ�������ȷ��Ҫ�˳���").c_str(), "����", 40,
		CCSize(700, 100), kCCTextAlignmentCenter);
	pQuitWarnInfo->setAnchorPoint(ccp(0.5, 1));
	pQuitWarnInfo->setPosition(ccp(fScreenMidX, fScreenMidY + 80));
	m_pLayerQuit->addChild(pQuitWarnInfo);

	CCMenuItemImage * pSubmitQuit = CCMenuItemImage::create("DanShuang/submit.png",
		"DanShuang/submit_c.png", this, menu_selector(CDanShuangGameView::OnSubmitQuit));
	pSubmitQuit->setPosition(CCPointMake(fScreenMidX - 160, fScreenMidY - 80));
	CCMenuItemImage * pCancelQuit = CCMenuItemImage::create("DanShuang/cancel.png",
		"DanShuang/cancel_c.png", this, menu_selector(CDanShuangGameView::OnCancelQuit));
	pCancelQuit->setPosition(CCPointMake(fScreenMidX + 160, fScreenMidY - 80));
	CCMenu *pMenuQuit = CCMenu::create(pSubmitQuit, pCancelQuit, NULL);
	pMenuQuit->setPosition(CCPointZero);
	m_pLayerQuit->addChild(pMenuQuit);

	m_pLayerQuit->setVisible(false);
	addChild(m_pLayerQuit, 15);

	//��Ϸ״̬
	CCTextureCache * pShareTextCache = CCTextureCache::sharedTextureCache();
	pShareTextCache->addImage("DanShuang/freetimestatus.png");
	pShareTextCache->addImage("DanShuang/cointimestatus.png");
	pShareTextCache->addImage("DanShuang/jettontimestatus.png");
	pShareTextCache->addImage("DanShuang/opentimestatus.png");

	m_pSpriteGameStatus = CCSprite::createWithTexture(pShareTextCache->textureForKey("DanShuang/freetimestatus.png"));
	m_pSpriteGameStatus->setAnchorPoint(CCPointZero);
	m_pSpriteGameStatus->setPosition(CCPointMake(fScreenMidX - 95, fScreenMidY + 305));
	addChild(m_pSpriteGameStatus, 4);

	m_pLabelLeaveTime = CCLabelAtlas::create("0123456789", "DanShuang/timenum.png", 23,31, '0');
	m_pLabelLeaveTime->setAnchorPoint(CCPointZero);
	m_pLabelLeaveTime->setPosition(CCPointMake(fScreenMidX + 45, fScreenMidY + 309));
	m_pLabelLeaveTime->setString("0");
	addChild(m_pLabelLeaveTime, 4);

	//�����Ϣ
	CCSprite *pScore = CCSprite::create("DanShuang/score.png");
	pScore->setPosition(CCPointMake(fScreenMidX - 294, fScreenMidY - 290));
	addChild(pScore, 4);

	CCSprite *pResult = CCSprite::create("DanShuang/result.png");
	pResult->setPosition(CCPointMake(fScreenMidX, fScreenMidY - 290));
	addChild(pResult, 4);

	CCSprite *pName = CCSprite::create("DanShuang/name.png");
	pName->setPosition(CCPointMake(fScreenMidX + 294, fScreenMidY - 290));
	addChild(pName, 4);

	//��ť
	//ׯ����Ϣ
	m_pBankerInfoButton = CCMenuItemImage::create("DanShuang/bankerinfo_1.png",
		"DanShuang/bankerinfo_2.png", this, menu_selector(CDanShuangGameView::OnBankerInfo));
	m_pBankerInfoButton->setPosition(CCPointMake(fScreenMidX - 560,fScreenMidY + 173));

	//��ׯ�б�
	m_pBankerList = CCMenuItemImage::create("DanShuang/bankerlist_1.png",
		"DanShuang/bankerlist_2.png", this, menu_selector(CDanShuangGameView::OnBankerList));
	m_pBankerList->setPosition(CCPointMake(fScreenMidX - 560,fScreenMidY + 50));

	//��ׯ
	m_pApplyBanker = CCMenuItemImage::create("DanShuang/applybanker_1.png",
		"DanShuang/applybanker_2.png","DanShuang/applybanker_3.png",this,
		menu_selector(CDanShuangGameView::OnApplyBanker));
	m_pApplyBanker->setPosition(CCPointMake(fScreenMidX - 560, fScreenMidY - 108));
	m_pApplyBanker->setEnabled(false);

	//��ׯ
	m_pCancelBanker = CCMenuItemImage::create("DanShuang/cancelbanker_1.png",
		"DanShuang/cancelbanker_2.png","DanShuang/cancelbanker_3.png",this,
		menu_selector(CDanShuangGameView::OnCancelBanker));
	m_pCancelBanker->setPosition(CCPointMake(fScreenMidX - 560, fScreenMidY - 108));
	m_pCancelBanker->setVisible(false);
	m_pCancelBanker->setEnabled(false);

	//������¼
	m_pScoreRecord = CCMenuItemImage::create("DanShuang/scoreresult_1.png",
		"DanShuang/scoreresult_2.png", this, menu_selector(CDanShuangGameView::OnScoreRecord));
	m_pScoreRecord->setPosition(CCPointMake(fScreenMidX -455, fScreenMidY - 225));

	//ȡ����ע
	m_pClearJetton = CCMenuItemImage::create("DanShuang/clear_1.png",
		"DanShuang/clear_2.png","DanShuang/clear_3.png",this,
		menu_selector(CDanShuangGameView::OnClearJetton));
	m_pClearJetton->setPosition(CCPointMake(fScreenMidX + 485, fScreenMidY - 195));

	//�˳�
	m_pQuit = CCMenuItemImage::create("DanShuang/quit_1.png",
		"DanShuang/quit_2.png", this, menu_selector(CDanShuangGameView::OnQuit));
	m_pQuit->setAnchorPoint(CCPointZero);
	m_pQuit->setPosition(CCPointMake(fScreenMidX + 485, fScreenMidY + 70));

	//����
	CCSprite * pJettonSprite0_nor = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton0_1.png"));
	CCSprite * pJettonSprite0_sel = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton0_2.png"));
	CCSprite * pJettonSprite0_dis = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton0_3.png"));
	m_pJetton0 = CJettonMenuSprite::create(pJettonSprite0_nor, pJettonSprite0_sel, pJettonSprite0_dis,
		this, menu_selector(CDanShuangGameView::OnJettonButton100));
	m_pJetton0->setPosition(CCPointMake(fScreenMidX + 560, fScreenMidY -18));
	m_pJetton0->setVisible(true);
	m_lCurrentJetton = 100;

	CCSprite * pJettonSprite1_nor = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton1_1.png"));
	CCSprite * pJettonSprite1_sel = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton1_2.png"));
	CCSprite * pJettonSprite1_dis = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton1_3.png"));
	m_pJetton1 = CJettonMenuSprite::create(pJettonSprite1_nor, pJettonSprite1_sel, pJettonSprite1_dis,
		this, menu_selector(CDanShuangGameView::OnJettonButton1000));
	m_pJetton1->setPosition(CCPointMake(fScreenMidX + 560, fScreenMidY -18));
	m_pJetton1->setVisible(false);

	CCSprite * pJettonSprite2_nor = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton2_1.png"));
	CCSprite * pJettonSprite2_sel = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton2_2.png"));
	CCSprite * pJettonSprite2_dis = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton2_3.png"));
	m_pJetton2 = CJettonMenuSprite::create(pJettonSprite2_nor, pJettonSprite2_sel, pJettonSprite2_dis,
		this, menu_selector(CDanShuangGameView::OnJettonButton10000));
	m_pJetton2->setPosition(CCPointMake(fScreenMidX + 560, fScreenMidY -18));
	m_pJetton2->setVisible(false);

	CCSprite * pJettonSprite3_nor = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton3_1.png"));
	CCSprite * pJettonSprite3_sel = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton3_2.png"));
	CCSprite * pJettonSprite3_dis = CCSprite::createWithSpriteFrame(pShareFrameCache->spriteFrameByName("jetton3_3.png"));
	m_pJetton3 = CJettonMenuSprite::create(pJettonSprite3_nor, pJettonSprite3_sel, pJettonSprite3_dis,
		this, menu_selector(CDanShuangGameView::OnJettonButton100000));
	m_pJetton3->setPosition(CCPointMake(fScreenMidX + 560, fScreenMidY -18));
	m_pJetton3->setVisible(false);

	CCMenu * pMenu = CCMenu::create(m_pBankerInfoButton, m_pBankerList, m_pApplyBanker, m_pCancelBanker, m_pScoreRecord,
		m_pClearJetton, m_pQuit, m_pJetton0, m_pJetton1, m_pJetton2, m_pJetton3, NULL);
	pMenu->setPosition(CCPointZero);
	addChild(pMenu,4);

	//��ʾ
	m_pLabelMessage = CCLabelTTF::create("","����",50, CCSize(1000, 150), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	m_pLabelMessage->setColor(ccc3(255,255,128));
	m_pLabelMessage->setAnchorPoint(CCPointMake(0.5f,0.5f));
	m_pLabelMessage->setPosition(CCPointMake(fScreenMidX, fScreenMidY));
	addChild(m_pLabelMessage,20, 100);

	//λ����Ϣ
	for (BYTE i = 0; i < AREA_COUNT; ++i)
	{
		switch(i)
		{
		case 0:
			{
				m_rcAreaRange[i] = CCRectMake(200, 190, 435, 400);
				break;
			}
		case 1:
			{
				m_rcAreaRange[i] = CCRectMake(645, 190, 431, 400);
				break;
			}
		default:
			{
				CC_ASSERT(false);
				break;
			}
		}
		m_PointAreaMid[i].setPoint(m_rcAreaRange[i].getMidX(), m_rcAreaRange[i].getMidY());
	}

	{
		CCPoint PointRed_A(200, 190), PointRed_B(200, 248), PointRed_C(200, 300), PointRed_D(200, 345),
			PointRed_E(259, 190), PointRed_F(310, 190), PointRed_G(355, 190);

		m_RedAreaTriangle[0].point[0] = PointRed_A;
		m_RedAreaTriangle[0].point[1] = PointRed_D;
		m_RedAreaTriangle[0].point[2] = PointRed_E;
		m_RedAreaTriangle[1].point[0] = PointRed_A;
		m_RedAreaTriangle[1].point[1] = PointRed_C;
		m_RedAreaTriangle[1].point[2] = PointRed_F;
		m_RedAreaTriangle[2].point[0] = PointRed_A;
		m_RedAreaTriangle[2].point[1] = PointRed_B;
		m_RedAreaTriangle[2].point[2] = PointRed_G;

		CCPoint PointBlue_A(1076, 190), PointBlue_B(1076, 248), PointBlue_C(1076, 300),PointBlue_D(1076, 345),
			PointBlue_E(1014, 190), PointBlue_F(963, 190), PointBlue_G(918, 190);

		m_BlueAreaTriangle[0].point[0] = PointBlue_A;
		m_BlueAreaTriangle[0].point[1] = PointBlue_D;
		m_BlueAreaTriangle[0].point[2] = PointBlue_E;
		m_BlueAreaTriangle[1].point[0] = PointBlue_A;
		m_BlueAreaTriangle[1].point[1] = PointBlue_C;
		m_BlueAreaTriangle[1].point[2] = PointBlue_F;
		m_BlueAreaTriangle[2].point[0] = PointBlue_A;
		m_BlueAreaTriangle[2].point[1] = PointBlue_B;
		m_BlueAreaTriangle[2].point[2] = PointBlue_G;
	}

	//�û���Ϣ
	char szUserScore[256];
	sprintf(szUserScore,"%lld", m_pDanShuangGame->m_pDirector->GetMeData()->nUserScore);
	m_pLabelUserScore = CCLabelAtlas::create("0123456789-", "DanShuang/userscorenum_s.png", 18, 27, '0');
	m_pLabelUserScore->setAnchorPoint(ccp(0.0f, 0.5f));
	m_pLabelUserScore->setString(szUserScore);
	m_pLabelUserScore->setPosition(CCPointMake(fScreenMidX - 246,fScreenMidY - 290));
	m_pLabelUserMark = CCLabelAtlas::create("0123456789-", "DanShuang/userscorenum_s.png", 18, 27, '0');
	m_pLabelUserMark->setAnchorPoint(ccp(0.0f, 0.5f));
	m_pLabelUserMark->setString("0");
	m_pLabelUserMark->setPosition(CCPointMake(fScreenMidX + 49,fScreenMidY - 290));

	//string strUserName = CStringHelper::GB2312ToUTF8(m_pSicBoGame->m_pDirector->GetMeData()->sUserName.c_str());
	string strUserName = m_pDanShuangGame->m_pDirector->GetMeData()->sUserName;
	//std::wstring sUTF8 = CStringHelper::UTF8StringToUTF16(m_pSicBoGame->m_pDirector->GetMeData()->sUserName.c_str());

	m_pLabelUserName = CCLabelTTF::create(strUserName.c_str(),"����",30, CCSize(300, 90), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	m_pLabelUserName->setDimensions(CCSize(300,90));
	m_pLabelUserName->setAnchorPoint(ccp(0.0f, 1.0f));
	m_pLabelUserName->setColor(ccc3(255,214,43));
	m_pLabelUserName->setPosition(CCPointMake(fScreenMidX + 340, fScreenMidY - 276));

	addChild(m_pLabelUserScore);
	addChild(m_pLabelUserMark);
	addChild(m_pLabelUserName);
	
	//��ע����
	for (int i=0; i<AREA_COUNT; ++i)
	{
		m_pLabelMeTableScore[i] = CCLabelAtlas::create("0123456789","DanShuang/userscorenum.png",24,36,'0');
		m_pLabelAllTableScore[i] = CCLabelAtlas::create("0123456789","DanShuang/userscorenum.png",24,36,'0');
		m_pTableScoreBK[0][i] = CCSprite::create("DanShuang/tablescoreback.png");
		m_pTableScoreBK[0][i]->setVisible(false);
		m_pTableScoreBK[1][i] = CCSprite::create("DanShuang/tablescoreback.png");
		m_pTableScoreBK[1][i]->setVisible(false);

		m_pLabelMeTableScore[i]->setAnchorPoint(CCPointMake(0.5f,0.5f));
		m_pLabelAllTableScore[i]->setAnchorPoint(CCPointMake(0.5f,0.5f));

		m_pLabelMeTableScore[i]->setPosition(CCPointMake(m_PointAreaMid[i].x, m_PointAreaMid[i].y - 140));
		m_pLabelAllTableScore[i]->setPosition(CCPointMake(m_PointAreaMid[i].x, m_PointAreaMid[i].y + 140));
		m_pTableScoreBK[0][i]->setPosition(CCPointMake(m_PointAreaMid[i].x, m_PointAreaMid[i].y - 140));
		m_pTableScoreBK[1][i]->setPosition(CCPointMake(m_PointAreaMid[i].x, m_PointAreaMid[i].y + 140));

		m_pLabelMeTableScore[i]->setString("0");
		m_pLabelAllTableScore[i]->setString("0");
		m_pLabelMeTableScore[i]->setVisible(false);
		m_pLabelAllTableScore[i]->setVisible(false);

		addChild(m_pTableScoreBK[0][i],2);
		addChild(m_pTableScoreBK[1][i],2);
		addChild(m_pLabelMeTableScore[i],2);
		addChild(m_pLabelAllTableScore[i],2);
	}

	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	return true;
}

//�任����
void CDanShuangGameView::OnJettonButton100(CCObject *pSender)
{
	m_pJetton0->setVisible(false);
	m_pJetton1->setVisible(true);
	m_pJetton2->setVisible(false);
	m_pJetton3->setVisible(false);

	//���ñ���
	m_lCurrentJetton=1000L;
}
void CDanShuangGameView::OnJettonButton1000(CCObject *pSender)
{
	m_pJetton0->setVisible(false);
	m_pJetton1->setVisible(false);
	m_pJetton2->setVisible(true);
	m_pJetton3->setVisible(false);
	//���ñ���
	m_lCurrentJetton=10000L;
}
void CDanShuangGameView::OnJettonButton10000(CCObject *pSender)
{
	m_pJetton0->setVisible(false);
	m_pJetton1->setVisible(false);
	m_pJetton2->setVisible(false);
	m_pJetton3->setVisible(true);
	//���ñ���
	m_lCurrentJetton=100000L;
}
void CDanShuangGameView::OnJettonButton100000(CCObject *pSender)
{
	m_pJetton0->setVisible(true);
	m_pJetton1->setVisible(false);
	m_pJetton2->setVisible(false);
	m_pJetton3->setVisible(false);
	//���ñ���
	m_lCurrentJetton=100L;
}

//���
void CDanShuangGameView::OnClearJetton(CCObject *pSender)
{
	//�������ָ��
	m_pDanShuangGame->OnClearJetton();
}

//��ׯ
void CDanShuangGameView::OnApplyBanker(CCObject *pSender)
{
	m_pDanShuangGame->OnApplyBanker();
}

//ȡ����ׯ
void CDanShuangGameView::OnCancelBanker(CCObject *pSender)
{
	m_pDanShuangGame->OnCancelBanker();
}

//�˳�
void CDanShuangGameView::OnQuit(CCObject *pSender)
{
	//�ǿ��У�ȷ���˳�
	if (m_pDanShuangGame->GetGameStatus() != GS_GAME_FREE)
	{
		m_pLayerQuit->setVisible(true);
	}
	else
	{
		OnSubmitQuit(NULL);
	}
}

//ȷ���˳�
void CDanShuangGameView::OnSubmitQuit(CCObject *pSender)
{
	//�����
#ifndef AOYIGAME
	if (!m_pDanShuangGame->m_pDirector->SendRoomData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ, NULL, 0))
	{
		m_pDanShuangGame->m_pDirector->ReturnFromGame(eLobbyType_Room, true);
		return ;
	}
#else
	m_pDanShuangGame->m_pDirector->ReturnFromGame(eLobbyType_Table);
#endif
	//timer
	scheduleOnce(schedule_selector(CDanShuangGameView::OnDisconnected), 2.0f);
}

//ȡ���˳�
void CDanShuangGameView::OnCancelQuit(CCObject *pSender)
{
	m_pLayerQuit->setVisible(false);
}

void CDanShuangGameView::OnDisconnected(float dt)
{
	m_pDanShuangGame->m_pDirector->ReturnFromGame(eLobbyType_Room, false);
}

//����ʱ��
void CDanShuangGameView::SetTime(int iTime, int iType)
{
	CCLOG("on set timer %d-%d\r\n", iTime, iType );
	if (iTime < 0)
	{
		switch(iType)
		{
		case GS_GAME_FREE:
			unschedule(schedule_selector(CDanShuangGameView::OnTimeFree));
			break;
		case GS_GAME_ANIMATION:
			unschedule(schedule_selector(CDanShuangGameView::OnTimeAnimation));
			break;
		case GS_GAME_START:
			unschedule(schedule_selector(CDanShuangGameView::OnTimePlaceJetton));
			break;
		case GS_GAME_END:
			unschedule(schedule_selector(CDanShuangGameView::OnTimeEnd));
			break;
		default:
			CC_ASSERT(false);
		}
		iTime = 0;
	}
	
	char szTime[8];
	sprintf(szTime,"%d",iTime);
	//m_pLabelTime->setString(szTime);
	m_pLabelLeaveTime->setString(szTime);

	if (iTime<=5 && iTime>=0 && iType != GS_GAME_ANIMATION)
	{	
		string strPath = SOUND_PATH;
		strPath += "TIME_WARIMG.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}

	CCLOG("on set timer %d-%d end\r\n", iTime, iType );
}

//����ʱ��
void CDanShuangGameView::SetGameTime(int iTime, int iType)
{
	CCLOG("SetGameTime");
	unschedule(schedule_selector(CDanShuangGameView::OnTimeFree));
	unschedule(schedule_selector(CDanShuangGameView::OnTimeAnimation));
	unschedule(schedule_selector(CDanShuangGameView::OnTimePlaceJetton));
	unschedule(schedule_selector(CDanShuangGameView::OnTimeEnd));

	m_cbGameTime = iTime;

	string strGameStatus;

	SetTime(iTime, iType);

	CCTextureCache * pShareTextCache = CCTextureCache::sharedTextureCache();
	switch(iType)
	{
	case GS_GAME_FREE:
		m_pSpriteGameStatus->setTexture(pShareTextCache->textureForKey("DanShuang/freetimestatus.png"));
		schedule(schedule_selector(CDanShuangGameView::OnTimeFree),1.0f);
		return;
	case GS_GAME_ANIMATION:
		m_pSpriteGameStatus->setTexture(pShareTextCache->textureForKey("DanShuang/cointimestatus.png"));
		schedule(schedule_selector(CDanShuangGameView::OnTimeAnimation),1.0f);
		return ;
	case GS_GAME_START:
		m_pSpriteGameStatus->setTexture(pShareTextCache->textureForKey("DanShuang/jettontimestatus.png"));
		schedule(schedule_selector(CDanShuangGameView::OnTimePlaceJetton),1.0f);
		return;
	case GS_GAME_END:
		m_pSpriteGameStatus->setTexture(pShareTextCache->textureForKey("DanShuang/opentimestatus.png"));
		schedule(schedule_selector(CDanShuangGameView::OnTimeEnd),1.0f);
		return;
	default:
		CC_ASSERT(false);
	}

	char szTime[8];
	sprintf(szTime,"%d",iTime);
	m_pLabelLeaveTime->setString(szTime);
	CCLOG("SetGameTime END");
}

//����ʱ��
void CDanShuangGameView::OnTimeEnd(float dt)
{
	CCLOG("Time:%d", m_cbGameTime);
	if (m_cbGameTime > 0)
		m_cbGameTime--;
	SetTime(m_cbGameTime, GS_GAME_END);
	CCLOG("Time End:%d", m_cbGameTime);
}

//�ױ�ʱ��
void CDanShuangGameView::OnTimeAnimation(float dt)
{
	if (m_cbGameTime > 0)
		m_cbGameTime--;
	SetTime(m_cbGameTime, GS_GAME_ANIMATION);
}

//��עʱ��
void CDanShuangGameView::OnTimePlaceJetton(float dt)
{
	if (m_cbGameTime > 0)
		m_cbGameTime--;
	SetTime(m_cbGameTime, GS_GAME_START);

	if (m_cbGameTime<=5)
		m_pClearJetton->setEnabled(false);
}

//����ʱ��
void CDanShuangGameView::OnTimeFree(float dt)
{
	if (m_cbGameTime > 0)
		m_cbGameTime--;
	SetTime(m_cbGameTime, GS_GAME_FREE);
}

//����ʤ��������˸
void CDanShuangGameView::SetWinnerSide(BYTE cbWinnerSide)
{
	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		CCSprite * pFlashFrame = NULL;

		if (m_enNowOpenType == CT_RED)
		{
			pFlashFrame = CCSprite::create("DanShuang/flashred.png");
		}
		else if (m_enNowOpenType == CT_BLUE)
		{
			pFlashFrame = CCSprite::create("DanShuang/flashblue.png");
		}
		else
		{
			CC_ASSERT(false);
		}

		pFlashFrame->setPosition(m_PointAreaMid[m_enNowOpenType - CT_RED]);

		pFlashFrame->runAction(CCSequence::create(CCBlink::create(3.0f,3),
			CCCallFuncN::create(this,callfuncN_selector(CDanShuangGameView::ShowSettlement)),NULL));
		m_pLayerFlashWinner->addChild(pFlashFrame);
	}
	else
	{
		m_pLayerFlashWinner->removeAllChildren();
	}
}

//��ʾ�����
void CDanShuangGameView::ShowSettlement(CCNode *pNode)
{
	CCSprite *pSettlement = CCSprite::create("DanShuang/settlement.png");
	pSettlement->setPosition(CCPointMake(m_screenSize.width/2, m_screenSize.height/2));
	m_pLayerFlashWinner->addChild(pSettlement, 1);

	//��ҳɼ�
	CCLabelAtlas *pMeCurGameScore = CCLabelAtlas::create("0123456789-","DanShuang/userscorenum_s.png",18,27,'0');
	char szMeCurGameScore[256];
	if (m_lMeCurGameScore<0)
		sprintf(szMeCurGameScore,":%lld",-m_lMeCurGameScore);
	else
		sprintf(szMeCurGameScore,"%lld",m_lMeCurGameScore);
	pMeCurGameScore->setString(szMeCurGameScore);
	pMeCurGameScore->setAnchorPoint(CCPointMake(0.5f,0.5f));

	//���ط���
	CCLabelAtlas *pMeCurGameReturnScore = CCLabelAtlas::create("0123456789-","DanShuang/userscorenum_s.png",18,27,'0');
	char szMeCurGameReturnScore[256];
	if (m_lMeCurGameReturnScore<0)
		sprintf(szMeCurGameReturnScore,":%lld",-m_lMeCurGameReturnScore);
	else
		sprintf(szMeCurGameReturnScore,"%lld",m_lMeCurGameReturnScore);
	pMeCurGameReturnScore->setString(szMeCurGameReturnScore);
	pMeCurGameReturnScore->setAnchorPoint(CCPointMake(0.5f,0.5f));

	pMeCurGameScore->setPosition(CCPointMake(m_screenSize.width/2 - 40, m_screenSize.height/2 + 31));
	pMeCurGameReturnScore->setPosition(CCPointMake(m_screenSize.width/2 + 165, m_screenSize.height/2 + 31));

	//ׯ�ҳɼ�
	CCLabelAtlas *pBankerCurGameScore = CCLabelAtlas::create("0123456789-","DanShuang/userscorenum_s.png",18,27,'0');
	char szBankerCurGameScore[256];
	if (m_lBankerCurGameScore<0)
		sprintf(szBankerCurGameScore,":%lld",-m_lBankerCurGameScore);
	else
		sprintf(szBankerCurGameScore,"%lld",m_lBankerCurGameScore);
	pBankerCurGameScore->setString(szBankerCurGameScore);
	pBankerCurGameScore->setAnchorPoint(CCPointMake(0.5f,0.5f));
	pBankerCurGameScore->setPosition(CCPointMake(m_screenSize.width/2 - 40,m_screenSize.height/2 - 83));

	m_pLayerFlashWinner->addChild(pMeCurGameScore,1);
	m_pLayerFlashWinner->addChild(pMeCurGameReturnScore,1);
	m_pLayerFlashWinner->addChild(pBankerCurGameScore,1);
}

//������г���
void CDanShuangGameView::ClearAllJetton()
{
	//���·���
	memset(m_lMeScore, 0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0, sizeof(m_lAllScore));

	//���½���
	m_pJettonBatchNode->removeAllChildrenWithCleanup(true);
	DrawUserScore();
	DrawTableScore();

	return;
}

//��������ע
void CDanShuangGameView::ClearUserJetton(WORD wChairID, const LONGLONG lAllJettonScore[AREA_COUNT])
{
	//������ҷ���
	memcpy(m_lAllScore, lAllJettonScore, sizeof(m_lAllScore));
	if (m_pDanShuangGame->m_pDirector->GetMeData()->nUserChair == wChairID)
	{
		memset(m_lMeScore, 0, sizeof(m_lMeScore));
		DrawUserScore();
	}

	//���½���
	m_pJettonBatchNode->removeChildByTag((int)wChairID, true);
	DrawTableScore();

	return ;
}

//���ó���
void CDanShuangGameView::PlaceUserJetton(BYTE cbJettonArea, LONGLONG lJettonScore, WORD wChairID)
{
	//У�����
	CC_ASSERT(cbJettonArea >= JA_RED && cbJettonArea <= JA_BLUE);
	if (cbJettonArea < JA_RED || cbJettonArea > JA_BLUE)
		return ;

	//�������
	BYTE cbAreaIndex = cbJettonArea - JA_RED;	//��ע������
	const BYTE cbJettonRadius = 19;	//����뾶
	const BYTE cbAreaRadius = 150;	//����뾶
	CCSpriteFrame * pJettonSpriteFrame[4];	//��ע�ó���֡
	CCSpriteFrameCache * pShareFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

	for (BYTE i = 0; i < CountArray(pJettonSpriteFrame); i++)
	{
		char szFileName[50] = "";	//�����ļ���
		snprintf(szFileName, sizeof(szFileName), "tablejetton%d.png", i);
		string strTemp = CStringHelper::GB2312ToUTF8(szFileName);
		pJettonSpriteFrame[i] = pShareFrameCache->spriteFrameByName(strTemp.c_str());
	}

	//������ע��Ϣ
	m_lAllScore[cbAreaIndex] += lJettonScore;
	if (m_pDanShuangGame->m_pDirector->GetMeData()->nUserChair == wChairID )
	{
		m_lMeScore[cbAreaIndex] += lJettonScore;
		DrawUserScore();
	}

	//���ӳ���
	for (int i = CountArray(g_dwJettonValue) - 1; i >= 0; --i)
	{
		LONGLONG lCellCount = lJettonScore / g_dwJettonValue[i];	//ѡ�г������

		if (lCellCount == 0)
			continue;

		//��ӳ���
		for (LONGLONG j = 0; j < lCellCount; j++)
		{
			CCPoint JettonPos;	//����λ��
			JettonPos.x = m_PointAreaMid[cbAreaIndex].x + (rand() % 2 ? (1) : (-1)) * (rand() % (cbAreaRadius - cbJettonRadius));
			JettonPos.y = m_PointAreaMid[cbAreaIndex].y + (rand() % 2 ? (1) : (-1)) * (rand() % (cbAreaRadius - cbJettonRadius));

			//������뾫��
			CCSprite * pJettonSprite = CCSprite::createWithSpriteFrame(pJettonSpriteFrame[i]);
			pJettonSprite->setPosition(JettonPos);

			m_pJettonBatchNode->addChild(pJettonSprite, 1, (int)wChairID);
		}

		//������Ŀ
		lJettonScore -= lCellCount * g_dwJettonValue[i];
	}

	//�������ӷ���
	DrawTableScore();

	return;
}

//�����˿���Ϣ
void CDanShuangGameView::SetCardInfo(enOpenCardType enCardType, bool bAnimation)
{
	//���ÿ��ƶ���
	CC_ASSERT(enCardType >= CT_RED && enCardType <= CT_BLUE);
	if (enCardType < CT_RED || enCardType > CT_BLUE)
		return ;

	m_enNowOpenType = enCardType;

	if (bAnimation)
		BeginOpenCoin();
}

//���ֳɼ�
void CDanShuangGameView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore = lMeCurGameScore;			
	m_lMeCurGameReturnScore = lMeCurGameReturnScore;			
	m_lBankerCurGameScore = lBankerCurGameScore;			
	m_lGameRevenue = lGameRevenue;					
}

//����ׯ�ҷ���
void CDanShuangGameView::SetBankerScore(WORD wBankerTime, LONGLONG lWinScore)
{
	m_wBankerTime = wBankerTime; 
	m_lBankerWinScore = lWinScore;
	DrawBankerInfo();
}

//������ʷ��¼
void CDanShuangGameView::SetGameHistory(const BYTE cbOpenRecord[MAX_HISTORY_RECORD])
{
	m_cbHistoryRecordCount = 0;
	for (BYTE i = 0; i < MAX_HISTORY_RECORD; ++i)
	{
		if (cbOpenRecord[i] == CT_NULL)
			break;
		m_enHistoryRecord[m_cbHistoryRecordCount++] = (enOpenCardType)cbOpenRecord[i];

		string strAddr;	//ͼƬ��ַ
		switch(cbOpenRecord[i])
		{
		case CT_RED:
			{
				strAddr = "DanShuang/recordred.png";
				break;
			}
		case CT_BLUE:
			{
				strAddr = "DanShuang/recordblue.png";
				break;
			}
		default:
			{
				CC_ASSERT(false);
			}
		}

		CCSprite * pSprite1 = CCSprite::create(strAddr.c_str());
		pSprite1->setPosition(ccp(40 + i * g_fRecordDistance, 20));
		m_pRecordClipNode->addChild(pSprite1, 1, i);
	}
	CCSprite * pSprite2 = CCSprite::create("DanShuang/recordback.png");
	pSprite2->setPosition(ccp(40 + (m_cbHistoryRecordCount - 1) * g_fRecordDistance, 22));
	m_pRecordClipNode->addChild(pSprite2, 2, 200);

	return;
}

//�����ׯ
void CDanShuangGameView::AddApplyBanker(const tagApplyUser &applyUser)
{
	//�Ƿ������б�
	for (DWORD i=0; i<m_ApplyUserArray.size(); ++i)
	{
		if (m_ApplyUserArray[i].wChairID == applyUser.wChairID)
		{
			return ;
		}
	}

	m_ApplyUserArray.push_back(applyUser);

	DrawBankerList();
}

//ȡ����ׯ
void CDanShuangGameView::CancelApplyBanker(const tagApplyUser &applyUser)
{
	bool bFind = false;
	for(vector<tagApplyUser>::iterator it=m_ApplyUserArray.begin(); it!=m_ApplyUserArray.end(); ++it)
	{
		if (it->wChairID == applyUser.wChairID)
		{
			m_ApplyUserArray.erase(it);
			bFind = true;
			break;
		}
	}
	if (!bFind)  // û��ɾ��
	{
		return ;
	}

	DrawBankerList();
}

//�ֻ�ׯ��
void CDanShuangGameView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
	if (bChangeBanker)
	{
		if (m_wBankerUser == m_pDanShuangGame->m_pDirector->GetMeData()->nUserChair)
		{
			CCSprite *pMeBanker = CCSprite::create("DanShuang/mebanker.png");
			pMeBanker->setPosition(CCPointMake(m_screenSize.width/2, m_screenSize.height/2));
			addChild(pMeBanker,1);
			pMeBanker->runAction(CCSequence::create(CCDelayTime::create(2.0f),
				CCCallFuncN::create(this,callfuncN_selector(CDanShuangGameView::DestorySelf)),NULL));
			
		}
		else if (m_wBankerUser != INVALID_CHAIR)
		{
			CCSprite *pOtherBanker = CCSprite::create("DanShuang/changebanker.png");
			pOtherBanker->setPosition(CCPointMake(m_screenSize.width/2, m_screenSize.height/2));
			addChild(pOtherBanker,1);
			pOtherBanker->runAction(CCSequence::create(CCDelayTime::create(2.0f),
				CCCallFuncN::create(this,callfuncN_selector(CDanShuangGameView::DestorySelf)),NULL));
		}
		else
		{
			CCSprite *pNoBanker = CCSprite::create("DanShuang/nobanker.png");
			pNoBanker->setPosition(CCPointMake(m_screenSize.width/2, m_screenSize.height/2));
			addChild(pNoBanker,1);
			pNoBanker->runAction(CCSequence::create(CCDelayTime::create(2.0f),
				CCCallFuncN::create(this,callfuncN_selector(CDanShuangGameView::DestorySelf)),NULL));
		}
	}
}

//ׯ����Ϣ
void CDanShuangGameView::SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore) 
{
	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser = wBankerUser;
		m_wBankerTime = 0L;
		m_lBankerWinScore = 0L;
	}
	m_lBankerScore = lBankerScore;
	DrawBankerInfo();
}

//ׯ����Ϣ
void CDanShuangGameView::OnBankerInfo(CCObject *pSender)
{
	m_pLayerBankerInfo->setVisible(true);
	m_pLayerBankerList->setVisible(false);
	m_pLayerScoreRecord->setVisible(false);
}

//��ׯ�б�
void CDanShuangGameView::OnBankerList(CCObject *pSender)
{
	m_pLayerBankerList->setVisible(true);
	m_pLayerBankerInfo->setVisible(false);
	m_pLayerScoreRecord->setVisible(false);
}


//������¼
void CDanShuangGameView::OnScoreRecord(CCObject *pSender)
{
	m_pLayerBankerList->setVisible(false);
	m_pLayerBankerInfo->setVisible(false);
	//m_pLayerScoreRecord->setVisible(true);

	m_pLayerScoreRecord->OnLayerIn(NULL,eLayerAni_UpIn,0.5f);
}

//���ƽ������
void CDanShuangGameView::OpenTypeAnimation()
{
	//�߼�У��
	CC_ASSERT(m_cbHistoryRecordCount >= MAX_HISTORY_RECORD && m_enNowOpenType != CT_NULL && MAX_HISTORY_RECORD == 10);
	if (m_cbHistoryRecordCount < MAX_HISTORY_RECORD || m_enNowOpenType == CT_NULL || MAX_HISTORY_RECORD != 10)
		return ;

	//�ͷŵ�һ����¼
	CCSprite * pSpriteRel = dynamic_cast<CCSprite *>(m_pRecordClipNode->getChildByTag(0));
	CCMoveBy * pActionMoveBy = CCMoveBy::create(0.5, ccp(-70, 0));	//�ƶ�����
	pSpriteRel->runAction(CCSequence::create(pActionMoveBy, CCCallFuncN::create(this,
		callfuncN_selector(CDanShuangGameView::TypeRelSelf)), NULL));

	//�ƶ��м��¼
	for (BYTE i = 1; i < 10; ++i)
	{
		CCMoveBy * pActionMoveByTemp = dynamic_cast<CCMoveBy *>(pActionMoveBy->copy());
		pActionMoveByTemp->autorelease();
		CCSprite * pSpriteMove = dynamic_cast<CCSprite *>(m_pRecordClipNode->getChildByTag(i));
		pSpriteMove->runAction(CCSequence::create(pActionMoveByTemp,
			CCCallFuncN::create(this, callfuncN_selector(CDanShuangGameView::TypeCutSelfTag)), NULL));
	}

	//����µļ�¼
	string strAddr;	//ͼƬ��ַ
	if (m_enNowOpenType == CT_RED)
		strAddr = "DanShuang/recordred.png";
	else if (m_enNowOpenType == CT_BLUE)
		strAddr = "DanShuang/recordblue.png";
	else
		CC_ASSERT(false);

	CCSprite * pSpriteAdd= CCSprite::create(strAddr.c_str());
	pSpriteAdd->setPosition(ccp(40 + 10 * g_fRecordDistance, 20));
	m_pRecordClipNode->addChild(pSpriteAdd, 1, 10);
	CCMoveBy * pActionMoveByTemp = dynamic_cast<CCMoveBy *>(pActionMoveBy->copy());
	pActionMoveByTemp->autorelease();
	pSpriteAdd->runAction(CCSequence::create(pActionMoveByTemp,
		CCCallFuncN::create(this, callfuncN_selector(CDanShuangGameView::TypeCutSelfTag)), NULL));

	return ;
}

//�ͷż�¼����
void CDanShuangGameView::TypeRelSelf(CCNode * pNode)
{
	pNode->getParent()->removeChild(pNode);
}

//�����Լ���ʶ
void CDanShuangGameView::TypeCutSelfTag(CCNode * pNode)
{
	pNode->setTag(pNode->getTag() - 1);
}

//��ɿ��ƶ���
void CDanShuangGameView::FinishOpenAnimation(bool bFlash)
{
	//������ʷ��¼
	if (bFlash && m_enNowOpenType != CT_NULL)
	{
		if (m_cbHistoryRecordCount < MAX_HISTORY_RECORD)
		{
			//����¼�¼
			m_enHistoryRecord[m_cbHistoryRecordCount] = m_enNowOpenType;
			string strAddr;	//ͼƬ��ַ
			if (m_enNowOpenType == CT_RED)
				strAddr = "DanShuang/recordred.png";
			else if (m_enNowOpenType == CT_BLUE)
				strAddr = "DanShuang/recordblue.png";
			else
				CC_ASSERT(false);

			CCSprite * pSprite1 = CCSprite::create(strAddr.c_str());
			pSprite1->setPosition(ccp(40 + m_cbHistoryRecordCount * g_fRecordDistance, 20));
			m_pRecordClipNode->addChild(pSprite1, 1, m_cbHistoryRecordCount);

			//���¼�¼����
			CCSprite * pSpriteBack = dynamic_cast<CCSprite *>(m_pRecordClipNode->getChildByTag(200));
			pSpriteBack->setPosition(ccp(40 + m_cbHistoryRecordCount * g_fRecordDistance, 22));

			m_cbHistoryRecordCount++;
		}
		else
		{
			//��ʾ���ƽ������
			OpenTypeAnimation();
		}
	}

	//���óɼ�
	m_lMeStatisticScore += m_lMeCurGameScore;
	DrawUserMark();

	//����Ӯ��
	if (bFlash)
		SetWinnerSide(0);

	//��������
	if (m_lMeCurGameScore>0)
	{
		string strPath = SOUND_PATH;
		strPath += "END_WIN.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}
	else if (m_lMeCurGameScore<0)
	{
		string strPath = SOUND_PATH;
		strPath += "END_LOST.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}
	else
	{
		string strPath = SOUND_PATH;
		strPath += "END_DRAW.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}
}

//�����¼�
bool CDanShuangGameView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void CDanShuangGameView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}
void CDanShuangGameView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pt = pTouch->getLocation();
	if (m_lCurrentJetton!=0L)
	{
		BYTE cbJettonArea=GetJettonArea(pt);
		CCLOG("cbJettonArea = %d",cbJettonArea);

		//������ע��Ϣ
		if (cbJettonArea >= JA_RED && cbJettonArea <= JA_BLUE)
		{
			m_pDanShuangGame->OnPlaceJetton(cbJettonArea, m_lCurrentJetton);
		}
	}
}

//��ȡ����
BYTE CDanShuangGameView::GetJettonArea(CCPoint &ClickPoint)
{
	if (m_rcAreaRange[JA_RED - JA_RED].containsPoint(ClickPoint))
	{
		for (BYTE i = 0; i < CountArray(m_RedAreaTriangle); i++)
		{
			if (m_GameLogic.IsPointInTriangle(ClickPoint, m_RedAreaTriangle[i]))
			{
				return 0xFF;
			}
		}

		return JA_RED;
	}
	else if (m_rcAreaRange[JA_BLUE - JA_RED].containsPoint(ClickPoint))
	{
		for (BYTE i = 0; i < CountArray(m_BlueAreaTriangle); i++)
		{
			if (m_GameLogic.IsPointInTriangle(ClickPoint, m_BlueAreaTriangle[i]))
			{
				return 0xFF;
			}
		}

		return JA_BLUE;
	}

	return 0xFF;
}

//�����ױҶ���
void CDanShuangGameView::BeginThrowCoin(bool bStart)
{
	m_pLayerCoinAnimation->setVisible(true);
	CCSpriteFrameCache * pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	if (bStart == false)
	{
		m_pSpriteCoin->setVisible(false);
		m_pSpriteHand->setVisible(true);
		m_pSpriteCoin->stopAllActions();
		m_pSpriteHand->stopAllActions();
		m_pSpriteHand->initWithSpriteFrame(pFrameCache->spriteFrameByName("hand_9.png"));
	}
	else
	{
		//��ʼ������
		m_pSpriteCoin->setVisible(true);
		m_pSpriteCoin->setPosition(ccp(704, 402));
		m_pSpriteCoin->initWithSpriteFrame(pFrameCache->spriteFrameByName("coin_1.png"));

		m_pSpriteHand->setVisible(true);
		m_pSpriteHand->setPosition(ccp(640, 317));
		m_pSpriteHand->initWithSpriteFrame(pFrameCache->spriteFrameByName("hand_1.png"));

		//�����ױ��ӳ�
		scheduleOnce(schedule_selector(CDanShuangGameView::ThrowCoinDelay), 1.0f);
	}

	return ;
}

//���п��Ҷ���
void CDanShuangGameView::BeginOpenCoin()
{
	//�������Ҷ���
	CCAnimationCache * pAnimaCache = CCAnimationCache::sharedAnimationCache();
	CCAnimation * kaibi_ani = pAnimaCache->animationByName("kaibi");
	kaibi_ani->setRestoreOriginalFrame(false);
	CCAnimate * kaibi = CCAnimate::create(kaibi_ani);
	m_pSpriteHand->runAction(kaibi);

	m_pSpriteCoin->setVisible(true);
	m_pSpriteCoin->setPosition(ccp(630, 270));
	string strCoinType;	//���ҽ��
	if (m_enNowOpenType == CT_RED)
		strCoinType = "coin_red.png";
	else if(m_enNowOpenType == CT_BLUE)
		strCoinType = "coin_blue.png";
	else
		CC_ASSERT(false);

	m_pSpriteCoin->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(strCoinType.c_str()));
	m_pSpriteCoin->setScaleY(0.7f);
	m_pSpriteCoin->runAction(CCSequence::create(CCScaleTo::create(0.0f, 1.0f, 0.7f),
		CCSpawn::create(CCMoveBy::create(0.27f, ccp(-3.0f, -6.0f)), CCScaleTo::create(0.27f, 1.0f, 0.8f), NULL),
		CCScaleTo::create(0.0f, 1.0f, 1.0f), NULL));

	//��ʾ������Ϣ
	this->scheduleOnce(schedule_selector(CDanShuangGameView::ShowOpenCardInfo), 3.0f);

	return ;
}

//�ױ��ӳ�
void CDanShuangGameView::ThrowCoinDelay(float dt)
{
	//����Ӳ�Ҷ���
	CCAnimationCache * pAnimaCache = CCAnimationCache::sharedAnimationCache();
	CCAnimation * coin_ani = pAnimaCache->animationByName("coin");
	coin_ani->setRestoreOriginalFrame(false);
	CCAnimate * coin = CCAnimate::create(coin_ani);
	m_pSpriteCoin->runAction(CCRepeatForever::create(coin));

	//����Ӳ��λ��
	m_wCoinAnimateTime = 0;
	schedule(schedule_selector(CDanShuangGameView::UpdateCoinPostion), 0.0660f);
}

//����Ӳ��λ��
void CDanShuangGameView::UpdateCoinPostion(float dt)
{
	const float fSpeed = 72;	//Ӳ���ٶ�
	const float fAccelerat = -6;	//Ӳ�Ҽ��ٶ�

	//����λ��
	float fPosition = fSpeed * m_wCoinAnimateTime + fAccelerat * m_wCoinAnimateTime * m_wCoinAnimateTime;
	float fBaseX = 704 - 4 * m_wCoinAnimateTime;
	float fBaseY = 480;
	m_pSpriteCoin->setPosition(ccp(fBaseX, fBaseY + fPosition));

	//����ʱ��
	m_wCoinAnimateTime++;

	if (m_wCoinAnimateTime >= 16)
	{
		this->unschedule(schedule_selector(CDanShuangGameView::UpdateCoinPostion));
		m_pSpriteCoin->stopAllActions();
		m_pSpriteCoin->setVisible(false);
		m_wCoinAnimateTime = 0;
	}

	if (m_wCoinAnimateTime == 1)
	{
		//�����ױҶ���
		CCAnimationCache *pAnimaCache = CCAnimationCache::sharedAnimationCache();
		CCAnimation * paobi_ani = pAnimaCache->animationByName("paobi");
		paobi_ani->setRestoreOriginalFrame(false);
		CCAnimate * paobi = CCAnimate::create(paobi_ani);
		m_pSpriteHand->runAction(paobi);

		//�����Ч
		string strPath = SOUND_PATH;
		strPath += "throw_coin.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}

	if (m_wCoinAnimateTime == 12)
	{
		//�����ӱҶ���
		CCAnimationCache *pAnimaCache = CCAnimationCache::sharedAnimationCache();
		CCAnimation * jiebi_ani = pAnimaCache->animationByName("jiebi");
		jiebi_ani->setRestoreOriginalFrame(false);
		CCAnimate * jiebi = CCAnimate::create(jiebi_ani);
		m_pSpriteHand->runAction(jiebi);
	}

	return ;
}

//��ʾ������Ϣ
void CDanShuangGameView::ShowOpenCardInfo(float dt)
{
	//���ض���ͼ��
	m_pSpriteHand->stopAllActions();
	m_pSpriteCoin->stopAllActions();
	m_pLayerCoinAnimation->setVisible(false);
	FinishOpenAnimation(true);
}

//��ʾ�Լ�
void CDanShuangGameView::DisplaySelf(CCNode *pNode)
{
	pNode->setVisible(true);
}

//�����Լ�
void CDanShuangGameView::DestorySelf(CCNode *pNode)
{
	removeChild(pNode);
}

//���Ʒ���
void CDanShuangGameView::DrawUserScore()
{
	LONGLONG lUserScore;
	LONGLONG lMeJetton = 0;
	for (int i=0; i < AREA_COUNT; i++)
	{
		lMeJetton += m_lMeScore[i];
	}
	lUserScore = m_pDanShuangGame->m_pDirector->GetMeData()->nUserScore - lMeJetton;
	char szUserScore[256];
	sprintf(szUserScore,"%lld",lUserScore);
	m_pLabelUserScore->setString(szUserScore);
}

//���Ƴɼ�
void CDanShuangGameView::DrawUserMark()
{
	char szUserMark[256];
	if (m_lMeStatisticScore<0)
		sprintf(szUserMark,":%lld",-m_lMeStatisticScore);
	else
		sprintf(szUserMark,"%lld",m_lMeStatisticScore);
	m_pLabelUserMark->setString(szUserMark);
}

//�����������
void CDanShuangGameView::DrawTableScore()
{
	for (int i=0; i < AREA_COUNT; i++)
	{
		if (m_lMeScore[i]>0)
		{
			m_pTableScoreBK[0][i]->setVisible(true);
			m_pLabelMeTableScore[i]->setVisible(true);
			char szMeScore[256];
			sprintf(szMeScore,"%lld",m_lMeScore[i]);
			m_pLabelMeTableScore[i]->setString(szMeScore);
		}
		else if (m_lMeScore[i] <= 0)
		{
			m_pTableScoreBK[0][i]->setVisible(false);
			m_pLabelMeTableScore[i]->setVisible(false);
			m_pLabelMeTableScore[i]->setString("0");
		}

		if (m_lAllScore[i]>0)
		{
			m_pTableScoreBK[1][i]->setVisible(true);
			m_pLabelAllTableScore[i]->setVisible(true);
			char szAllScore[256];
			sprintf(szAllScore,"%lld",m_lAllScore[i]);
			m_pLabelAllTableScore[i]->setString(szAllScore);
		}
		else if (m_lAllScore[i] <= 0)
		{
			m_pTableScoreBK[1][i]->setVisible(false);
			m_pLabelAllTableScore[i]->setVisible(false);
			m_pLabelAllTableScore[i]->setString("0");
		}
	}
}

//�ر�
void CDanShuangGameView::OnClose(CCObject *pSender)
{
	m_pLayerBankerInfo->setVisible(false);
	m_pLayerBankerList->setVisible(false);
	m_pLayerScoreRecord->setVisible(false);
	m_pQuit->setEnabled(true);
}

//����ׯ����Ϣ
void CDanShuangGameView::DrawBankerInfo()
{
	tagLocalUser *pUser;
	if (m_wBankerUser==INVALID_CHAIR)
		pUser = NULL;
	else
		pUser = m_pDanShuangGame->m_pDirector->GetTableUserData(m_wBankerUser);

	if (pUser == NULL)
	{
		string strUser = CStringHelper::GB2312ToUTF8("ϵͳ��ׯ");
		m_pBankerUser->setString(strUser.c_str());
		m_pBankerScore->setString("0");
	}
	else
	{
		//string strUser = CStringHelper::GB2312ToUTF8(pUser->sUserName.c_str());
		m_pBankerUser->setString(pUser->sUserName.c_str());
		char szBankerScore[256];
		sprintf(szBankerScore,"%lld",pUser->nUserScore);
		m_pBankerScore->setString(szBankerScore);
	}

	char szBankerMark[256];
	if (m_lBankerWinScore<0)
		sprintf(szBankerMark,":%lld",-m_lBankerWinScore);
	else
		sprintf(szBankerMark,"%lld",m_lBankerWinScore);
	m_pBankerMark->setString(szBankerMark);
	char szBankerTime[128];
	sprintf(szBankerTime,"%d",m_wBankerTime);
	m_pBankerTime->setString(szBankerTime);
}

//������ׯ�б�
void CDanShuangGameView::DrawBankerList()
{
	for (BYTE i=0; i < SHOW_APPLY_BANKER; ++i)
	{
		if (i < m_ApplyUserArray.size())
		{
			m_pLabelApplyBankerUser[i]->setString(m_ApplyUserArray[i].strUserName.c_str());
		}
		else
		{
			m_pLabelApplyBankerUser[i]->setString("");
		}
	}
}

//��ʾ
void CDanShuangGameView::ShowMessage(const char *Msg, bool bIsGB2312/* = true*/)
{
	string strMsg = (bIsGB2312 ? CStringHelper::GB2312ToUTF8(Msg) : Msg);
	m_pLabelMessage->stopAllActions();
	m_pLabelMessage->setPosition(ccp(m_screenSize.width / 2, m_screenSize.height / 2));
	m_pLabelMessage->setString(strMsg.c_str());
	m_pLabelMessage->setVisible(true);
	m_pLabelMessage->runAction(CCSequence::create(CCFadeIn::create(0.5f),CCDelayTime::create(2.0f),
		CCSpawn::create(CCMoveTo::create(1.0f,CCPointMake(m_screenSize.width/2, m_screenSize.height)),CCFadeOut::create(1.0f),
		NULL),NULL));
}
//���̲���
void CDanShuangGameView::ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag)
{
	CCLOG("ccOnKeyDown keyCode=%d, keyFlag=%d\r\n", keyCode, keyFlag );

	char szApplyBankerCondition[256];
	sprintf(szApplyBankerCondition,"CDanShuangGameView::ccOnKeyDown keyCode=%d, keyFlag=%d",keyCode, keyFlag);
	//ShowMessage(szApplyBankerCondition);

	//������¼ͼ���ж�
	if (m_pLayerScoreRecord->isVisible())
	{
		if (keyCode == 144)
		{
			m_pLayerScoreRecord->touchCancel(NULL, 0);
		}
		else
		{
			return ;
		}
	}

	//����״̬�ж�
	switch(keyCode)
	{
	case 145:	//97 /*VK_NUMPAD1*/ /*��*/
		{
			m_pDanShuangGame->OnPlaceJetton(JA_RED, m_lCurrentJetton);
		}
		break;
	case 146:	//98 /*VK_NUMPAD2*/ /*��*/
		{
			m_pDanShuangGame->OnPlaceJetton(JA_BLUE, m_lCurrentJetton);
		}
		break;
	case 147:	//99 /*VK_NUMPAD3*/ /*�л�����*/
		{
			if (m_pJetton0->isVisible() && m_pJetton0->isEnabled())
			{
				OnJettonButton100(NULL);
			}
			else if (m_pJetton1->isVisible() && m_pJetton1->isEnabled())
			{
				OnJettonButton1000(NULL);
			}
			else if (m_pJetton2->isVisible() && m_pJetton2->isEnabled())
			{
				OnJettonButton10000(NULL);
			}
			else if (m_pJetton3->isVisible() && m_pJetton3->isEnabled())
			{
				OnJettonButton100000(NULL);
			}
		}
		break;
	case 148:	//100 /*VK_NUMPAD4*/ /*��ׯ*/
		{
			if (m_pApplyBanker && m_pApplyBanker->isVisible() && m_pApplyBanker->isEnabled())
			{
				OnApplyBanker(NULL);
			}
		}
		break;
	case 149:	//101 /*VK_NUMPAD5*/ /*��ׯ*/
		{
			if (m_pCancelBanker && m_pCancelBanker->isVisible() && m_pCancelBanker->isEnabled())
			{
				OnCancelBanker(NULL);
			}
		}
		break;
	case 150:	//102 /*VK_NUMPAD6*/ /*�����ע**/
		{
			if (m_pClearJetton && m_pClearJetton->isVisible() && m_pClearJetton->isEnabled())
			{
				OnClearJetton(NULL);
			}
		}
		break;
	case 151:	//103 /*VK_NUMPAD7*/ /*ׯ����Ϣ*/
		{
			OnBankerInfo(NULL);
		}
		break;
	case 152:	//104 /*VK_NUMPAD8*/ /*��ׯ�б�*/
		{
			OnBankerList(NULL);
		}
		break;
	case 153:	//105 /*VK_NUMPAD9*/ /*������¼*/
		{
			OnScoreRecord(NULL);
		}
		break;
	case 156:	//109 /*VK_SUBTRACT*/ /*�˳���Ϸ*/
		{
			OnQuit(NULL);
		}
		break;
	case 154:	//111 /*VK_DIVIDE ����/ */ /*ȷ��*/
		{
			if (m_pLayerQuit->isVisible())
			{
				OnSubmitQuit(NULL);
			}
		}
		break;
	case 155:	//106 /*VK_MULTIPLY ����* */ /*ȡ��*/
		{
			if (m_pLayerQuit->isVisible())
			{
				OnCancelQuit(NULL);
			}
		}
		break;
	case 144:	//96 /*VK_NUMPAD0*/ /*�رմ���*/
		{
			if (m_pLayerBankerInfo->isVisible() || m_pLayerBankerList->isVisible())
			{
				OnClose(NULL);
			}
		}
		break;
	default:
		return ;
	}

	return ;
}

//void CDanShuangGameView::ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag)
//{
//	CCLOG("ccOnKeyUp keyCode=%d, keyFlag=%d\r\n", keyCode, keyFlag );
//
//	char szApplyBankerCondition[256];
//	sprintf(szApplyBankerCondition,"CDanShuangGameView::ccOnKeyUp keyCode=%d, keyFlag=%d",keyCode, keyFlag);
//
//	if (keyCode == 148)	//100 /*VK_NUMPAD4*/ /*����ׯ*/
//	{
//		if (m_pApplyBanker && m_pApplyBanker->isVisible() && m_pApplyBanker->isEnabled())
//		{
//			OnApplyBanker(NULL);
//		}
//		else if (m_pCancelBanker && m_pCancelBanker->isVisible() && m_pCancelBanker->isEnabled())
//		{
//			OnCancelBanker(NULL);
//		}
//	}
//}

//�����þ��鼯����
CDanShuangGameView::GameJettonBatchNode * CDanShuangGameView::GameJettonBatchNode::create(const char *fileImage, unsigned int capacity)
{
	GameJettonBatchNode *batchNode = new GameJettonBatchNode();
	batchNode->initWithFile(fileImage, capacity);
	batchNode->autorelease();

	return batchNode;
}

//�Ƴ���ͬ���ֵ�����нڵ�
void CDanShuangGameView::GameJettonBatchNode::removeChildByTag(int tag, bool cleanup)
{
	CC_ASSERT(tag != kCCNodeTagInvalid);
	vector<CCNode *> veFindNode;	//��ͬtag�Ľڵ�

	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCNode* pNode = (CCNode*) child;
			if(pNode && pNode->getTag() == tag)
			{
				veFindNode.push_back(pNode);
			}
		}
	}

	for (vector<CCNode *>::const_iterator it = veFindNode.begin(); it != veFindNode.end(); ++it)
	{
		this->removeChild(*it, cleanup);
	}

	return ;
}
