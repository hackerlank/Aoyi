#include "stdafx.h"
#include "GameLogic.h"

//定义point用减法
CCPoint operator-(const CCPoint& left, const CCPoint& right)
{
	return CCPoint(left.x - right.x, left.y - right.y);
}

//定义point用点积
float PointDot(const CCPoint& left, const CCPoint& right)
{
	return left.x * right.x + left.y * right.y;
}

//取得下注区域赔率
WORD CGameLogic::GetAreaLossPercent(BYTE cbArea)
{
	if (cbArea == JA_RED)
		return 2;
	else if (cbArea == JA_BLUE)
		return 2;
	else
	{
		CC_ASSERT(false);
		return 0;
	}
}

//判断点是否在三角形内部
bool CGameLogic::IsPointInTriangle(const CCPoint & desPoint, const tagTriangle & desTriangle)
{
	//定义向量
	CCPoint point0 = desTriangle.point[1] - desTriangle.point[0];
	CCPoint point1 = desTriangle.point[2] - desTriangle.point[0];
	CCPoint point2 = desPoint - desTriangle.point[0];

	//定义点积
	float fDot00 = PointDot(point0, point0);
	float fDot01 = PointDot(point0, point1);
	float fDot02 = PointDot(point0, point2);
	float fDot11 = PointDot(point1, point1);
	float fDot12 = PointDot(point1, point2);

	//定义除数
	float fInverDeno = 1 / (fDot00 * fDot11 - fDot01 * fDot01);

	//判断比例
	float u = (fDot11 * fDot02 - fDot01 * fDot12) * fInverDeno;
	if (u < 0 || u > 1)
	{
		return false;
	}

	float v = (fDot00 * fDot12 - fDot01 * fDot02) * fInverDeno;
	if (v < 0 || v > 1)
	{
		return false;
	}

	return u + v <= 1;
}


