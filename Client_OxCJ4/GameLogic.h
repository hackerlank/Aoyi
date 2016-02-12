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
//宏定义
#define ALL_COUNT					54									//全部扑克

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
enum emCardType
{
	CT_NIU0 = 0,			//无牛
	CT_NIU1,				//牛1
	CT_NIU2,				//牛2
	CT_NIU3,				//牛3
	CT_NIU4,				//牛4
	CT_NIU5,				//牛5
	CT_NIU6,				//牛6
	CT_NIU7,				//牛7
	CT_NIU8,				//牛8
	CT_NIU9,				//牛9
	CT_NIUNIU = 10,			//牛牛
	CT_NIUWH_TWODOUBLE,		//两对五花牛
	CT_NIUWH_THREE,			//三条五花牛
	CT_NIUWH_HULU,			//葫芦五花牛
	CT_NIUWH_TWOGUI,		//两鬼五花牛
	CT_NIUWH_FOUR,			//四炸五花牛
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[ALL_COUNT];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//判断类型
	BYTE CheckCardType(const BYTE cbCheckData[], BYTE cbCheckCount);
	//取得最大可用类型
	BYTE GetMaxCardType(BYTE cbCardData[], BYTE cbCardCount);
	BYTE GetCardIndex(const BYTE cbData);

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//功能函数
public:
	//取得请求数目的扑克
	void GetDataWithGiveCard(const BYTE cbSrcData[], const BYTE cbSrcCount, BYTE cbDestData[], BYTE cbDestCount);
	//替换五花牛为其它牛牛
	bool ChangeWuhuaToNiuniu(const BYTE cbCanUseData[], const BYTE cbCanUseCount, BYTE cbChangeData[], BYTE cbChangeCount);
	//对比扑克
	bool CompareCard(const BYTE cbFirstData[], const BYTE cbNextData[], BYTE cbFirstCount, BYTE cbNextCount);
	//移除扑克
	BYTE RemoveCard(const BYTE cbRemoveData[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
	//判断是否可以取得相应五花牛牌型
	bool CanGetDestWuhuaType(const BYTE cbCheckData[], const BYTE cbCheckCount, const BYTE cbCheckType);
	//数值排序
	void SortValue(BYTE cbData[], int nCount);
	//是否为牛
	bool IsOxType(const BYTE cbCardData[], BYTE cbCardCount);

	//辅助函数
private:
	//查找牌值最大扑克
	BYTE FindMaxValueData(const BYTE cbFindData[], BYTE cbFindCount);
};
//////////////////////////////////////////////////////////////////////////

}
#endif	//_OX_CJ4_GAME_LOGIC_HEAD_FILE_
