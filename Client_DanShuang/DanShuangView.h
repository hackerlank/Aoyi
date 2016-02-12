#ifndef _GAME_DAN_SHUANG_VIEW_H_
#define _GAME_DAN_SHUANG_VIEW_H_

#include "stdafx.h"
#include "ScoreRecordLayer.h"
#include "JettonMenuSprite.h"

#define JETTON_COUNT				4									//筹码
#define SHOW_APPLY_BANKER			5									// 庄家数目

/////////////////////////////////////////////
//上庄列表信息
struct tagApplyUser
{
	WORD							wChairID;								//玩家椅子号
	string							strUserName;							//玩家名字
};

class CDanShuangGame;

class CDanShuangGameView : public CCLayer
{
	//下注信息
private:
	LONGLONG						m_lMeScore[AREA_COUNT];					//玩家自己区域下注
	LONGLONG						m_lAllScore[AREA_COUNT];				//所有玩家区域下注

	//位置信息
private:
	CCRect							m_rcAreaRange[AREA_COUNT];				//下注区矩形范围
	CCPoint							m_PointAreaMid[AREA_COUNT];				//区域中心位置
	tagTriangle						m_RedAreaTriangle[3];					//红区域的两三角形
	tagTriangle						m_BlueAreaTriangle[3];					//蓝区域的两三角形

	//开奖信息
private:
	enOpenCardType					m_enNowOpenType;						//当前开牌类型

	//历史信息
private:
	LONGLONG						m_lMeStatisticScore;					//玩家游戏成绩统计
	enOpenCardType					m_enHistoryRecord[MAX_HISTORY_RECORD];	//历史记录结果
	BYTE							m_cbHistoryRecordCount;					//历史记录数目
	vector<tagApplyUser>			m_ApplyUserArray;						// 申请庄家队列

	//状态变量
private:
	LONGLONG						m_lCurrentJetton;						//当前筹码

	//庄家信息
private:	
	WORD							m_wBankerUser;							//当前庄家
	WORD							m_wBankerTime;							//做庄次数
	LONGLONG						m_lBankerScore;							//庄家积分
	LONGLONG						m_lBankerWinScore;						//庄家总成绩
	bool							m_bEnableSysBanker;						//可否系统做庄

	//当局成绩
private:
	LONGLONG						m_lMeCurGameScore;						//我的成绩
	LONGLONG						m_lMeCurGameReturnScore;				//我的成绩
	LONGLONG						m_lBankerCurGameScore;					//庄家成绩
	LONGLONG						m_lGameRevenue;							//游戏税收

	//辅助信息
private:
	BYTE							m_cbGameTime;							//时间
	CCSize							m_screenSize;							//屏幕尺寸
	CCSprite						*m_pSpriteGameStatus;					//游戏状态
	CCLabelAtlas					*m_pLabelLeaveTime;						//剩余时间

	//游戏图层
	CCLayer							*m_pLayerCoinAnimation;					//硬币动画图层
	CCSprite						*m_pSpriteHand;							//动画精灵手
	CCSprite						*m_pSpriteCoin;							//动画精灵硬币
	WORD							m_wCoinAnimateTime;						//硬币动画时间
	CCLayer							*m_pLayerFlashWinner;					//获胜区域闪烁图层
	CCLayer							*m_pLayerBankerList;					//上庄列表图层
	CCLayer							*m_pLayerBankerInfo;					//庄家信息图层
	CScoreRecordLayer				*m_pLayerScoreRecord;					//分数记录图层
	CCLayer							*m_pLayerHistoryRecord;					//历史记录图层
	CCClippingNode					*m_pRecordClipNode;						//记录用遮罩层
	CCSpriteBatchNode				*m_pJettonBatchNode;					//筹码用精灵集合类
	CCLayer							*m_pLayerQuit;							//退出图层

	//玩家信息
	CCLabelAtlas					*m_pLabelUserScore;						//玩家分数
	CCLabelTTF						*m_pLabelUserName;						//玩家名称
	CCLabelAtlas					*m_pLabelUserMark;						//玩家成绩

	//分数信息
	CCLabelAtlas					*m_pLabelMeTableScore[AREA_COUNT];		//桌子玩家分数
	CCLabelAtlas					*m_pLabelAllTableScore[AREA_COUNT];		//桌子全部分数
	CCSprite						*m_pTableScoreBK[2][AREA_COUNT];		//桌子分数背景
	CCLabelTTF						*m_pLabelMessage;						//提示消息

	//庄家信息
	CCLabelTTF						*m_pBankerUser;							//庄家名称
	CCLabelAtlas					*m_pBankerScore;						//庄家分数
	CCLabelAtlas					*m_pBankerTime;							//庄家上庄次数
	CCLabelAtlas					*m_pBankerMark;							//庄家成绩

	//上庄列表
	CCLabelTTF						*m_pLabelApplyBankerUser[SHOW_APPLY_BANKER];

	//非控制按钮
private:
	CCMenuItemImage					*m_pBankerInfoButton;					//庄家信息
	CCMenuItemImage					*m_pBankerList;							//上庄列表
	CCMenuItemImage					*m_pQuit;								//退出
	CCMenuItemImage					*m_pScoreRecord;						//分数记录

