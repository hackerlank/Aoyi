#include "Stdafx.h"
#include "Star97GameConstant.h"
#include <cmath>

namespace CommonFunc
{

//���������Ŀ
int NumOfCombine(int nSrcNum, int nCombineNum)
{
	//����У��
	ASSERT(nCombineNum > 0 && nSrcNum > 0 && nSrcNum >= nCombineNum);
	if (nCombineNum <= 0 || nSrcNum <= 0 || nSrcNum < nCombineNum)
		return 0;

	if (nCombineNum == 1)
		return nSrcNum;
	else if (nCombineNum == nSrcNum)
		return 1;

	//�����˷���Ŀ
	if ((nCombineNum << 1) > nSrcNum)
		nCombineNum = nSrcNum - nCombineNum;

	int nNumerator = 1;	//����
	int nDenominator = 1;	//��ĸ
	for (int i = 0; i < nCombineNum; ++i)
	{
		nNumerator *= (nSrcNum - i);
		nDenominator *= (nCombineNum - i);
	}

	return nNumerator / nDenominator;
}

//����������Ŀ
int NumOfRank(int nTypeNum, int nRankNum)
{
	//����У��
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

//�жϵ�������
enSingleLineType GetSingleLineType(int nLineId, const tagAllCellResult & result)
{
	//����У��
	ASSERT(nLineId >= 0 && nLineId < LINE_NUM);
	if (nLineId < 0 || nLineId >= LINE_NUM)
		return SLTypeNull;

	//ȡ�ø�������
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

	//��֦��������ж�
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
		//����������ͬ�ж�
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
		else	//3��BAR�ж�
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

//�ж�ȫ������
enAllLineType GetAllLineType(const BYTE cbResult[CELL_NUM])
{
	//ȫ����ͬ�����ж�
	bool bAllSame = true;
	BYTE cbFirstType = cbResult[0];

	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (cbResult[i] == 0)	//�ų���Ч����
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

	//ȫ����BAR�ж�
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

	//ȫ��ˮ���ж�
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

//�ж���������
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

//��������7����
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
