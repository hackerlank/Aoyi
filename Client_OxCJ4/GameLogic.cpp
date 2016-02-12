#include "StdAfx.h"
#include "GameLogic.h"

namespace GameOxCJ4
{

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE CGameLogic::m_cbCardListData[ALL_COUNT] = 
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x41,0x42,
};

BYTE CGameLogic::GetCardIndex(const BYTE cbData)
{
	BYTE cbBegin = 0;
	BYTE cbEnd = ALL_COUNT - 1;

	while (cbBegin <= cbEnd)
	{
		BYTE cbMid = (cbBegin + cbEnd) >> 1;
		if (m_cbCardListData[cbMid] == cbData)
			return cbMid;
		else if (m_cbCardListData[cbMid] < cbData)
			cbBegin = cbMid + 1;
		else
			cbEnd = cbMid - 1;
	}

	ASSERT(false);
	return 0xff;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{

}

//��������
CGameLogic::~CGameLogic()
{

}

//�ж�����
BYTE CGameLogic::CheckCardType(const BYTE cbCheckData[], BYTE cbCheckCount)
{
	ASSERT(cbCheckCount == MAX_COUNT && cbCheckData[0] != 0);
	if (cbCheckCount !=  MAX_COUNT || cbCheckData[0] == 0)
		return CT_NIU0;

	//�������
	BYTE cbTempData[MAX_COUNT];
	ZeroMemory(cbTempData, sizeof(cbTempData));
	CopyMemory(cbTempData, cbCheckData, sizeof(cbCheckData[0]) * cbCheckCount);

	//�廨ţ�ж�
	bool bIsWuhua = true;
	for (BYTE i = 0; i < CountArray(cbTempData); ++i)
	{
		cbTempData[i] = GetCardLogicValue(cbTempData[i]);
		if (cbTempData[i] <= 10)
		{
			bIsWuhua = false;
			break;
		}
	}

	if (bIsWuhua == true)
	{
		BYTE cbWuhua[MAX_COUNT];
		ZeroMemory(cbWuhua, sizeof(cbWuhua));
		CopyMemory(cbWuhua, cbTempData, sizeof(cbTempData));
		SortValue(cbWuhua, CountArray(cbWuhua));
		
		if (cbWuhua[0] <= 13)
		{
			//���㵥�ơ����ӡ���������������Ŀ
			BYTE cbSameCount[4];
			ZeroMemory(cbSameCount, sizeof(cbSameCount));

			BYTE cbIndex = 0;
			while (cbIndex < CountArray(cbWuhua))
			{
				BYTE cbCount = 1;	//��ͬ��Ŀ
				for (BYTE i = cbIndex + 1; i < CountArray(cbWuhua); ++i)
				{
					if (cbWuhua[cbIndex] == cbWuhua[i])
						++cbCount;
					else
						break;
				}

				ASSERT(cbCount <= 4);
				if (cbCount > 4)
					break;

				++cbSameCount[cbCount - 1];
				cbIndex += cbCount;
			}

			//�����ж�
			if (cbSameCount[0] == 1 && cbSameCount[1] == 2)
				return CT_NIUWH_TWODOUBLE;
			else if (cbSameCount[0] == 2 && cbSameCount[2] == 1)
				return CT_NIUWH_THREE;
			else if (cbSameCount[1] == 1 && cbSameCount[2] == 1)
				return CT_NIUWH_HULU;
			else if(cbSameCount[0] == 1 && cbSameCount[3] == 1)
				return CT_NIUWH_FOUR;
			else
				ASSERT(false);
		}
		else if (cbWuhua[0] == 17 && cbWuhua[1] == 16)
		{
			if (cbWuhua[2] == cbWuhua[3] && cbWuhua[3] == cbWuhua[4])
				return CT_NIUWH_TWOGUI;
		}
	}

	//ţֵ�����ж�
	if (IsOxType(cbCheckData, 3) == true)
	{
		if (IsOxType(&cbCheckData[3], 2) == true)
			return CT_NIUNIU;

		//����ţֵ
		int nSumValue = 0;
		for (BYTE i = 3; i < cbCheckCount; ++i)
		{
			BYTE cbOxValue = GetCardValue(cbCheckData[i]);
			if (cbOxValue < 10)
			{
				nSumValue += cbOxValue;
			}
		}

		nSumValue %= 10;

		switch(nSumValue)
		{
		case 0:
			ASSERT(false);
			return CT_NIUNIU;
		case 1:
			return CT_NIU1;
		case 2:
			return CT_NIU2;
		case 3:
			return CT_NIU3;
		case 4:
			return CT_NIU4;
		case 5:
			return CT_NIU5;
		case 6:
			return CT_NIU6;
		case 7:
			return CT_NIU7;
		case 8:
			return CT_NIU8;
		case 9:
			return CT_NIU9;
		}
	}

	return CT_NIU0;
}

//ȡ������������
BYTE CGameLogic::GetMaxCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//����У��
	ASSERT(cbCardCount == MAX_COUNT);
	if (cbCardCount != MAX_COUNT)
		return CT_NIU0;

	//ţţΪ���
	SortCardList(cbCardData, cbCardCount);
	BYTE cbCardType = CheckCardType(cbCardData, cbCardCount);
	if (cbCardType >= CT_NIUNIU)
		return cbCardType;

	//ȡ�����ֵ
	BYTE cbMaxCardData[MAX_COUNT];
	ZeroMemory(cbMaxCardData, sizeof(cbMaxCardData));
	CopyMemory(cbMaxCardData, cbCardData, sizeof(cbCardData[0]) * cbCardCount);
	BYTE cbMaxCardType = CT_NIU0;
	for (BYTE i = 0; i < cbCardCount; ++i)
	{
		for (BYTE j = i + 1; j < cbCardCount; ++j)
		{
			//��������
			BYTE cbCheckData[MAX_COUNT];
			ZeroMemory(cbCheckData, sizeof(cbCheckData));
			for (BYTE k = 0, cbIndex = 0; k < cbCardCount; ++k)
			{
				if (k == i)
					cbCheckData[3] = cbCardData[k];
				else if (k == j)
					cbCheckData[4] = cbCardData[k];
				else
					cbCheckData[cbIndex++] = cbCardData[k];
			}

			//�����ж�
			BYTE cbCheckType = CheckCardType(cbCheckData, cbCardCount);
			if (cbCheckType > cbMaxCardType)
			{
				CopyMemory(cbMaxCardData, cbCheckData, sizeof(cbCheckData));
				cbMaxCardType = cbCheckType;
			}
		}
	}

	CopyMemory(cbCardData, cbMaxCardData, sizeof(cbMaxCardData));
	return cbMaxCardType;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount <= ALL_COUNT);
	//ת����ֵ
	BYTE cbLogicValue[ALL_COUNT];
	ZeroMemory(cbLogicValue, sizeof(cbLogicValue));
	for (BYTE i=0;i<cbCardCount;i++)
		cbLogicValue[i] = GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����У��
	ASSERT(cbBufferCount <= ALL_COUNT);
	if (cbBufferCount > ALL_COUNT)
		cbBufferCount = ALL_COUNT;

	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	ZeroMemory(cbCardData, sizeof(cbCardData));
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//ȡ��������Ŀ���˿�
void CGameLogic::GetDataWithGiveCard(const BYTE cbSrcData[], const BYTE cbSrcCount, BYTE cbDestData[], BYTE cbDestCount)
{
	//��ʼ�����
	ZeroMemory(cbDestData, sizeof(cbDestData[0]) * cbDestCount);

	//����У��
	ASSERT(cbDestCount <= cbSrcCount && cbSrcCount <= ALL_COUNT);
	if (cbDestCount > cbSrcCount || cbSrcCount > ALL_COUNT)
		return ;

	//�������
	BYTE cbTempData[ALL_COUNT];
	ZeroMemory(cbTempData, sizeof(cbTempData));
	CopyMemory(cbTempData, cbSrcData, sizeof(cbSrcData[0]) * cbSrcCount);

	BYTE cbGetedCount = 0, cbLeftCount = cbSrcCount;
	while (cbGetedCount < cbDestCount)
	{
		BYTE cbIndex = rand() % cbLeftCount;
		cbDestData[cbGetedCount++] = cbTempData[cbIndex];
		cbTempData[cbIndex] = cbTempData[--cbLeftCount];
	}

	return ;
}

