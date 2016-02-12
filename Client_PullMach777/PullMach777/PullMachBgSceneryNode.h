#ifndef _PullMach_BG_SCENERY_NODE_H_
#define _PullMach_BG_SCENERY_NODE_H_

namespace GamePullMach777
{

//背景图类型
enum enSceneryType
{
	Scenery_Normal = 0,	//正常背景图
	Scenery_BigPrize,	//大奖模式背景图
	Scenery_GivePrize,	//发奖背景图
};

class CPMResManager;

class CPullMachBgSceneryNode : public CCNode
{
public:
	static CPullMachBgSceneryNode * create(CPMResManager * pResManager);

	CPullMachBgSceneryNode(CPMResManager * pResManager);
	virtual ~CPullMachBgSceneryNode();

	virtual bool init();

public:
	//更新背景图
	void updateBgScenery(enSceneryType enType);
	//直接指定背景图
	void directSetBgScenery(enSceneryType enType);
	//取得背景类型对应资源ID
	static int getResIdBySceneryType(enSceneryType enType);

private:
	//降低透明度定时器
	void reduceOpacitySchedule(float dt);
	//增加透明度定时器
	void increaseOpacitySchedule(float dt);

private:
	CPMResManager * m_pResManager;
	CCSprite * m_pBgScenery;	//背景图
	enSceneryType m_enBgType;	//当前背景图类型
	bool m_bUpdating;	//是否正在更新
};

}

#endif	//_PullMach_BG_SCENERY_NODE_H_