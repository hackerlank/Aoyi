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
	//���õ�ǰ����
	void setScore(LONGLONG lScore);
	//����������ʾ����:���շ��������ˢ�´���
	void continueUpdateScore(LONGLONG lShowScore, int nMaxUpdateCount);

private:
	//���·�����ʱ��
	void updateScoreSchedule(float dt);
	//��ʾ����
	void showScore(LONGLONG lScore);
	//���������Ӧ��λ����:��������λ�������顢����λ������
	void computeBitNumOfScore(LONGLONG lScore, int nBitNum[20], int & nBitCount);

private:
	static const int sm_nFontResId[10];	//������Դid

	CPMResManager* m_pResManager;
	CCSpriteBatchNode * m_pFontBatchNode;	//������Ӧ���鼯����
	bool m_bUpdating;	//�Ƿ����ڸ���
	LONGLONG m_lNowScore;	//��ǰ����
	LONGLONG m_lDestScore;	//Ŀ�����
	LONGLONG m_lEachUpdateScore;	//ÿ�θ��·���
	bool m_bAddScore;	//�Ƿ�Ϊ�ӷ�ģʽ
};

}

#endif	//_PULL_MACH_PRIZE_REMIAN_FONT_NODE_H_