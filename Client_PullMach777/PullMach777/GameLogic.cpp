#include "StdAfx.h"
#include "GameLogic.h"

namespace GamePullMach777
{

//构造函数
CGameLogic::CGameLogic()
{
}
//析构函数
CGameLogic::~CGameLogic()
{
}


//判断单线类型
enSingleLineType CGameLogic::GetSingleLineType(int nLineId, const BYTE cbAllResult[CELL_NUM])
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
		cbResult[i] = cbAllResult[nPos];
	}

	return GetSingleLineType(cbResult);
}

enSingleLineType CGameLogic::GetSingleLineType(const BYTE cbResult[UNIT_LINE])
{
	if (cbResult[0] == cbResult[1] && cbResult[1] == cbResult[2])
	{
		switch (cbResult[0])
		{
		case OTypeNull:
			return SLTypeNull;
		case OTypeCoin:
			return SLTypeCoin;
		case OTypeLeech:
			return SLTypeCoin;
		case OTypeTsama:
			return SLTypeCoin;
		case OTypeBell:
			return SLTypeCoin;
		case OTypeBlue7:
			return SLTypeCoin;
		case OTypeRed7:
			return SLTypeCoin;
		case OTypeLogo:
			return SLTypeCoin;
		default:
			return SLTypeNull;
		}
	}
	else
	{
		return SLTypeNull;
	}

}

//判断中奖类型
enSingleLineType CGameLogic::GetWinType(const BYTE cbAllResult[CELL_NUM])
{
	enSingleLineType lineType = SLTypeNull;

	for (int i = 0; i < LINE_NUM; ++i)
	{
		lineType = GetSingleLineType(i, cbAllResult);
		if (lineType != SLTypeNull)
			break;
	}

	return lineType;
}

//取得不中奖结果
void CGameLogic::GetNoWinResult(BYTE cbResult[CELL_NUM])
{
	//初始化返回值
	ZeroMemory(cbResult, sizeof(cbResult[0]) * CELL_NUM);

	//随机设置各图标值
	for (int i = 0; i < CELL_NUM; ++i)
	{
		cbResult[i] = rand() % OTypeRed7 + OTypeCoin;
	}

	//取得使用数目小于2个的物品，不包括logo
	BYTE cbUnusedType[OBJECT_TYPE * 2] = {0};
	int nUnusedCount = 0;
	GetUnusedObjectType(cbResult, cbUnusedType, nUnusedCount);

	//替换中奖类型的图标
	for (int i = 0; i < LINE_NUM; ++i)
	{
		if (GetSingleLineType(i, cbResult) != SLTypeNull)
		{
			int nPos = g_nLinesPosIndex[i][rand() % UNIT_LINE];
			cbResult[nPos] = cbUnusedType[--nUnusedCount];
		}
	}
	ASSERT(nUnusedCount >= 0);

	return ;
}

//取得指定类型的中奖结果
void CGameLogic::GetDestTypeResult(enSingleLineType enDestType, BYTE cbResult[CELL_NUM])
{
	//参数校验
	ASSERT(enDestType != SLTypeNull);
	if (enDestType == SLTypeNull)
		return ;

	//初始化返回值
	ZeroMemory(cbResult, sizeof(cbResult[0]) * CELL_NUM);

	//取得中奖线对应图标类型
	enObjectType cellType = OTypeNull;
	switch(enDestType)
	{
	case SLTypeCoin:
		cellType = OTypeCoin;
		break;
	case SLTypeLeech:
		cellType = OTypeLeech;
		break;
	case SLTypeTsama:
		cellType = OTypeTsama;
		break;
	case SLTypeBell:
		cellType = OTypeBell;
		break;
	case SLTypeBlue7:
		cellType = OTypeBlue7;
		break;
	case SLTypeRed7:
		cellType = OTypeRed7;
		break;
	case SLTypeLogo:
		cellType = OTypeLogo;
		break;
	}

	//随机设置各图标值
	for (int i = 0; i < CELL_NUM; ++i)
	{
		cbResult[i] = rand() % OTypeRed7 + OTypeCoin;
	}

	//设置中奖结果
	int nRandLineId = rand() % LINE_NUM;	//随机中奖线ID
	for (int i = 0; i < UNIT_LINE; ++i)
	{
		int nPos = g_nLinesPosIndex[nRandLineId][i];
		cbResult[nPos] = cellType;
	}

	//取得未使用的物品种类
	BYTE cbUnusedType[OBJECT_TYPE * 2] = {0};
	int nUnusedCount = 0;
	GetUnusedObjectType(cbResult, cbUnusedType, nUnusedCount);

	//替换非中奖线的中奖类型的图标
	for (int i = 0; i < LINE_NUM; ++i)
	{
		if (i == nRandLineId)
			continue;
		else if (GetSingleLineType(i, cbResult) != SLTypeNull)
		{
			//计算可用来替换为不中奖类型图标的位置
			int nLinePos = rand() % UNIT_LINE;
			int nChangePos = g_nLinesPosIndex[i][nLinePos];	//替换位置
			while (true)
			{
				bool bCanUsed = true;
				for (int j = 0; j < UNIT_LINE; ++j)
				{
					int nWinPos = g_nLinesPosIndex[nRandLineId][j];
					if (nWinPos == nChangePos)
					{
						bCanUsed = false;
						break;
					}
				}

				if (bCanUsed)
					break;
				else
				{
					nLinePos = (nLinePos + 1) % UNIT_LINE;
					nChangePos = g_nLinesPosIndex[i][nLinePos];
				}
			}

			cbResult[nChangePos] = cbUnusedType[--nUnusedCount];
		}
	}
	ASSERT(nUnusedCount >= 0);

	return ;
}

