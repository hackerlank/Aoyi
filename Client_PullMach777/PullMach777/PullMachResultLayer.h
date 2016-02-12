#ifndef _PullMach_RESULT_LAYER_H_
#define _PullMach_RESULT_LAYER_H_

#include "CMD_Game.h"

namespace GamePullMach777
{

class CPullMachGameLayer;
class CPMResManager;
class CPullMachRollNode;
class CPullMachBgSceneryNode;
class CGraduallyShowSprite;
class CContinueUpdateLabelAtlas;
class CPullMachPrizeRemainFontNode;

class CPullMachResultLayer : public CCLayer
{
public:
	static CPullMachResultLayer * create(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);

	CPullMachResultLayer(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);
	~CPullMachResultLayer();

	virtual bool init();

public:
	//设置玩家信息
	void setUserInfo(const tagLocalUser * pUserData);
	//设置滚动结果
	void setRollResult(BYTE cbResult[CELL_NUM]);
	//设置中奖线
	void setWinLineId(int nLineId);
	//设置各图标滚动次数
	void setRollCount(int nIndex, int nCount);
	//停止所有图标转动
	void stopAllIconRoll();
	//停止滚轮滚动,左中右：0、1、2
	void stopIconRoll(int nIndex);

	//设置为大奖模式
	void setBigPrizeMode(bool bPrizeMode);
	//设置logo数目
	bool setLogoCount(const int nLogoCount[VER_LINE_NUM]);

	//设置玩家分数
	void setUserScore(LONGLONG lScore);
	//设置玩家总输赢
	void setAllWinLoseScore(LONGLONG lScore);
	//设置上分分数
	void setCreditScore(LONGLONG lScore, bool bDirectShow);
	//玩家押注
	void setBetScore(int nBetScore);
	//设置玩家赢分
	void setWinScore(LONGLONG lScore, bool bDirectShow);
	//设置大奖剩余分数
	void setBigPrizeRemainScore(LONGLONG lRemainScore, bool bDirectShow);
	 
	//开启发奖动画:大奖分数、奖励倍数
	void openGivePrizeAni(LONGLONG lPrizeScore, int nPrizeTimes);
	//关闭发奖动画
	void closeGivePrizeAni();
	bool isOpenGivePrizeAni() { return m_bOpenGivePrizeAni;}
	//开启蓝7动画
	void openBlue7Ani(bool bOpen);
	//开启红7动画
	void openRed7Ani(bool bOpen);
	//开启logo动画
	void openLogoAni(bool bOpen);
	//开启宝箱动画
	void openBoxPrizeAni(bool bOpen);

private:
	//图标滚动定时器
	void rollSchedule(float dt);
	//蓝7边框灯闪烁定时器
	void flashBlue7FrameSchedule(float dt);

private:
	CPullMachGameLayer * m_pGameLayer;
	CPMResManager * m_pResManager;

	CPullMachRollNode * m_pRollNode[VER_LINE_NUM];	//滚动图标
	CPullMachBgSceneryNode * m_pBgScenery;	//背景图片
	CGraduallyShowSprite * m_pRemainPrizeTip;	//剩余大奖提示
	CCSprite * m_pRollBgLight;	//滚动背景发光
	CCSprite * m_pBetCoin[3];	//下注金币

	CCSprite * m_pPrizeTimesTip;	//大奖倍数提示
	CCSprite * m_pBlueLightFrame;	//蓝光边框
	CCSprite * m_pTreasureBox;	//宝箱
	CCSprite * m_pFlyGirl;	//飞行女孩
	CCSprite * m_pScroll;	//卷轴背景

	//界面数字
	CCLabelAtlas * m_pFontUserScore;	//玩家分数
	CCLabelAtlas * m_pFontAllWinLose;	//总输赢
	CContinueUpdateLabelAtlas * m_pFontCreditScore;	//上分分数
	CContinueUpdateLabelAtlas * m_pFontWinScore;	//赢得分数
	CPullMachPrizeRemainFontNode * m_pPrizeRemainScore;	//大奖剩余分数
	CCLabelAtlas * m_pFontPrizeTimes;	//大奖倍数
	CCLabelAtlas * m_pFontLogoNum[VER_LINE_NUM];	//竖行logo数

	//玩家信息
	CCSprite * m_pUserFace;	//玩家头像
	CCPoint m_FacePos;	//头像位置
	CCLabelTTF * m_pUserName;	//玩家姓名
	CCLabelTTF * m_pUserID;	//玩家ID

	//游戏变量
	bool m_bBigPrizeMode;	//是否大奖模式
	bool m_bOpenGivePrizeAni;	//是否开启发奖动画
	int m_nLogoCount[VER_LINE_NUM];	//各转轮已开出logo数目
	int m_nRollCount[VER_LINE_NUM];	//各转轮滚动次数
	bool m_bStopRoll[VER_LINE_NUM];	//是否已让相应转轮停止转动
	int m_nLightFrameIndex;	//当前发光框架索引
};

}
#endif	//_PullMach_RESULT_LAYER_H_
