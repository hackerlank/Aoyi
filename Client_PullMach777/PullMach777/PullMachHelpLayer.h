#ifndef _PullMach_HELP_LAYER_H_
#define _PullMach_HELP_LAYER_H_

namespace GamePullMach777
{

class CPMResManager;

class CPullMachHelpLayer : public CCLayer
{
public:
	static CPullMachHelpLayer * create(CPMResManager * pResManager);

	CPullMachHelpLayer(CPMResManager * pResManager);
	virtual ~CPullMachHelpLayer();

	virtual bool init();
	virtual void onEnter();
	void OnTouchClose(CCObject *sender, CCControlEvent controlEvent);

	virtual void setVisible(bool visible);
private:
	CPMResManager * m_pResManager;
	CCScrollView * m_pScroll;
};

}

#endif	//_PullMach_HELP_LAYER_H_