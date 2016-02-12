#ifndef _PULL_MACH_MENU_H_
#define _PULL_MACH_MENU_H_

namespace GamePullMach777
{

class CPullMachMenu : public CCMenu
{
public:
	static CPullMachMenu * create();
	CPullMachMenu();
	~CPullMachMenu();

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

private:
	CCMenuItem* itemForTouchPM(CCTouch * touch);
};

}

#endif	//_PULL_MACH_MENU_H_