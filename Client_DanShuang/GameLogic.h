#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////

//定义三角形结构
struct tagTriangle
{
	CCPoint point[3];
};

//游戏逻辑
class CGameLogic
{
public:
	//取得下注区域赔率
	WORD GetAreaLossPercent(BYTE cbArea);
	//判断点是否在三角形内部
	bool IsPointInTriangle(const CCPoint & desPoint, const tagTriangle & desTriangle);

};
//////////////////////////////////////////////////////////////////////////

#endif
