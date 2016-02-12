#include "Stdafx.h"
#include "Star97GameConstant.h"
#include <cmath>

namespace CommonFunc
{

//计算组合数目
int NumOfCombine(int nSrcNum, int nCombineNum)
{
	//参数校验
	ASSERT(nCombineNum > 0 && nSrcNum > 0 && nSrcNum >= nCombineNum);
	if (nCombineNum <= 0 || nSrcNum <= 0 || nSrcNum < nCombineNum)
		return 0;

	if (nCombineNum == 1)
		return nSrcNum;
	else if (nCombineNum == nSrcNum)
		return 1;

	//调整乘法数目
	if ((nCombineNum << 1) > nSrcNum)
		nCombineNum = nSrcNum - nCombineNum;

	int nNumerator = 1;	//分子
	int nDenominator = 1;	//分母
	for (int i = 0; i < nCombineNum; ++i)
	{
		nNumerator *= (nSrcNum - i);
		nDenominator *= (nCombineNum - i);
	}

	return nNumerator / nDenominator;
}

//计算排列数目
int NumOfRank(int nTypeNum, int nRankNum)
{
	//参数校验
	ASSERT(nRankNum > 0 && nTypeNum > 0);
	if (nRankNum <= 0 || nTypeNum <= 0)
		return 0;

	//int nCount = 1;
	//for (int i = 0; i < nRankNum; ++i)
	//{
	//	nCount *= nTypeNum;
	//}

	return (int)std::pow((double)nTypeNum, nRankNum);
}

//判断单线类型
enSingleLineType GetSingleLineType(int nLineId, const tagAllCellResult & result)
{
	//参数校验
	ASSERT(nLineId >= 0 && nLineId < LINE_NUM);
	if (nLineId < 0 || nLineId >= LINE_NUM)
		return SLTypeNull;

	//取得该线数据
	BYTE cbResult[UNIT_LINE];
	ZeroMemory(cbResult, sizeof(cbResult));
	for (int i = 0; i < UNIT_LINE; ++i)
	{
		int nPos = g_nLinesPosIndex[nLineId][i];
		cbResult[i] = result.cbResult[nPos];
	}

	return GetSingleLineType(cbResult);
}

enSingleLineType GetSingleLineType(const BYTE cbResult[UNIT_LINE])
{
	if (cbResult[0] == 0)
		return SLTypeNull;

	//荔枝相关类型判断
	if (cbResult[0] == OTypeLeech)
	{
		if (cbResult[1] != OTypeLeech)
			return SLTypeLeechOne;
		else if (cbResult[2] != OTypeLeech)
			return SLTypeLeechTwo;
		else
			return SLTypeLeech;
	}
	else
	{
		//三个类型相同判断
		if (cbResult[0] == cbResult[1] && cbResult[1] == cbResult[2])
		{
			switch(cbResult[0])
			{
			case OTypeOrange:
				return SLTypeOrange;
			case OTypePawpaw:
				return SLTypePawpaw;
			case OTypeBell:
				return SLTypeBell;
			case OTypeTsama:
				return SLTypeTsama;
			case OTypeBarY:
				return SLTypeBarY;
			case OTypeBarR:
				return SLTypeBarR;
			case OTypeBarB:
				return SLTypeBarB;
			case OTypeNum7:
				return SLTypeNum7;
			default:
				ASSERT(false);
				return SLTypeNull;
			}
		}
		else	//3杂BAR判断
		{
			bool bAnyBar = true;
			for (int i = 0; i < UNIT_LINE; ++i)
			{
				if (cbResult[i] != OTypeBarY && cbResult[i] != OTypeBarR && cbResult[i] != OTypeBarB)
				{
					bAnyBar = false;
					break;
				}
			}

			if (bAnyBar)
				return SLTypeAnyBar;
		}
	}

	return SLTypeNull;
}

//判断全盘类型
enAllLineType GetAllLineType(const BYTE cbResult[CELL_NUM])
{
	//全部相同类型判断
	bool bAllSame = true;
	BYTE cbFirstType = cbResult[0];

	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (cbResult[i] == 0)	//排除无效类型
			return ALTypeNull;
		if (cbResult[i] != cbFirstType)
		{
			bAllSame = false;
			break;
		}
	}

	if (bAllSame)
	{
		switch(cbFirstType)
		{
		case OTypeOrange:
			return ALTypeOrange;
		case OTypePawpaw:
			return ALTypePawpaw;
		case OTypeBell:
			return ALTypeBell;
		case OTypeTsama:
			return ALTypeTsama;
		case OTypeBarY:
			return ALTypeBarY;
		case OTypeLeech:
			return ALTypeLeech;
		case OTypeBarR:
			return ALTypeBarR;
		case OTypeBarB:
			return ALTypeBarB;
		case OTypeNum7:
			return ALTypeNum7;
		default:
			ASSERT(false);
			return ALTypeNull;
		}
	}

	//全盘杂BAR判断
	bool bAllAnyBar = true;

	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (cbResult[i] != OTypeBarY && cbResult[i] != OTypeBarR && cbResult[i] != OTypeBarB)
		{
			bAllAnyBar = false;
			break;
		}
	}
	if (bAllAnyBar)
		return ALTypeAnyBar;

	//全盘水果判断
	bool bAllFruits = true;

	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (cbResult[i] != OTypeLeech && cbResult[i] != OTypeOrange &&
			cbResult[i] != OTypePawpaw && cbResult[i] != OTypeTsama)
		{
			bAllFruits = false;
			break;
		}
	}
	if (bAllFruits)
		return ALTypeFruits;

	return ALTypeNull;
}

//判断明星类型
enStarType GetStarType(const BYTE cbResult[CELL_NUM])
{
	if (cbResult[0] != 0)
		return StarTypeNull;
	for (int i = 2; i < CELL_NUM; ++i)
	{
		if (cbResult[i] != 0)
			return StarTypeNull;
	}

	if (cbResult[1] == StarTypeTimes2 || cbResult[1] == StarTypeTimes3 || cbResult[1] == StarTypeTimes4)
		return static_cast<enStarType>(cbResult[1]);
	else
		return StarTypeNull;
}

//计算数字7个数
int ComputeNum7Count(const BYTE cbResult[CELL_NUM])
{
	int nNum7Count = 0;
	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (cbResult[i] == OTypeNum7)
			++nNum7Count;
	}

	return nNum7Count;
}

}