//给结果添加logo图标
void CGameLogic::AddLogoToResult(BYTE cbResult[CELL_NUM], int nLogoChance, int nAllChance)
{
	//参数校验
	ASSERT(nLogoChance >= 0 && nAllChance > 0 && nAllChance >= nLogoChance);
	ASSERT(nAllChance <= RAND_MAX);
	if (nLogoChance < 0 || nAllChance <= 0)
		return;
	if (nAllChance < nLogoChance)
		nLogoChance = nAllChance;
	if (nAllChance > RAND_MAX)
	{
		nLogoChance = nLogoChance / nAllChance * RAND_MAX;
		nAllChance = RAND_MAX;
	}

	//标记中奖类型所在位置
	bool bWinPos[CELL_NUM] = {0};
	int nWinLineId[LINE_NUM] = {0};	//中奖线ID
	int nWinLineCount = 0;
	for (int i = 0; i < LINE_NUM; ++i)
	{
		if (GetSingleLineType(i, cbResult) != SLTypeNull)
		{
			nWinLineId[nWinLineCount++] = i;
			for (int j = 0; j < UNIT_LINE; ++j)
			{
				int nPos = g_nLinesPosIndex[i][j];
				bWinPos[nPos] = true;
			}
		}
	}

	//取得可设置为logo图标的位置
	int nLogoUsedPos[CELL_NUM] = {0};
	int nLogoUsedCount = 0;
	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (bWinPos[i] == false)
		{
			nLogoUsedPos[nLogoUsedCount++] = i;
		}
	}

	//更新各位置Logo显示
	for (int i = 0; i < nLogoUsedCount; ++i)
	{
		if ((rand() % nAllChance) <= (nLogoChance - 1))
		{
			cbResult[nLogoUsedPos[i]] = OTypeLogo;
		}
	}

	//取得未使用的物品种类
	BYTE cbUnusedType[OBJECT_TYPE * 2] = {0};
	int nUnusedCount = 0;
	GetUnusedObjectType(cbResult, cbUnusedType, nUnusedCount);

	//处理可能存在的额外logo中奖线
	for (int i = 0; i < LINE_NUM; ++i)
	{
		//判断该线是否为原始中奖线
		bool bIsWinLine = false;
		for (int j = 0; j < nWinLineCount; ++j)
		{
			if (i == nWinLineId[j])
			{
				bIsWinLine = true;
				break;
			}
		}
		if (bIsWinLine)
			continue;

		if (GetSingleLineType(i, cbResult) != SLTypeNull)
		{
			//取得可用来替换中奖图标的位置
			int nLinePos = rand() % UNIT_LINE;
			int nChangePos = g_nLinesPosIndex[i][nLinePos];	//替换位置
			while (true)
			{
				bool bCanUsed = false;
				for (int j = 0; j < nLogoUsedCount; ++j)
				{
					if (nChangePos == nLogoUsedPos[j])
					{
						bCanUsed = true;
						break;
					}
				}

				if (bCanUsed)
					break;
				else
				{
					nLinePos = (nLinePos + 1) % UNIT_LINE;
					nChangePos = g_nLinesPosIndex[i][nLinePos];
				}
			}

			cbResult[nChangePos] = cbUnusedType[--nUnusedCount];
		}
	}

	return ;
}

//计算各物品使用数目
void CGameLogic::ComputeObjectUsedCount(const BYTE cbResult[CELL_NUM], int nCount[OBJECT_TYPE])
{
	//初始化物品数目
	ZeroMemory(nCount, sizeof(nCount[0]) * OBJECT_TYPE);

	for (int i = 0; i < CELL_NUM; ++i)
	{
		ASSERT(cbResult[i] >= 1 && cbResult[i] <= OBJECT_TYPE);
		++nCount[cbResult[i] - 1];
	}

	return ;
}

//取得未使用的物品类型
void CGameLogic::GetUnusedObjectType(const BYTE cbResult[CELL_NUM], BYTE cbUnusedType[OBJECT_TYPE * 2],
	int &nUnusedCount)
{
	//计算各物品使用数目
	int nObjectCount[OBJECT_TYPE];
	ZeroMemory(nObjectCount, sizeof(nObjectCount));
	ComputeObjectUsedCount(cbResult, nObjectCount);

	//取得使用数目小于2个的物品，不包括logo
	ZeroMemory(cbUnusedType, sizeof(cbUnusedType[0]) * (OBJECT_TYPE * 2));
	nUnusedCount = 0;

	for (int i = 0; i < OTypeRed7; ++i)
	{
		if (nObjectCount[i] >= 2)
			continue;
		else if (nObjectCount[i] == 1)
		{
			cbUnusedType[nUnusedCount++] = i + 1;
		}
		else if (nObjectCount[i] == 0)
		{
			cbUnusedType[nUnusedCount++] = i + 1;
			cbUnusedType[nUnusedCount++] = i + 1;
		}
	}
	ASSERT(nUnusedCount >= 3);
	UpsetData(cbUnusedType, nUnusedCount);	//混乱数据

	return ;
}

}