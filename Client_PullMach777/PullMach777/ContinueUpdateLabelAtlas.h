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
	//���÷���
	void setScore(LONGLONG lScore);
	//����������ʾ����:���շ��������ˢ�´���
	void continueUpdateScore(LONGLONG lShowScore, int nMaxUpdateCount);

private:
	//���·�����ʱ��
	void updateScoreSchedule(float dt);
	//����������ʾ����
	virtual void setString(const char *label);

private:
	bool m_bUpdating;	//�Ƿ����ڸ���
	LONGLONG m_lNowScore;	//��ǰ����
	LONGLONG m_lDestScore;	//Ŀ�����
	LONGLONG m_lEachUpdateScore;	//ÿ�θ��·���
	bool m_bAddScore;	//�Ƿ�Ϊ�ӷ�ģʽ
};

}

#endif	//_CONTINUE_UPDATE_LABEL_ATLAS_H_