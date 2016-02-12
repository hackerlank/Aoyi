#ifndef _GRADUALLY_SHOW_SPRITE_H_
#define _GRADUALLY_SHOW_SPRITE_H_

namespace GamePullMach777
{

class CPMResManager;

class CGraduallyShowSprite : public CCSprite
{
public:
	static CGraduallyShowSprite * create(CPMResManager * pResManager, int type, int cfgid);
	CGraduallyShowSprite();
	virtual ~CGraduallyShowSprite();

public:
	//����ʾ:����Ƶ�ʣ�ÿ�����ӳ���,�Ƿ������ʾ
	void graduallyShow(float fUpdateRate, float fEachAddLength, bool bHoriShow = true);

private:
	//����ʾ��ʱ��
	void showSchedule(float dt);

private:
	bool m_bShowing;	//�Ƿ�������ʾ
	CCSize m_ShowAllSize;	//����ʾ�ߴ�
	float m_fEachAddLength;	//ÿ�����ӳ���
	bool m_bHoriShow;	//�Ƿ������ʾ
};

}

#endif	//_GRADUALLY_SHOW_SPRITE_H_
