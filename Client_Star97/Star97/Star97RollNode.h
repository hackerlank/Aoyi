#ifndef _STAR97_ROLL_NODE_H_
#define _STAR97_ROLL_NODE_H_

#include "LayerBase.h"
#include "Star97GameConstant.h"

namespace GameStar97
{
#define ICON_SPRITE_COUNT		14	//每图层含有图标数目

//滚动类型
enum enRollType
{
	RollType_Normal = 0,	//普通滚动类型
	RollType_Star,	//明星滚动类型
};

class CResManager;
class CLayerBase;

class CStar97RollNode : public CCNode, public CLayerBase
{
public:
	static CStar97RollNode * create(CResManager * pResManager);

	CStar97RollNode(CResManager * pResManager);
	virtual ~CStar97RollNode();

	virtual bool init();
	virtual void visit();

public:
	void beforeDraw();
	void afterDraw();
	CCRect getViewRect();

public:
	//随机图标图层结果
	void randIconLayerResult(int nLayerID);
	//开始转动图标图层，参数：0~8，对应g_vObjCardResList中索引号
	void beginRollIconLayer(int nTypeID);
	//停止转动图标
	void endRollIconLayer();
	//设置明星中奖
	void setStarPrize(int nStarLevel, int nStarIndex);
	//是否在滚动
	bool isRolling()
	{
		return m_bIsRolling;
	}
	bool canStopSound()
	{
		return m_bCanStopSound;
	}

private:
	//图标正常转动定时器
	void normalRollSchedule(float dt);
	//图标结束转动定时器
	void finishRollSchedule(float dt);
	//明星中奖时图标转动定时器
	void starRollSchedule(float dt);
	//计算停止长度,参数：结束时还需转动图标个数,不小于3个,小于ICON_SPRITE_COUNT个
	float computeEndRollLength(const int nEndCount = 4);
	//计算指定图标图层中，处在给定世界坐标处的,精灵的索引号
	int computeIndexAtWorldPos(const CCPoint & worldPos, int nLayerID);
	//图标图层位置循环判断
	void checkIconLayerPos();
	//设置滚动标识为停止滚动
	void setEndMarkRool();

private:
	static const CCSize m_contentSize;

	CResManager * m_pResManager;
	CCLayer * m_pStarLayer;	//明星图层
	CCLayer * m_pIconLayer1;	//图标图层1
	CCLayer * m_pIconLayer2;	//图标图层2
	CCSprite * m_pIconSprite1[ICON_SPRITE_COUNT];	//图标精灵数组1
	CCSprite * m_pIconSprite2[ICON_SPRITE_COUNT];	//图标精灵数组2
	int m_nResultTypeID;	//最终结果ID

	float m_fRollEndLength;	//停止转动长度
	float m_fRollSumLength;	//累计转动长度
	float m_fLayer1BeginPosY;	//图层1开始停止转动时所处Y位置
	float m_fLayer2BeginPosY;	//图层2开始停止转动时所处Y位置
	bool m_bIsRolling;	//是否在滚动
	bool m_bCanStopSound;	//可以停止声音
	enRollType m_eRollType;	//当前滚动类型
	float m_fEndAcceler;	//结束转动加速度
	int m_nEndTime;	//结束转动次数
	int m_nRandBIndex;	//随机开始索引
	int m_nTypePos[OBJECT_TYPE][6];	//各物品对应位置
	int m_nTypeCount[OBJECT_TYPE];	//各物品数目
};

}
#endif	//_STAR97_ROLL_NODE_H_
