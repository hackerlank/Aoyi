#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CMD_Game.h"

//////////////////////////////////////////////////////////////////////////

namespace GamePullMach777
{

#define ASSERT(cond)  CC_ASSERT(cond)
#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
#ifndef CopyMemory
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#endif

//物品种类
enum enObjectType
{
	OTypeNull = 0,			//无效类型
	OTypeCoin = 1,			//银币类型
	OTypeLeech = 2,			//荔枝
	OTypeTsama = 3,			//西瓜
	OTypeBell = 4,			//铃铛
	OTypeBlue7 = 5,			//蓝7
	OTypeRed7 = 6,			//红7
	OTypeLogo = 7,			//logo图标
};

//单线中奖类型
enum enSingleLineType
{
	SLTypeNull = 0,			//不中奖
	SLTypeCoin = 1,			//3银币
	SLTypeLeech = 2,		//3荔枝
	SLTypeTsama = 3,		//3西瓜
	SLTypeBell = 4,			//3铃铛
	SLTypeBlue7 = 5,		//3蓝7
	SLTypeRed7 = 6,			//3红7
	SLTypeLogo = 7,			//3个logo图标
};

//每条线包含图标对应位置索引,线从上到下
const int g_nLinesPosIndex[LINE_NUM][UNIT_LINE] =
{
	0, 1, 2,		//line 1
	3, 4, 5,		//line 2
	6, 7, 8,		//line 3
};

//游戏逻辑类
class CGameLogic
{
	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//功能函数
public:
	//判断单线类型
	enSingleLineType GetSingleLineType(int nLineId, const BYTE cbAllResult[CELL_NUM]);
	enSingleLineType GetSingleLineType(const BYTE cbResult[UNIT_LINE]);
	//判断中奖类型
	enSingleLineType GetWinType(const BYTE cbAllResult[CELL_NUM]);
	//取得不中奖结果
	void GetNoWinResult(BYTE cbResult[CELL_NUM]);
	//取得指定类型的中奖结果
	void GetDestTypeResult(enSingleLineType enDestType, BYTE cbResult[CELL_NUM]);
	//给结果添加logo图标,参数：结果、logo概率、总概率
	void AddLogoToResult(BYTE cbResult[CELL_NUM], int nLogoChance, int nAllChance);
	//计算各物品使用数目
	void ComputeObjectUsedCount(const BYTE cbResult[CELL_NUM], int nCount[OBJECT_TYPE]);
	//取得未使用的物品类型
	void GetUnusedObjectType(const BYTE cbResult[CELL_NUM], BYTE cbUnusedType[OBJECT_TYPE * 2], int &nUnusedCount);

	//通用函数
public:
	//混乱数据
	template <class T> inline void UpsetData( T * pData, int nCount);
};

template <class T> void CGameLogic::UpsetData( T * pData, int nCount)
{
	//参数校验
	ASSERT(pData != NULL && nCount > 0);
	if (pData == NULL || nCount <= 0)
		return ;

	T * pTemp = new T[nCount];
	ZeroMemory(pTemp, sizeof(T) * nCount);
	CopyMemory(pTemp, pData, sizeof(T) * nCount);

	int nIndex = 0;
	while (nIndex < nCount)
	{
		int nPos = rand() % (nCount - nIndex);
		pData[nIndex++] = pTemp[nPos];
		pTemp[nPos] = pTemp[nCount - nIndex];
	}

	delete [] pTemp;
	return ;
}

}
#endif