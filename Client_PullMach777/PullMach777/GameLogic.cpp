#include "StdAfx.h"
#include "GameLogic.h"

namespace GamePullMach777
{

//���캯��
CGameLogic::CGameLogic()
{
}
//��������
CGameLogic::~CGameLogic()
{
}


//�жϵ�������
enSingleLineType CGameLogic::GetSingleLineType(int nLineId, const BYTE cbAllResult[CELL_NUM])
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

//�ж��н�����
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

//ȡ�ò��н����
void CGameLogic::GetNoWinResult(BYTE cbResult[CELL_NUM])
{
	//��ʼ������ֵ
	ZeroMemory(cbResult, sizeof(cbResult[0]) * CELL_NUM);

	//������ø�ͼ��ֵ
	for (int i = 0; i < CELL_NUM; ++i)
	{
		cbResult[i] = rand() % OTypeRed7 + OTypeCoin;
	}

	//ȡ��ʹ����ĿС��2������Ʒ��������logo
	BYTE cbUnusedType[OBJECT_TYPE * 2] = {0};
	int nUnusedCount = 0;
	GetUnusedObjectType(cbResult, cbUnusedType, nUnusedCount);

	//�滻�н����͵�ͼ��
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

//ȡ��ָ�����͵��н����
void CGameLogic::GetDestTypeResult(enSingleLineType enDestType, BYTE cbResult[CELL_NUM])
{
	//����У��
	ASSERT(enDestType != SLTypeNull);
	if (enDestType == SLTypeNull)
		return ;

	//��ʼ������ֵ
	ZeroMemory(cbResult, sizeof(cbResult[0]) * CELL_NUM);

	//ȡ���н��߶�Ӧͼ������
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

	//������ø�ͼ��ֵ
	for (int i = 0; i < CELL_NUM; ++i)
	{
		cbResult[i] = rand() % OTypeRed7 + OTypeCoin;
	}

	//�����н����
	int nRandLineId = rand() % LINE_NUM;	//����н���ID
	for (int i = 0; i < UNIT_LINE; ++i)
	{
		int nPos = g_nLinesPosIndex[nRandLineId][i];
		cbResult[nPos] = cellType;
	}

	//ȡ��δʹ�õ���Ʒ����
	BYTE cbUnusedType[OBJECT_TYPE * 2] = {0};
	int nUnusedCount = 0;
	GetUnusedObjectType(cbResult, cbUnusedType, nUnusedCount);

	//�滻���н��ߵ��н����͵�ͼ��
	for (int i = 0; i < LINE_NUM; ++i)
	{
		if (i == nRandLineId)
			continue;
		else if (GetSingleLineType(i, cbResult) != SLTypeNull)
		{
			//����������滻Ϊ���н�����ͼ���λ��
			int nLinePos = rand() % UNIT_LINE;
			int nChangePos = g_nLinesPosIndex[i][nLinePos];	//�滻λ��
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

//��������logoͼ��
void CGameLogic::AddLogoToResult(BYTE cbResult[CELL_NUM], int nLogoChance, int nAllChance)
{
	//����У��
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

	//����н���������λ��
	bool bWinPos[CELL_NUM] = {0};
	int nWinLineId[LINE_NUM] = {0};	//�н���ID
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

	//ȡ�ÿ�����Ϊlogoͼ���λ��
	int nLogoUsedPos[CELL_NUM] = {0};
	int nLogoUsedCount = 0;
	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (bWinPos[i] == false)
		{
			nLogoUsedPos[nLogoUsedCount++] = i;
		}
	}

	//���¸�λ��Logo��ʾ
	for (int i = 0; i < nLogoUsedCount; ++i)
	{
		if ((rand() % nAllChance) <= (nLogoChance - 1))
		{
			cbResult[nLogoUsedPos[i]] = OTypeLogo;
		}
	}

	//ȡ��δʹ�õ���Ʒ����
	BYTE cbUnusedType[OBJECT_TYPE * 2] = {0};
	int nUnusedCount = 0;
	GetUnusedObjectType(cbResult, cbUnusedType, nUnusedCount);

	//������ܴ��ڵĶ���logo�н���
	for (int i = 0; i < LINE_NUM; ++i)
	{
		//�жϸ����Ƿ�Ϊԭʼ�н���
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
			//ȡ�ÿ������滻�н�ͼ���λ��
			int nLinePos = rand() % UNIT_LINE;
			int nChangePos = g_nLinesPosIndex[i][nLinePos];	//�滻λ��
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

//�������Ʒʹ����Ŀ
void CGameLogic::ComputeObjectUsedCount(const BYTE cbResult[CELL_NUM], int nCount[OBJECT_TYPE])
{
	//��ʼ����Ʒ��Ŀ
	ZeroMemory(nCount, sizeof(nCount[0]) * OBJECT_TYPE);

	for (int i = 0; i < CELL_NUM; ++i)
	{
		ASSERT(cbResult[i] >= 1 && cbResult[i] <= OBJECT_TYPE);
		++nCount[cbResult[i] - 1];
	}

	return ;
}

//ȡ��δʹ�õ���Ʒ����
void CGameLogic::GetUnusedObjectType(const BYTE cbResult[CELL_NUM], BYTE cbUnusedType[OBJECT_TYPE * 2],
	int &nUnusedCount)
{
	//�������Ʒʹ����Ŀ
	int nObjectCount[OBJECT_TYPE];
	ZeroMemory(nObjectCount, sizeof(nObjectCount));
	ComputeObjectUsedCount(cbResult, nObjectCount);

	//ȡ��ʹ����ĿС��2������Ʒ��������logo
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
	UpsetData(cbUnusedType, nUnusedCount);	//��������

	return ;
}

}