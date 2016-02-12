#ifndef GAME_CONST_HEAD_FILE
#define GAME_CONST_HEAD_FILE

////////////////////��Ϸ�г������塢ö����������/////////////////////////////

#include "CMD_Game.h"

#define ASSERT(cond)  CC_ASSERT(cond)
#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
#ifndef CopyMemory
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#endif

//��Ʒ����
enum enObjectType
{
	OTypeNull = 0,			//��Ч����
	OTypeLeech = 1,			//��֦
	OTypeOrange = 2,		//����
	OTypePawpaw = 3,		//ľ��
	OTypeBell = 4,			//����
	OTypeTsama = 5,			//����
	OTypeBarY = 6,			//��BAR
	OTypeBarR = 7,			//��BAR
	OTypeBarB = 8,			//��BAR
	OTypeNum7 = 9,			//����7
};

const enObjectType g_FruitsType[4] = {OTypeLeech, OTypeOrange, OTypePawpaw, OTypeTsama,};	//ˮ������
const enObjectType g_BarsType[3] = {OTypeBarY, OTypeBarR, OTypeBarB,};	//BAR����

//��������
enum enStarType
{
	StarTypeNull = 0,		//��Чֵ
	StarTypeTimes2 = 2,		//2������
	StarTypeTimes3 = 3,		//3������
	StarTypeTimes4 = 4,		//4������
};

//�����н�����
enum enSingleLineType
{
	SLTypeNull = 0,			//���н�
	SLTypeLeechOne = 1,		//1��֦2����
	SLTypeLeechTwo = 2,		//2��֦1����
	SLTypeLeech = 3,		//3��֦
	SLTypeOrange = 4,		//3����
	SLTypeAnyBar = 5,		//3������ȫ��ͬBAR
	SLTypePawpaw = 6,		//3ľ��
	SLTypeBell = 7,			//3����
	SLTypeTsama = 8,		//3����
	SLTypeBarY = 9,			//3��BAR
	SLTypeBarR = 10,		//3��BAR
	SLTypeBarB = 11,		//3��BAR
	SLTypeNum7 = 12,		//3����7
};

//ȫ���н�����
enum enAllLineType
{
	ALTypeNull = 0,			//���н�
	ALTypeFruits = 1,		//ȫ��ˮ��
	ALTypeAnyBar = 2,		//ȫ����BAR
	ALTypeOrange = 3,		//ȫ�̳���
	ALTypePawpaw = 4,		//ȫ��ľ��
	ALTypeBell = 5,			//ȫ������
	ALTypeTsama = 6,		//ȫ������
	ALTypeBarY = 7,			//ȫ�̻�BAR
	ALTypeLeech = 8,		//ȫ����֦
	ALTypeBarR = 9,			//ȫ�̺�BAR
	ALTypeBarB = 10,		//ȫ����BAR
	ALTypeNum7 = 11,		//ȫ������7
};

//���н���������
enum enLineWinRoughType
{
	LWRTypeLeech1 = 0,		//1��֦2����
	LWRTypeLeech2 = 1,		//2��֦1����
	LWRTypeSame3 = 2,		//3����ͬ
};

//�����н���Ӧ��������
const int g_nWinSingleLine[13] =
{	0,
	2, 5, 10, 10,
	10, 14, 18, 20,
	30, 50, 70, 80,
};

//ȫ���н���Ӧ��������
const int g_nWinAllLine[12] =
{	0,
	15, 40, 40, 40,
	40, 60, 60, 80,
	90, 100, 200,
};

//����7����Ŀ��Ӧ��������
const int g_nWinNum7Count[10] =
{
	0, 0, 2, 5,
	20, 40, 60, 80,
	100, 200,
};

//ÿ���߰���ͼ���Ӧλ������
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

//���߸����Ϳ�������ֱ�
const int g_nRatioSingleLine[13] =
{	0,
	4000, 1600, 800, 800,
	800, 571, 444, 400,
	266, 160, 114, 45,
};

//�����ÿ��ͼ������
struct tagAllCellResult
{
	BYTE cbResult[CELL_NUM];
};

/////////////////////////////ͨ�ú���//////////////////////////////////////
namespace CommonFunc
{
	//���������Ŀ
	int NumOfCombine(int nSrcNum, int nCombineNum);
	//����������Ŀ
	int NumOfRank(int nTypeNum, int nRankNum);
	//��������
	template <class T> inline void UpsetData( T * pData, int nCount);

	//�жϵ�������
	enSingleLineType GetSingleLineType(int nLineId, const tagAllCellResult & result);
	enSingleLineType GetSingleLineType(const BYTE cbResult[UNIT_LINE]);

	//�ж�ȫ������
	enAllLineType GetAllLineType(const BYTE cbResult[CELL_NUM]);
	inline enAllLineType GetAllLineType(const tagAllCellResult & result)
	{
		return GetAllLineType(result.cbResult);
	}

	//�ж���������
	enStarType GetStarType(const BYTE cbResult[CELL_NUM]);
	inline enStarType GetStarType(const tagAllCellResult & result)
	{
		return GetStarType(result.cbResult);
	}

	//��������7����
	int ComputeNum7Count(const BYTE cbResult[CELL_NUM]);
}

template <class T> void CommonFunc::UpsetData( T * pData, int nCount)
{
	//����У��
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