#ifndef _STAR97_RESULT_LAYER_H_
#define _STAR97_RESULT_LAYER_H_

#include "CMD_Game.h"
#include "Star97RollNode.h"
#include "LayerBase.h"

namespace GameStar97
{

class CStar97GameLayer;
class CResManager;
class CStar97RollNode;
class CLayerBase;

//游戏状态
enum enGameStatusInfo
{
	GSInfo_Bet = 0,	//上分状态
	GSInfo_Roll,	//滚动状态
	GSInfo_GameOver,	//失败
	GSInfo_YouWin,	//胜利
};

class CStar97ResultLayer : public CCLayer, public CLayerBase
{
public:
	static CStar97ResultLayer * create(CStar97GameLayer * pGameLayer, CResManager * pResManager);

	CStar97ResultLayer(CStar97GameLayer * pGameLayer, CResManager * pResManager);
	~CStar97ResultLayer();

	virtual bool init();

public:
	//设置上分分数
	void setCreditScore(LONGLONG lScore);
	//玩家押注
	void betScore(int nBetScore, int nLineBet);
	//设置自然得分
	void setNormalScore(LONGLONG lScore);
	//设置彩金得分
	void setHandselScore(LONGLONG lScore, BYTE cbHandselRatio);
	//取得明星有效次数
	int getStarCount()
	{
		return m_nStarCount;
	}

	//设置滚动结果
	void setRollResult(BYTE cbResult[CELL_NUM], BYTE cbStarRatio);
	//设置各图标滚动次数
	void setRollCount(int nIndex, int nCount);
	//停止所有图标转动
	void stopAllIconRoll();
	//停止图标滚动
	void stopIconRoll(const bool bStop[CELL_NUM]);

	//设置闪烁框显示
	void setFlashFrameVisible(bool bFlashFrameVisible[CELL_NUM]);
	//设置中奖线显示
	void setLinesVisible(bool bLinesVisible[LINE_NUM]);
	//设置当前状态信息
	void setStatusInfo(enGameStatusInfo eStatus);
	//设置彩金池分数
	void setHandselPoolScore(LONGLONG lScore);
	//复位得分信息
	void resetScoreInfo();
	//更新明星次数
	void updateStarCount();
	//设置自动游戏提示
	void setAutoTipsVisible(bool bVisible);

	//设置明星中奖分数
	void setStarWinScore(LONGLONG lWinScore, int nStarLevel, int nStarCount);
	//设置普通中奖分数
	void setNormalWinScore(const tagWinScore & winScore, int nCellScore, BYTE cbHandselRatio);

private:
	//图标滚动定时器
	void rollSchedule(float dt);
	//更新彩金得分,先更新彩金分（如果有的话），再更新普通的分，最后更新额外得分
	void handelScoreSchedule(float dt);
	//更新自然得分
	void normalScoreSchedule(float dt);
	//更新额外得分
	void extraScoreSchedule(float dt);
	//计算更新频率分数
	void computeUpdateRate(const LONGLONG lDestScore, LONGLONG & lRateScore);
	//设置额外奖显示数字
	void setExtraFontScore(LONGLONG lScore);
	//计算分数对应每位数字
	void computeScoreNum(LONGLONG lScore, int nNum[], int nCout);

private:
	CStar97GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	CStar97RollNode * m_pRollNode[CELL_NUM];	//滚动图标
	CCSprite * m_pFlashFrame[CELL_NUM];	//闪烁框
	CCSprite * m_pWinLine[LINE_NUM];	//中奖线
	CCSprite * m_pAutoTips;	//自动玩提示
	CCSprite * m_pHaleSprite;	//1/2图标
	CCSprite * m_pBonusSprite;	//bonus图标
	CCSprite * m_pExtraBg;	//额外奖励背景
	CCSprite * m_pStatusTips;	//游戏状态提示

	//界面数字
	CCLabelAtlas * m_pFontHandselPool;	//彩金池分数
	CCLabelAtlas * m_pFontExtraScore[5];	//额外得分
	CCLabelAtlas * m_pFontHandselScore;	//彩金得分
	CCLabelAtlas * m_pFontNormalScore;	//自然得分
	CCLabelAtlas * m_pFontCreditScore;	//上分分数
	CCLabelAtlas * m_pFontBetScore;	//下注分数
	CCLabelAtlas * m_pFontLineScore[LINE_NUM];	//每线下注分数
	CCLabelAtlas * m_pFontStarRatio;	//明星倍数
	CCLabelAtlas * m_pFontStarCount;	//明星次数

	//分数变量
	LONGLONG m_lHandselPool;	//彩金池分数
	LONGLONG m_lExtraScore;	//额外得分
	LONGLONG m_lHandselScore;	//彩金得分
	LONGLONG m_lNormalScore;	//自然得分
	LONGLONG m_lCreditScore;	//上分分数
	int m_nBetScore;	//押注分数
	LONGLONG m_lUpdateRate;	//更新频率，每次更新多少分
	int m_nStarLevel;	//明星等级
	int m_nStarCount;	//明星次数

	//滚动相关
	enRollType m_eRollType;	//当前滚动类型
	int m_nRollCount[CELL_NUM];	//各图标滚动次数
	bool m_bStopRoll[CELL_NUM];	//是否已让相应图标停止转动


};

}
#endif	//_STAR97_RESULT_LAYER_H_
