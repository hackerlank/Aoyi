#ifndef _PullMach_BG_SCENERY_NODE_H_
#define _PullMach_BG_SCENERY_NODE_H_

namespace GamePullMach777
{

//����ͼ����
enum enSceneryType
{
	Scenery_Normal = 0,	//��������ͼ
	Scenery_BigPrize,	//��ģʽ����ͼ
	Scenery_GivePrize,	//��������ͼ
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
	//���±���ͼ
	void updateBgScenery(enSceneryType enType);
	//ֱ��ָ������ͼ
	void directSetBgScenery(enSceneryType enType);
	//ȡ�ñ������Ͷ�Ӧ��ԴID
	static int getResIdBySceneryType(enSceneryType enType);

private:
	//����͸���ȶ�ʱ��
	void reduceOpacitySchedule(float dt);
	//����͸���ȶ�ʱ��
	void increaseOpacitySchedule(float dt);

private:
	CPMResManager * m_pResManager;
	CCSprite * m_pBgScenery;	//����ͼ
	enSceneryType m_enBgType;	//��ǰ����ͼ����
	bool m_bUpdating;	//�Ƿ����ڸ���
};

}

#endif	//_PullMach_BG_SCENERY_NODE_H_