//�滻�廨ţΪ����ţţ,����Ϊ�滻J��Q��K������Ϊ1~10��ֵ���˿�
bool CGameLogic::ChangeWuhuaToNiuniu(const BYTE cbCanUseData[], const BYTE cbCanUseCount, BYTE cbChangeData[], BYTE cbChangeCount)
{
	//����У��
	ASSERT(cbChangeCount == MAX_COUNT && cbCanUseCount > 0);
	if (cbChangeCount != MAX_COUNT || cbCanUseCount <= 0)
		return false;

	//�廨ţ�����ж�
	if (CheckCardType(cbChangeData, cbChangeCount) <= CT_NIUNIU)
	{
		ASSERT(false);
		return false;
	}

	//���������ṹ
	struct strChange
	{
		BYTE cbData[4];
		BYTE cbCount;
	};

	//���������˿�
	strChange ChangeArray[11];
	ZeroMemory(ChangeArray, sizeof(ChangeArray));
	BYTE cbRealUsefulCount = 0;
	for (BYTE i = 0; i < cbCanUseCount; ++i)
	{
		BYTE cbLogicValue = GetCardLogicValue(cbCanUseData[i]);
		if (cbLogicValue <= 10)
		{
			BYTE & cbTempCount = ChangeArray[cbLogicValue].cbCount;
			if (cbTempCount >= 4)
			{
				ASSERT(false);
				continue;
			}
			ChangeArray[cbLogicValue].cbData[cbTempCount] = cbCanUseData[i];
			++cbTempCount;
			++cbRealUsefulCount;
		}
	}
	if (cbRealUsefulCount <= 0)
		return false;

	//����ȡ�����滻����
	bool bTenValueCanUse = false;
	BYTE cbTenData = 0;
	if (ChangeArray[10].cbCount > 0)
	{
		bTenValueCanUse = true;
		cbTenData = ChangeArray[10].cbData[rand() % (ChangeArray[10].cbCount)];
		cbRealUsefulCount -= ChangeArray[10].cbCount;
		ZeroMemory(&ChangeArray[10], sizeof(ChangeArray[10]));
	}

	//����3���滻���ԡ���ֵ10���˿���ĿΪ0
	//Ҫʹ3����ֵ1~9��������Ϊţ������ΪAAB��ABC�����������ΪAAAʱ��Aֵ��Ϊ3.33��6.66��10��Aֵ�����ڡ�
	if (cbRealUsefulCount >= 3)
	{
		for (BYTE cbFIndex = 1; cbFIndex < 10; ++cbFIndex)
		{
			if (ChangeArray[cbFIndex].cbCount == 0)
				continue;
			for (BYTE cbSIndex = cbFIndex; cbSIndex < 10; ++cbSIndex)
			{
				if (cbSIndex == cbFIndex)
				{
					if (ChangeArray[cbSIndex].cbCount < 2)
						continue;
				}
				else if (ChangeArray[cbSIndex].cbCount == 0)
					continue;

				for (BYTE cbTIndex = cbSIndex; cbTIndex < 10; ++cbTIndex)
				{
					if (cbTIndex == cbSIndex)
					{
						if (ChangeArray[cbTIndex].cbCount < 2)
							continue;
					}
					else if (ChangeArray[cbTIndex].cbCount == 0)
						continue;

					//�ҵ��˿������
					if ((cbFIndex + cbSIndex + cbTIndex) % 10 == 0)
					{
						//����滻λ��
						ASSERT(cbChangeCount >= 3);
						BYTE cbIndex1 = rand() % (cbChangeCount - 1) + 1;
						BYTE cbIndex2 = cbIndex1 - 1;
						BYTE cbIndex3 = (cbIndex1 + 1) % cbChangeCount;

						if (cbFIndex != cbSIndex && cbFIndex != cbTIndex && cbSIndex != cbTIndex)
						{
							cbChangeData[cbIndex1] = ChangeArray[cbFIndex].cbData[rand() % (ChangeArray[cbFIndex].cbCount)];
							cbChangeData[cbIndex2] = ChangeArray[cbSIndex].cbData[rand() % (ChangeArray[cbSIndex].cbCount)];
							cbChangeData[cbIndex3] = ChangeArray[cbTIndex].cbData[rand() % (ChangeArray[cbTIndex].cbCount)];
						}
						else if (cbFIndex == cbSIndex)
						{
							cbChangeData[cbIndex1] = ChangeArray[cbFIndex].cbData[0];
							cbChangeData[cbIndex2] = ChangeArray[cbFIndex].cbData[ChangeArray[cbFIndex].cbCount - 1];
							cbChangeData[cbIndex3] = ChangeArray[cbTIndex].cbData[rand() % (ChangeArray[cbTIndex].cbCount)];
						}
						else if (cbFIndex == cbTIndex)
						{
							cbChangeData[cbIndex1] = ChangeArray[cbFIndex].cbData[0];
							cbChangeData[cbIndex2] = ChangeArray[cbSIndex].cbData[rand() % (ChangeArray[cbSIndex].cbCount)];
							cbChangeData[cbIndex3] = ChangeArray[cbFIndex].cbData[ChangeArray[cbFIndex].cbCount - 1];
						}
						else if (cbSIndex == cbTIndex)
						{
							cbChangeData[cbIndex1] = ChangeArray[cbFIndex].cbData[rand() % (ChangeArray[cbFIndex].cbCount)];
							cbChangeData[cbIndex2] = ChangeArray[cbSIndex].cbData[0];
							cbChangeData[cbIndex3] = ChangeArray[cbSIndex].cbData[ChangeArray[cbSIndex].cbCount - 1];
						}
						else
						{
							ASSERT(false);
							return false;
						}

						SortCardList(cbChangeData, cbChangeCount);
						return true;
					}
				}
			}
		}
	}

	//��ȡ2���滻���ԡ���ֵ10���˿���ĿΪ0
	if (cbRealUsefulCount >= 2)
	{
		if (rand() % 2 == 0)
		{
			//��ȡ�м����������Ӳ���
			BYTE cbFIndex = 5;
			BYTE cbSIndex = 5;
			if (ChangeArray[5].cbCount < 2)
			{
				cbFIndex = 6;
				cbSIndex = 4;
			}

			while (cbFIndex <= 9 && cbSIndex >= 1)
			{
				if (ChangeArray[cbFIndex].cbCount > 0 && ChangeArray[cbSIndex].cbCount > 0)
				{
					//����滻λ��
					BYTE cbIndex1 = rand() % cbChangeCount;
					BYTE cbIndex2 = (cbIndex1 + 1) % cbChangeCount;

					if (cbFIndex == 5 && cbSIndex == 5)
					{
						cbChangeData[cbIndex1] = ChangeArray[5].cbData[0];
						cbChangeData[cbIndex2] = ChangeArray[5].cbData[ChangeArray[5].cbCount - 1];
					}
					else
					{
						cbChangeData[cbIndex1] = ChangeArray[cbFIndex].cbData[rand() % (ChangeArray[cbFIndex].cbCount)];
						cbChangeData[cbIndex2] = ChangeArray[cbSIndex].cbData[rand() % (ChangeArray[cbSIndex].cbCount)];
					}

					SortCardList(cbChangeData, cbChangeCount);
					return true;
				}
				else
				{
					++cbFIndex;
					--cbSIndex;
				}
			}
		}
		else
		{
			//��ȡ�������м����Ӳ���
			BYTE cbFIndex = 9;
			BYTE cbSIndex = 1;
			BYTE cbFEnd = 5;
			BYTE cbSEnd = 5;

			if (ChangeArray[5].cbCount < 2)
			{
				cbFEnd = 6;
				cbSEnd = 4;
			}

			while (cbFIndex >= cbFEnd && cbSIndex <= cbSEnd)
			{
				if (ChangeArray[cbFIndex].cbCount > 0 && ChangeArray[cbSIndex].cbCount > 0)
				{
					//����滻λ��
					BYTE cbIndex1 = rand() % cbChangeCount;
					BYTE cbIndex2 = (cbIndex1 + 1) % cbChangeCount;

					if (cbFIndex == 5 && cbSIndex == 5)
					{
						cbChangeData[cbIndex1] = ChangeArray[5].cbData[0];
						cbChangeData[cbIndex2] = ChangeArray[5].cbData[ChangeArray[5].cbCount - 1];
					}
					else
					{
						cbChangeData[cbIndex1] = ChangeArray[cbFIndex].cbData[rand() % (ChangeArray[cbFIndex].cbCount)];
						cbChangeData[cbIndex2] = ChangeArray[cbSIndex].cbData[rand() % (ChangeArray[cbSIndex].cbCount)];
					}

					SortCardList(cbChangeData, cbChangeCount);
					return true;
				}
				else
				{
					--cbFIndex;
					++cbSIndex;
				}
			}
		}
	}

	//��ȡ�����滻����
	if (bTenValueCanUse == true)
	{
		cbChangeData[rand() % cbChangeCount] = cbTenData;
		SortCardList(cbChangeData, cbChangeCount);
		return true;
	}

	//δ�ҵ����滻��
	return false;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstData[], const BYTE cbNextData[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//����У��
	ASSERT(cbFirstCount == MAX_COUNT && cbNextCount == MAX_COUNT);
	if (cbFirstCount != MAX_COUNT || cbNextCount != MAX_COUNT)
		return false;

	//ȡ������
	BYTE cbFirstType = CheckCardType(cbFirstData, cbFirstCount);
	if (cbFirstType > CT_NIUNIU)
		cbFirstType = CT_NIUNIU;
	BYTE cbNextType = CheckCardType(cbNextData, cbNextCount);
	if (cbNextType > CT_NIUNIU)
		cbNextType = CT_NIUNIU;

	if (cbFirstType != cbNextType)
		return (cbNextType > cbFirstType);

	//������ͬ
	BYTE cbFirstMaxData = FindMaxValueData(cbFirstData, cbFirstCount);
	BYTE cbNextMaxData = FindMaxValueData(cbNextData, cbNextCount);

	if (GetCardLogicValue(cbFirstMaxData) != GetCardLogicValue(cbNextMaxData))
	{
		return (GetCardLogicValue(cbNextMaxData) > GetCardLogicValue(cbFirstMaxData));
	}
	else
	{
		return (GetCardColor(cbNextMaxData) > GetCardColor(cbFirstMaxData));
	}
}

//�Ƴ��˿�
BYTE CGameLogic::RemoveCard(const BYTE cbRemoveData[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//����У��
	ASSERT(cbRemoveCount != 0 && cbCardCount != 0 && cbRemoveCount <= cbCardCount);
	if (cbRemoveCount == 0 || cbCardCount == 0 || cbRemoveCount > cbCardCount)
		return cbCardCount;

	//�������
	BYTE cbTempData[ALL_COUNT];
	ZeroMemory(cbTempData, sizeof(cbTempData));
	CopyMemory(cbTempData, cbCardData, sizeof(cbCardData[0]) * cbCardCount);

	//����ɾ����
	for (BYTE i = 0; i < cbRemoveCount; ++i)
	{
		for (BYTE j = 0; j < cbCardCount; ++j)
		{
			if (cbRemoveData[i] == cbTempData[j])
			{
				cbTempData[j] = 0xFF;
				break;
			}
		}
	}

	//ɾ������
	BYTE cbRemainCount = 0;
	for (BYTE i = 0; i < cbCardCount; ++i)
	{
		if (cbTempData[i] != 0xFF)
			cbCardData[cbRemainCount++] = cbTempData[i];
	}

	return cbRemainCount;
}

//�ж��Ƿ����ȡ����Ӧ�廨ţ����
bool CGameLogic::CanGetDestWuhuaType(const BYTE cbCheckData[], const BYTE cbCheckCount, const BYTE cbCheckType)
{
	//����У��
	ASSERT(cbCheckType > CT_NIUNIU && cbCheckType <= CT_NIUWH_FOUR);
	if (cbCheckType <= CT_NIUNIU || cbCheckType > CT_NIUWH_FOUR)
		return false;

	ASSERT(cbCheckCount <= ALL_COUNT);
	if (cbCheckCount > ALL_COUNT || cbCheckCount < MAX_COUNT)
		return false;

	//�������
	BYTE cbTempData[ALL_COUNT];
	ZeroMemory(cbTempData, sizeof(cbTempData));
	CopyMemory(cbTempData, cbCheckData, sizeof(cbCheckData[0]) * cbCheckCount);
	SortCardList(cbTempData, cbCheckCount);

	if (GetCardColor(cbTempData[2]) == 0x40)	//���ֻ����2�Ź���
		return false;

	//�����˿���Ŀ
	BYTE cbSameCount[4];	//�ֱ�Ϊ���ơ����ӡ����š�������Ŀ
	ZeroMemory(cbSameCount, sizeof(cbSameCount));

	if (cbCheckType == CT_NIUWH_TWOGUI)
	{
		//���������廨ţ����
		if (cbTempData[0] != 0x42 || cbTempData[1] != 0x41)
			return false;

		BYTE cbIndex = 2;
		while (cbIndex < cbCheckCount)
		{
			BYTE cbCount = 1;
			BYTE cbCompareValue = GetCardValue(cbTempData[cbIndex]);
			if (cbCompareValue <= 10)
				break;

			for (++cbIndex; cbIndex < cbCheckCount;++cbIndex)
			{
				if (GetCardValue(cbTempData[cbIndex]) == cbCompareValue)
					++cbCount;
				else
					break;
			}

			ASSERT(cbCount <= 4);
			if (cbCount > 4)
				break;

			++cbSameCount[cbCount - 1];
		}

		//�����ж�
		if (cbSameCount[2] + cbSameCount[3] >= 1)
			return true;
		else
			return false;
	}
	else	//�����廨ţ�Ʒ���
	{
		BYTE cbIndex = 0;
		for (BYTE i = 0; i < cbCheckCount; ++i)
		{
			if (GetCardColor(cbTempData[i]) == 0x40)
				++cbIndex;
			else
				break;
		}

		while (cbIndex < cbCheckCount)
		{
			BYTE cbCount = 1;
			BYTE cbCompareValue = GetCardValue(cbTempData[cbIndex]);
			if (cbCompareValue <= 10)
				break;

			for (++cbIndex; cbIndex < cbCheckCount;++cbIndex)
			{
				if (GetCardValue(cbTempData[cbIndex]) == cbCompareValue)
					++cbCount;
				else
					break;
			}

			ASSERT(cbCount <= 4);
			if (cbCount > 4)
				break;

			++cbSameCount[cbCount - 1];
		}

		//�����ж�
		if (cbCheckType == CT_NIUWH_TWODOUBLE)	//�����廨ţ
		{
			//ȡ�ô��ڵ���2�ŵ���Ŀ
			BYTE cbGreater2 = cbSameCount[1] + cbSameCount[2] + cbSameCount[3];

			if (cbGreater2 > 2)
				return true;
			else if (cbGreater2 == 2 && cbSameCount[0] >= 1)
				return true;
			else
				return false;
		}
		else if (cbCheckType == CT_NIUWH_THREE)	//�����廨ţ
		{
			//ȡ�ô��ڵ���3�š�С��3����Ŀ
			BYTE cbGreater3 = cbSameCount[2] + cbSameCount[3];
			BYTE cbSmaller3 = cbSameCount[0] + cbSameCount[1];

			if (cbGreater3 >= 3)
				return true;
			else if (cbGreater3 == 2)
			{
				if (cbSmaller3 >= 1)
					return true;
				else
					return false;
			}
			else if (cbGreater3 == 1 && cbSmaller3 >= 2)
				return true;
			else
				return false;
		}
		else if (cbCheckType == CT_NIUWH_HULU)	//��«�廨ţ
		{
			//ȡ�ô��ڵ���3�š�������Ŀ
			BYTE cbGreater3 = cbSameCount[2] + cbSameCount[3];
			BYTE cbEqual2 = cbSameCount[1];

			if (cbGreater3 >= 2)
				return true;
			else if (cbGreater3 == 1 && cbEqual2 >= 1)
				return true;
			else
				return false;
		}
		else if (cbCheckType == CT_NIUWH_FOUR)	//��ը�廨ţ
		{
			//ȡ�ô��ڵ���4�š�С��4����Ŀ
			BYTE cbGreater4 = cbSameCount[3];
			BYTE cbSmaller4 = cbSameCount[0] + cbSameCount[1] + cbSameCount[2];

			if (cbGreater4 >= 2)
				return true;
			else if (cbGreater4 == 1 && cbSmaller4 >= 1)
				return true;
			else
				return false;
		}
		else
		{
			ASSERT(false);
			return false;
		}
	}

	ASSERT(false);
	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	if (cbCardColor==0x40) return cbCardValue+15;
	return cbCardValue;
}

//�Ƿ�Ϊţ
bool CGameLogic::IsOxType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//����У��
	ASSERT(cbCardCount == 3 || cbCardCount == 2);
	ASSERT(cbCardData != NULL);
	if (cbCardData == NULL || (cbCardCount != 3 && cbCardCount != 2))
		return false;

	BYTE cbTempData[3];
	ZeroMemory(cbTempData, sizeof(cbTempData));
	CopyMemory(cbTempData, cbCardData, sizeof(cbCardData[0]) * cbCardCount);
	SortCardList(cbTempData, cbCardCount);

	if (GetCardColor(cbTempData[0]) == 0x40)
		return true;

	//����ţֵ
	int nSumValue = 0;
	for (BYTE i = 0; i < cbCardCount; ++i)
	{
		BYTE cbOxValue = GetCardValue(cbTempData[i]);
		if (cbOxValue < 10)
		{
			nSumValue += cbOxValue;
		}
	}

	return (nSumValue % 10 == 0);
}

//������ֵ����˿�
BYTE CGameLogic::FindMaxValueData(const BYTE cbFindData[], BYTE cbFindCount)
{
	//����У��
	if (cbFindCount <= 0)
	{
		ASSERT(false);
		return 0;
	}

	BYTE cbMaxData = 0;
	BYTE cbMaxValue = 0;

	for (BYTE i = 0; i < cbFindCount; ++i)
	{
		BYTE cbTempValue = GetCardLogicValue(cbFindData[i]);
		if (cbTempValue > cbMaxValue)
		{
			cbMaxData = cbFindData[i];
			cbMaxValue = cbTempValue;
		}
		else if(cbTempValue == cbMaxValue)
		{
			if (GetCardColor(cbFindData[i]) > GetCardColor(cbMaxData))
			{
				cbMaxData = cbFindData[i];
				cbMaxValue = cbTempValue;
			}
		}
	}

	return cbMaxData;
}

//��ֵ����
void CGameLogic::SortValue(BYTE cbData[], int nCount)
{
	if (nCount <= 0)
	{
		ASSERT(false);
		return ;
	}

	bool bIsSorted = false;
	BYTE cbComCount = nCount - 1;
	while (bIsSorted == false)
	{
		bIsSorted = true;
		for (int i = 0; i < cbComCount; ++i)
		{
			if (cbData[i] < cbData[i + 1])
			{
				BYTE cbTempData = cbData[i];
				cbData[i] = cbData[i + 1];
				cbData[i + 1] = cbTempData;
				bIsSorted = false;
			}
		}
		--cbComCount;
	}

	return ;
}

}
//////////////////////////////////////////////////////////////////////////
