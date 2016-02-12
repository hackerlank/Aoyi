#ifndef GAME_CONST_HEAD_FILE
#define GAME_CONST_HEAD_FILE

////////////////////游戏中常量定义、枚举类型声明/////////////////////////////

#include "CMD_Game.h"

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
	OTypeLeech = 1,			//荔枝
	OTypeOrange = 2,		//橙子
	OTypePawpaw = 3,		//木瓜
	OTypeBell = 4,			//铃铛
	OTypeTsama = 5,			//西瓜
	OTypeBarY = 6,			//黄BAR
	OTypeBarR = 7,			//红BAR
	OTypeBarB = 8,			//蓝BAR
	OTypeNum7 = 9,			//数字7
};

const enObjectType g_FruitsType[4] = {OTypeLeech, OTypeOrange, OTypePawpaw, OTypeTsama,};	//水果类型
const enObjectType g_BarsType[3] = {OTypeBarY, OTypeBarR, OTypeBarB,};	//BAR类型

//明星种类
enum enStarType
{
	StarTypeNull = 0,		//无效值
	StarTypeTimes2 = 2,		//2倍明星
	StarTypeTimes3 = 3,		//3倍明星
	StarTypeTimes4 = 4,		//4倍明星
};

//单线中奖类型
enum enSingleLineType
{
	SLTypeNull = 0,			//不中奖
	SLTypeLeechOne = 1,		//1荔枝2其他
	SLTypeLeechTwo = 2,		//2荔枝1其他
	SLTypeLeech = 3,		//3荔枝
	SLTypeOrange = 4,		//3橙子
	SLTypeAnyBar = 5,		//3个不完全相同BAR
	SLTypePawpaw = 6,		//3木瓜
	SLTypeBell = 7,			//3铃铛
	SLTypeTsama = 8,		//3西瓜
	SLTypeBarY = 9,			//3黄BAR
	SLTypeBarR = 10,		//3红BAR
	SLTypeBarB = 11,		//3蓝BAR
	SLTypeNum7 = 12,		//3数字7
};

//全盘中奖类型
enum enAllLineType
{
	ALTypeNull = 0,			//不中奖
	ALTypeFruits = 1,		//全盘水果
	ALTypeAnyBar = 2,		//全盘杂BAR
	ALTypeOrange = 3,		//全盘橙子
	ALTypePawpaw = 4,		//全盘木瓜
	ALTypeBell = 5,			//全盘铃铛
	ALTypeTsama = 6,		//全盘西瓜
	ALTypeBarY = 7,			//全盘黄BAR
	ALTypeLeech = 8,		//全盘荔枝
	ALTypeBarR = 9,			//全盘红BAR
	ALTypeBarB = 10,		//全盘蓝BAR
	ALTypeNum7 = 11,		//全盘数字7
};

//线中奖粗略类型
enum enLineWinRoughType
{
	LWRTypeLeech1 = 0,		//1荔枝2其他
	LWRTypeLeech2 = 1,		//2荔枝1其他
	LWRTypeSame3 = 2,		//3个相同
};

//单线中奖对应奖励倍率
const int g_nWinSingleLine[13] =
{	0,
	2, 5, 10, 10,
	10, 14, 18, 20,
	30, 50, 70, 80,
};

//全盘中奖对应奖励倍率
const int g_nWinAllLine[12] =
{	0,
	15, 40, 40, 40,
	40, 60, 60, 80,
	90, 100, 200,
};

//开出7的数目对应奖励倍率
const int g_nWinNum7Count[10] =
{
	0, 0, 2, 5,
	20, 40, 60, 80,
	100, 200,
};

//每条线包含图标对应位置索引
const int g_nLinesPosIndex[LINE_NUM][UNIT_LINE] =
{
	3, 4, 5,		//line A
	0, 1, 2,		//line B
	6, 7, 8,		//line C
	0, 4, 8,		//line D
	6, 4, 2,		//line E
	0, 3, 6,		//line F
	1, 4, 7,		//line G
	2, 5, 8,		//line H
};

//单线各类型开出的万分比
const int g_nRatioSingleLine[13] =
{	0,
	4000, 1600, 800, 800,
	800, 571, 444, 400,
	266, 160, 114, 45,
};

//结果中每个图标类型
struct tagAllCellResult
{
	BYTE cbResult[CELL_NUM];
};

/////////////////////////////通用函数//////////////////////////////////////
namespace CommonFunc
{
	//计算组合数目
	int NumOfCombine(int nSrcNum, int nCombineNum);
	//计算排列数目
	int NumOfRank(int nTypeNum, int nRankNum);
	//混乱数据
	template <class T> inline void UpsetData( T * pData, int nCount);

	//判断单线类型
	enSingleLineType GetSingleLineType(int nLineId, const tagAllCellResult & result);
	enSingleLineType GetSingleLineType(const BYTE cbResult[UNIT_LINE]);

	//判断全盘类型
	enAllLineType GetAllLineType(const BYTE cbResult[CELL_NUM]);
	inline enAllLineType GetAllLineType(const tagAllCellResult & result)
	{
		return GetAllLineType(result.cbResult);
	}

	//判断明星类型
	enStarType GetStarType(const BYTE cbResult[CELL_NUM]);
	inline enStarType GetStarType(const tagAllCellResult & result)
	{
		return GetStarType(result.cbResult);
	}

	//计算数字7个数
	int ComputeNum7Count(const BYTE cbResult[CELL_NUM]);
}

template <class T> void CommonFunc::UpsetData( T * pData, int nCount)
{
	//参数校验
	CC_ASSERT(pData != NULL && nCount > 0);
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

#endif	//GAME_CONST_HEAD_FILE