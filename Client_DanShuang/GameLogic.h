#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////

//���������νṹ
struct tagTriangle
{
	CCPoint point[3];
};

//��Ϸ�߼�
class CGameLogic
{
public:
	//ȡ����ע��������
	WORD GetAreaLossPercent(BYTE cbArea);
	//�жϵ��Ƿ����������ڲ�
	bool IsPointInTriangle(const CCPoint & desPoint, const tagTriangle & desTriangle);

};
//////////////////////////////////////////////////////////////////////////

#endif
