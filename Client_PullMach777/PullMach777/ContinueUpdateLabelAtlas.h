#ifndef _CONTINUE_UPDATE_LABEL_ATLAS_H_
#define _CONTINUE_UPDATE_LABEL_ATLAS_H_

namespace GamePullMach777
{

class CPMResManager;

class CContinueUpdateLabelAtlas : public CCLabelAtlas
{
public:
	static CContinueUpdateLabelAtlas * create(CPMResManager * pResManager, int type, int cfgid);
	CContinueUpdateLabelAtlas();
	virtual ~CContinueUpdateLabelAtlas();

public:
	//设置分数
	void setScore(LONGLONG lScore);
	//持续更新显示函数:最终分数、最大刷新次数
	void continueUpdateScore(LONGLONG lShowScore, int nMaxUpdateCount);

private:
	//更新分数定时器
	void updateScoreSchedule(float dt);
	//重载设置显示函数
	virtual void setString(const char *label);

private:
	bool m_bUpdating;	//是否正在更新
	LONGLONG m_lNowScore;	//当前分数
	LONGLONG m_lDestScore;	//目标分数
	LONGLONG m_lEachUpdateScore;	//每次更新分数
	bool m_bAddScore;	//是否为加分模式
};

}

#endif	//_CONTINUE_UPDATE_LABEL_ATLAS_H_