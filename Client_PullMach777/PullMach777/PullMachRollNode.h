#ifndef _PullMach_ROLL_NODE_H_
#define _PullMach_ROLL_NODE_H_

namespace GamePullMach777
{
#define ICON_SPRITE_COUNT		12	//ÿͼ�㺬��ͼ����Ŀ

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
	//���ͼ��ͼ����
	void randIconLayerResult(int nLayerID);
	//��ʼת��ͼ��ͼ��
	void beginRollByResult(BYTE cbResult[3]);
	//ֹͣת��ͼ��
	void endRollIconLayer();
	//�Ƿ��ڹ���
	bool isRolling(){ return m_bIsRolling; }
	//��˸ͼ��
	void flashIcon(int nPos);

private:
	//ͼ������ת����ʱ��
	void normalRollSchedule(float dt);
	//ͼ�����ת����ʱ��
	void finishRollSchedule(float dt);
	//����ֹͣ����,����������ʱ����ת��ͼ�����,��С��4��,С��ICON_SPRITE_COUNT��
	float computeEndRollLength(int nEndCount = 4);
	//����ָ��ͼ��ͼ���У����ڸ����������괦��,�����������
	int computeIndexAtWorldPos(const CCPoint & worldPos, int nLayerID);
	//ͼ��ͼ��λ��ѭ���ж�
	void checkIconLayerPos();
	//ֹͣ��˸ͼ��
	void stopFlashCallBack(CCNode * pSender);

private:
	static const CCSize m_contentSize;

	CPMResManager * m_pResManager;
	CCLayer * m_pIconLayer1;	//ͼ��+ͼ��1
	CCLayer * m_pIconLayer2;	//ͼ��ͼ��2
	CCSprite * m_pIconSprite1[ICON_SPRITE_COUNT];	//ͼ�꾫������1
	CCSprite * m_pIconSprite2[ICON_SPRITE_COUNT];	//ͼ�꾫������2
	CCSprite * m_pResultSprite[3];	//�������
	BYTE m_cbResult[3];	//���ս��

	float m_fRollEndLength;	//ֹͣת������
	float m_fRollSumLength;	//�ۼ�ת������
	float m_fLayer1BeginPosY;	//ͼ��1��ʼֹͣת��ʱ����Yλ��
	float m_fLayer2BeginPosY;	//ͼ��2��ʼֹͣת��ʱ����Yλ��
	bool m_bIsRolling;	//�Ƿ��ڹ���

};

}
#endif	//_PullMach_ROLL_NODE_H_
