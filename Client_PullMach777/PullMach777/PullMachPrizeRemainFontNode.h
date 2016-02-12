#ifndef _PULL_MACH_PRIZE_REMIAN_FONT_NODE_H_
#define _PULL_MACH_PRIZE_REMIAN_FONT_NODE_H_

namespace GamePullMach777
{

class CPMResManager;

class CPullMachPrizeRemainFontNode : public CCNode
{
public:
	static CPullMachPrizeRemainFontNode * create(CPMResManager * pResManager);

	CPullMachPrizeRemainFontNode(CPMResManager * pResManager);
	virtual ~CPullMachPrizeRemainFontNode();

	virtual bool init();

public:
	//设置当前分数
	void setScore(LONGLONG lScore);
	//持续更新显示函数:最终分数、最大刷新次数
	void continueUpdateScore(LONGLONG lShowScore, int nMaxUpdateCount);

private:
	//更新分数定时器
	void updateScoreSchedule(float dt);
	//显示分数
	void showScore(LONGLONG lScore);
	//计算分数对应各位数字:分数、各位数字数组、数字位数个数
	void computeBitNumOfScore(LONGLONG lScore, int nBitNum[20], int & nBitCount);

private:
	static const int sm_nFontResId[10];	//字体资源id

	CPMResManager* m_pResManager;
	CCSpriteBatchNode * m_pFontBatchNode;	//分数对应精灵集合类
	bool m_bUpdating;	//是否正在更新
	LONGLONG m_lNowScore;	//当前分数
	LONGLONG m_lDestScore;	//目标分数
	LONGLONG m_lEachUpdateScore;	//每次更新分数
	bool m_bAddScore;	//是否为加分模式
};

}

#endif	//_PULL_MACH_PRIZE_REMIAN_FONT_NODE_H_