	//控制按钮
public:
	CJettonMenuSprite				*m_pJetton0;							//筹码100
	CJettonMenuSprite				*m_pJetton1;							//筹码1000
	CJettonMenuSprite				*m_pJetton2;							//筹码10000
	CJettonMenuSprite				*m_pJetton3;							//筹码100000
	CCMenuItemImage					*m_pClearJetton;						//清除下注
	CCMenuItemImage					*m_pApplyBanker;						//申请上庄
	CCMenuItemImage					*m_pCancelBanker;						//取消上庄

	//组件变量
private:
	CDanShuangGame					*m_pDanShuangGame;						//游戏接口
	CGameLogic						m_GameLogic;							//游戏逻辑

	//定义游戏筹码用精灵集合类
private:
	class GameJettonBatchNode : public CCSpriteBatchNode
	{
	public:
		//创建函数
		static GameJettonBatchNode* create(const char* fileImage) {
			return GameJettonBatchNode::create(fileImage, kDefaultSpriteBatchCapacity);
		}
		static GameJettonBatchNode * create(const char *fileImage, unsigned int capacity);

		//通过参数标记，移除子节点,清空相同标记值的所有节点
		virtual void removeChildByTag(int tag, bool cleanup);
	};

public:
	CDanShuangGameView(CDanShuangGame *pDanShuangGame);
	~CDanShuangGameView(void);

	static CDanShuangGameView* create(CDanShuangGame *pDanShuangGame);
	//初始化
	virtual bool init();
	void OnDisconnected(float dt);

	//触摸
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	//键盘操作
	virtual void ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag);
	virtual void ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag){};

	//动画相关
private:
	//进行开币动画
	void BeginOpenCoin();
	//抛币延迟
	void ThrowCoinDelay(float dt);
	//更新硬币位置
	void UpdateCoinPostion(float dt);
	//显示开牌信息
	void ShowOpenCardInfo(float dt);
	//显示自己
	void DisplaySelf(CCNode *pNode);
	//显示结算框
	void ShowSettlement(CCNode *pNode);
	//销毁
	void DestorySelf(CCNode *pNode);

	//开牌记录动画相关
private:
	//开牌结果动画
	void OpenTypeAnimation();
	//释放记录本身
	void TypeRelSelf(CCNode * pNode);
	//减少自己标识
	void TypeCutSelfTag(CCNode * pNode);

	//按钮回调函数
private:
	//筹码100
	void OnJettonButton100(CCObject *pSender);
	//筹码1000
	void OnJettonButton1000(CCObject *pSender);
	//筹码10000
	void OnJettonButton10000(CCObject *pSender);
	//筹码100000
	void OnJettonButton100000(CCObject *pSender);
	//清除
	void OnClearJetton(CCObject *pSender);
	//上庄
	void OnApplyBanker(CCObject *pSender);
	//分数记录
	void OnScoreRecord(CCObject *pSender);
	//庄家信息
	void OnBankerInfo(CCObject *pSender);
	//上庄列表
	void OnBankerList(CCObject *pSender);
	//取消上庄
	void OnCancelBanker(CCObject *pSender);
	//关闭
	void OnClose(CCObject *pSender);
	//退出
	void OnQuit(CCObject *pSender);
	//确认退出
	void OnSubmitQuit(CCObject *pSender);
	//取消退出
	void OnCancelQuit(CCObject *pSender);

	//状态定时器
private:
	//设置时间
	void SetTime(int iTime, int iType);
	//空闲时间
	void OnTimeFree(float dt);
	//动画时间
	void OnTimeAnimation(float dt);
	//下注时间
	void OnTimePlaceJetton(float dt);
	//结束时间
	void OnTimeEnd(float dt);

	//游戏控制接口
public:
	//获胜区域闪烁
	void SetWinnerSide(BYTE cbWinnerSide);
	//清除所有筹码
	void ClearAllJetton();
	//清空玩家下注
	void ClearUserJetton(WORD wChairID, const LONGLONG lAllJettonScore[AREA_COUNT]);
	//放置筹码
	void PlaceUserJetton(BYTE cbJettonArea, LONGLONG lJettonScore, WORD wChairID);
	//设置开奖结果
	void SetCardInfo(enOpenCardType enCardType, bool bAnimation);
	//设置成绩
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore,
		LONGLONG lGameRevenue);
	//设置庄家分数
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore);
	//添加用户到上庄列表
	void AddApplyBanker(const tagApplyUser &applyUser);
	//取消玩家上庄
	void CancelApplyBanker(const tagApplyUser &applyUser);
	//玩家取消下注
	void UserClearJetton(WORD wClearUser);
	//显示提示消息
	void ShowMessage(const char *Msg, bool bIsGB2312 = true);
	//设置历史记录
	void SetGameHistory(const BYTE cbOpenRecord[MAX_HISTORY_RECORD]);
	//显示切换庄家
	void ShowChangeBanker( bool bChangeBanker );

	//设置庄家信息
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//设置游戏状态时间
	void SetGameTime(int iTime, int iType);
	//完成开牌动画
	void FinishOpenAnimation(bool bFlash);
	//取得当前状态剩余时间
	BYTE GetGameTime() {return m_cbGameTime;}
	//进行抛币动画
	void BeginThrowCoin(bool bStart);

	//游戏下注区相关
private:
	//获取区域
	BYTE GetJettonArea(CCPoint &ClickPoint);

	//绘制
public:
	//绘制分数
	void DrawUserScore();
private:
	//绘制成绩
	void DrawUserMark();
	//绘制桌面分数
	void DrawTableScore();
	//绘制庄家信息
	void DrawBankerInfo();
	//绘制上庄列表
	void DrawBankerList();
};

#endif