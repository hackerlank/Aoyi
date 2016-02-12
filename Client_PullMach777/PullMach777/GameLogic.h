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

//��Ʒ����
enum enObjectType
{
	OTypeNull = 0,			//��Ч����
	OTypeCoin = 1,			//��������
	OTypeLeech = 2,			//��֦
	OTypeTsama = 3,			//����
	OTypeBell = 4,			//����
	OTypeBlue7 = 5,			//��7
	OTypeRed7 = 6,			//��7
	OTypeLogo = 7,			//logoͼ��
};

//�����н�����
enum enSingleLineType
{
	SLTypeNull = 0,			//���н�
	SLTypeCoin = 1,			//3����
	SLTypeLeech = 2,		//3��֦
	SLTypeTsama = 3,		//3����
	SLTypeBell = 4,			//3����
	SLTypeBlue7 = 5,		//3��7
	SLTypeRed7 = 6,			//3��7
	SLTypeLogo = 7,			//3��logoͼ��
};

//ÿ���߰���ͼ���Ӧλ������,�ߴ��ϵ���
const int g_nLinesPosIndex[LINE_NUM][UNIT_LINE] =
{
	0, 1, 2,		//line 1
	3, 4, 5,		//line 2
	6, 7, 8,		//line 3
};

//��Ϸ�߼���
class CGameLogic
{
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ܺ���
public:
	//�жϵ�������
	enSingleLineType GetSingleLineType(int nLineId, const BYTE cbAllResult[CELL_NUM]);
	enSingleLineType GetSingleLineType(const BYTE cbResult[UNIT_LINE]);
	//�ж��н�����
	enSingleLineType GetWinType(const BYTE cbAllResult[CELL_NUM]);
	//ȡ�ò��н����
	void GetNoWinResult(BYTE cbResult[CELL_NUM]);
	//ȡ��ָ�����͵��н����
	void GetDestTypeResult(enSingleLineType enDestType, BYTE cbResult[CELL_NUM]);
	//��������logoͼ��,�����������logo���ʡ��ܸ���
	void AddLogoToResult(BYTE cbResult[CELL_NUM], int nLogoChance, int nAllChance);
	//�������Ʒʹ����Ŀ
	void ComputeObjectUsedCount(const BYTE cbResult[CELL_NUM], int nCount[OBJECT_TYPE]);
	//ȡ��δʹ�õ���Ʒ����
	void GetUnusedObjectType(const BYTE cbResult[CELL_NUM], BYTE cbUnusedType[OBJECT_TYPE * 2], int &nUnusedCount);

	//ͨ�ú���
public:
	//��������
	template <class T> inline void UpsetData( T * pData, int nCount);
};

template <class T> void CGameLogic::UpsetData( T * pData, int nCount)
{
	//����У��
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