#ifndef _OX_CJ4_GAME_LOGIC_HEAD_FILE_
#define _OX_CJ4_GAME_LOGIC_HEAD_FILE_

#pragma once

#include "CMD_OxCJ4.h"

#ifndef ZeroMemory
#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))
#endif

#ifndef CopyMemory
#define CopyMemory(Destination, Source, Length) memcpy((Destination), (Source), (Length))
#endif

#ifndef ASSERT
#define ASSERT CC_ASSERT
#endif

namespace GameOxCJ4
{

//////////////////////////////////////////////////////////////////////////
//�궨��
#define ALL_COUNT					54									//ȫ���˿�

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�˿�����
enum emCardType
{
	CT_NIU0 = 0,			//��ţ
	CT_NIU1,				//ţ1
	CT_NIU2,				//ţ2
	CT_NIU3,				//ţ3
	CT_NIU4,				//ţ4
	CT_NIU5,				//ţ5
	CT_NIU6,				//ţ6
	CT_NIU7,				//ţ7
	CT_NIU8,				//ţ8
	CT_NIU9,				//ţ9
	CT_NIUNIU = 10,			//ţţ
	CT_NIUWH_TWODOUBLE,		//�����廨ţ
	CT_NIUWH_THREE,			//�����廨ţ
	CT_NIUWH_HULU,			//��«�廨ţ
	CT_NIUWH_TWOGUI,		//�����廨ţ
	CT_NIUWH_FOUR,			//��ը�廨ţ
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[ALL_COUNT];				//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�ж�����
	BYTE CheckCardType(const BYTE cbCheckData[], BYTE cbCheckCount);
	//ȡ������������
	BYTE GetMaxCardType(BYTE cbCardData[], BYTE cbCardCount);
	BYTE GetCardIndex(const BYTE cbData);

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//���ܺ���
public:
	//ȡ��������Ŀ���˿�
	void GetDataWithGiveCard(const BYTE cbSrcData[], const BYTE cbSrcCount, BYTE cbDestData[], BYTE cbDestCount);
	//�滻�廨ţΪ����ţţ
	bool ChangeWuhuaToNiuniu(const BYTE cbCanUseData[], const BYTE cbCanUseCount, BYTE cbChangeData[], BYTE cbChangeCount);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstData[], const BYTE cbNextData[], BYTE cbFirstCount, BYTE cbNextCount);
	//�Ƴ��˿�
	BYTE RemoveCard(const BYTE cbRemoveData[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//�ж��Ƿ����ȡ����Ӧ�廨ţ����
	bool CanGetDestWuhuaType(const BYTE cbCheckData[], const BYTE cbCheckCount, const BYTE cbCheckType);
	//��ֵ����
	void SortValue(BYTE cbData[], int nCount);
	//�Ƿ�Ϊţ
	bool IsOxType(const BYTE cbCardData[], BYTE cbCardCount);

	//��������
private:
	//������ֵ����˿�
	BYTE FindMaxValueData(const BYTE cbFindData[], BYTE cbFindCount);
};
//////////////////////////////////////////////////////////////////////////

}
#endif	//_OX_CJ4_GAME_LOGIC_HEAD_FILE_
