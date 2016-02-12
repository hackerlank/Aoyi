#ifndef _PullMach_ROLL_NODE_H_
#define _PullMach_ROLL_NODE_H_

namespace GamePullMach777
{
#define ICON_SPRITE_COUNT		12	//每图层含有图标数目

class CPMResManager;

class CPullMachRollNode : public CCNode
{
public:
	static CPullMachRollNode * create(CPMResManager * pResManager);

	CPullMachRollNode(CPMResManager * pResManager);
	virtual ~CPullMachRollNode();

	virtual bool init();
	virtual void visit();

public:
	void beforeDraw();
	void afterDraw();
	CCRect getViewRect();

public:
	//随机图标图层结果
	void randIconLayerResult(int nLayerID);
	//开始转动图标图层
	void beginRollByResult(BYTE cbResult[3]);
	//停止转动图标
	void endRollIconLayer();
	//是否在滚动
	bool isRolling(){ return m_bIsRolling; }
	//闪烁图标
	void flashIcon(int nPos);

private:
	//图标正常转动定时器
	void normalRollSchedule(float dt);
	//图标结束转动定时器
	void finishRollSchedule(float dt);
	//计算停止长度,参数：结束时还需转动图标个数,不小于4个,小于ICON_SPRITE_COUNT个
	float computeEndRollLength(int nEndCount = 4);
	//计算指定图标图层中，处在给定世界坐标处的,精灵的索引号
	int computeIndexAtWorldPos(const CCPoint & worldPos, int nLayerID);
	//图标图层位置循环判断
	void checkIconLayerPos();
	//停止闪烁图标
	void stopFlashCallBack(CCNode * pSender);

private:
	static const CCSize m_contentSize;

	CPMResManager * m_pResManager;
	CCLayer * m_pIconLayer1;	//图标+图层1
	CCLayer * m_pIconLayer2;	//图标图层2
	CCSprite * m_pIconSprite1[ICON_SPRITE_COUNT];	//图标精灵数组1
	CCSprite * m_pIconSprite2[ICON_SPRITE_COUNT];	//图标精灵数组2
	CCSprite * m_pResultSprite[3];	//结果精灵
	BYTE m_cbResult[3];	//最终结果

	float m_fRollEndLength;	//停止转动长度
	float m_fRollSumLength;	//累计转动长度
	float m_fLayer1BeginPosY;	//图层1开始停止转动时所处Y位置
	float m_fLayer2BeginPosY;	//图层2开始停止转动时所处Y位置
	bool m_bIsRolling;	//是否在滚动

};

}
#endif	//_PullMach_ROLL_NODE_H_
