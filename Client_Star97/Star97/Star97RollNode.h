#ifndef _STAR97_ROLL_NODE_H_
#define _STAR97_ROLL_NODE_H_

#include "LayerBase.h"
#include "Star97GameConstant.h"

namespace GameStar97
{
#define ICON_SPRITE_COUNT		14	//ÿͼ�㺬��ͼ����Ŀ

//��������
enum enRollType
{
	RollType_Normal = 0,	//��ͨ��������
	RollType_Star,	//���ǹ�������
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
	//���ͼ��ͼ����
	void randIconLayerResult(int nLayerID);
	//��ʼת��ͼ��ͼ�㣬������0~8����Ӧg_vObjCardResList��������
	void beginRollIconLayer(int nTypeID);
	//ֹͣת��ͼ��
	void endRollIconLayer();
	//���������н�
	void setStarPrize(int nStarLevel, int nStarIndex);
	//�Ƿ��ڹ���
	bool isRolling()
	{
		return m_bIsRolling;
	}
	bool canStopSound()
	{
		return m_bCanStopSound;
	}

private:
	//ͼ������ת����ʱ��
	void normalRollSchedule(float dt);
	//ͼ�����ת����ʱ��
	void finishRollSchedule(float dt);
	//�����н�ʱͼ��ת����ʱ��
	void starRollSchedule(float dt);
	//����ֹͣ����,����������ʱ����ת��ͼ�����,��С��3��,С��ICON_SPRITE_COUNT��
	float computeEndRollLength(const int nEndCount = 4);
	//����ָ��ͼ��ͼ���У����ڸ����������괦��,�����������
	int computeIndexAtWorldPos(const CCPoint & worldPos, int nLayerID);
	//ͼ��ͼ��λ��ѭ���ж�
	void checkIconLayerPos();
	//���ù�����ʶΪֹͣ����
	void setEndMarkRool();

private:
	static const CCSize m_contentSize;

	CResManager * m_pResManager;
	CCLayer * m_pStarLayer;	//����ͼ��
	CCLayer * m_pIconLayer1;	//ͼ��ͼ��1
	CCLayer * m_pIconLayer2;	//ͼ��ͼ��2
	CCSprite * m_pIconSprite1[ICON_SPRITE_COUNT];	//ͼ�꾫������1
	CCSprite * m_pIconSprite2[ICON_SPRITE_COUNT];	//ͼ�꾫������2
	int m_nResultTypeID;	//���ս��ID

	float m_fRollEndLength;	//ֹͣת������
	float m_fRollSumLength;	//�ۼ�ת������
	float m_fLayer1BeginPosY;	//ͼ��1��ʼֹͣת��ʱ����Yλ��
	float m_fLayer2BeginPosY;	//ͼ��2��ʼֹͣת��ʱ����Yλ��
	bool m_bIsRolling;	//�Ƿ��ڹ���
	bool m_bCanStopSound;	//����ֹͣ����
	enRollType m_eRollType;	//��ǰ��������
	float m_fEndAcceler;	//����ת�����ٶ�
	int m_nEndTime;	//����ת������
	int m_nRandBIndex;	//�����ʼ����
	int m_nTypePos[OBJECT_TYPE][6];	//����Ʒ��Ӧλ��
	int m_nTypeCount[OBJECT_TYPE];	//����Ʒ��Ŀ
};

}
#endif	//_STAR97_ROLL_NODE_H_
