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
	//逐渐显示:更新频率，每次增加长度,是否横向显示
	void graduallyShow(float fUpdateRate, float fEachAddLength, bool bHoriShow = true);

private:
	//逐渐显示定时器
	void showSchedule(float dt);

private:
	bool m_bShowing;	//是否正在显示
	CCSize m_ShowAllSize;	//总显示尺寸
	float m_fEachAddLength;	//每次增加长度
	bool m_bHoriShow;	//是否横向显示
};

}

#endif	//_GRADUALLY_SHOW_SPRITE_H_
