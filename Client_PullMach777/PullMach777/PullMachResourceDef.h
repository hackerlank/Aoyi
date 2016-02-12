#ifndef _OXCJ4_RESOURCE_DEF_H_
#define _OXCJ4_RESOURCE_DEF_H_

#include "CMD_Game.h"

namespace GamePullMach777
{

enum enSpriteResDef
{
	//背景
	PullMach_Bg_Interface = 11,

	//图标
	PullMach_CARD_Coin = 400,
	PullMach_CARD_Leech = 401,
	PullMach_CARD_Tsama = 402,
	PullMach_CARD_Bell = 403,
	PullMach_CARD_Blue7 = 404,
	PullMach_CARD_Red7 = 405,
	PullMach_CARD_Logo = 406,

	//按钮
	PullMach_Bt_Start_N = 1001,
	PullMach_Bt_Start_C = 1002,
	PullMach_Bt_Start_D = 1003,
	PullMach_Bt_Quit_N = 1004,
	PullMach_Bt_Help_N = 1005,
	PullMach_Bt_Bank_Storage_N = 1006,
	PullMach_Bt_Zi_Dong_N = 1007,
	PullMach_Bt_Zi_Dong_C = 1008,
	PullMach_Bt_Zi_Dong_D = 1009,
	PullMach_Bt_Qu_Xiao_ZD_N = 1010,
	PullMach_Bt_Qu_Xiao_ZD_C = 1011,
	PullMach_Bt_Qu_Xiao_ZD_D = 1012,
	PullMach_Bt_Shang_Fen_N = 1013,
	PullMach_Bt_Shang_Fen_C = 1014,
	PullMach_Bt_Shang_Fen_D = 1015,
	PullMach_Bt_Ya_Fen_N = 1016,
	PullMach_Bt_Ya_Fen_C = 1017,
	PullMach_Bt_Ya_Fen_D = 1018,
	PullMach_Bt_Stop_Left_N = 1019,
	PullMach_Bt_Stop_Left_C = 1020,
	PullMach_Bt_Stop_Left_D = 1021,
	PullMach_Bt_Stop_Mid_N = 1022,
	PullMach_Bt_Stop_Mid_C = 1023,
	PullMach_Bt_Stop_Mid_D = 1024,
	PullMach_Bt_Stop_Right_N = 1025,
	PullMach_Bt_Stop_Right_C = 1026,
	PullMach_Bt_Stop_Right_D = 1027,
	PullMach_Bt_Help_Close_N = 1028,
	PullMach_Bt_Help_Close_C = 1029,

	//游戏界面
	PullMach_Bg_Box = 1101,
	PullMach_Bg_Credit_Frame = 1102,
	PullMach_Bg_DangQianFenShu = 1103,
	PullMach_Bg_Roll_Mach = 1104,
	PullMach_Bg_Roll_Mach_Bg = 1105,
	PullMach_Bg_Roll_Mach_Light = 1106,
	PullMach_Bg_Scroll = 1107,
	PullMach_Bg_ShengYuJiangLi = 1108,
	PullMach_Bg_Silver_Coin = 1109,
	PullMach_Bg_User_Info_Bg = 1110,
	PullMach_Bg_User_Score_Frame = 1111,
	PullMach_Bg_ZhongShuYing = 1112,
	PullMach_Bg_Specil_Num0 = 1113,
	PullMach_Bg_Specil_Num1 = 1114,
	PullMach_Bg_Specil_Num2 = 1115,
	PullMach_Bg_Specil_Num3 = 1116,
	PullMach_Bg_Specil_Num4 = 1117,
	PullMach_Bg_Specil_Num5 = 1118,
	PullMach_Bg_Specil_Num6 = 1119,
	PullMach_Bg_Specil_Num7 = 1120,
	PullMach_Bg_Specil_Num8 = 1121,
	PullMach_Bg_Specil_Num9 = 1122,
	PullMach_Bg_Prize_Time_Tip = 1123,
	PullMach_Bg_Help_Bar_Up = 1124,
	PullMach_Bg_Help_Bar_Down = 1125,
	PullMach_Bg_Help_Bg = 1126,

	//界面大图
	PullMach_Bg_Normal = 1301,
	PullMach_Bg_Big_Prize = 1302,
	PullMach_Bg_Give_Prize = 1303,
	PullMach_Bg_Change = 1304,
	PullMach_Bg_Flash_Frame1 = 1305,
	PullMach_Bg_Flash_Frame2 = 1306,
	PullMach_Bg_Split = 1307,
	PullMach_Bg_Help_Info = 1308,

	//动画起始图
	PullMach_AniBegin_Box_Light1 = 1401,
	PullMach_AniBegin_Box_Open1 = 1402,
	PullMach_AniBegin_Red7_Swing1 = 1403,
	PullMach_AniBegin_Girl_Fly = 1404,

	//atlas文字
	PullMach_Font_Credit_Score = 201,
	PullMach_Font_Win_Score = 202,
	PullMach_Font_User_Score = 203,
	PullMach_Font_Prize_Times = 204,
	PullMach_Font_Logo_Num = 205,

	//font文字
	PullMach_Font_Room_Name = 302,
	PullMach_Font_Room_Sit_Name = 303,
};

enum enSoundResDef
{
	//背景音乐
	//PullMach_Sound_Back_Music = 2000,

	//音效
	PullMach_Sound_Bet_Coin = 2001,
	PullMach_Sound_Change_Scene = 2002,
	PullMach_Sound_Credit_Score = 2003,
	PullMach_Sound_Get_Prize = 2004,
	PullMach_Sound_Roll = 2005,
	PullMach_Sound_Stop = 2006,
	PullMach_Sound_Win_Tip = 2007,
};

//物体图标列表
const int g_vObjCardResList[OBJECT_TYPE] = {
	PullMach_CARD_Coin, PullMach_CARD_Leech, PullMach_CARD_Tsama,
	PullMach_CARD_Bell, PullMach_CARD_Blue7, PullMach_CARD_Red7,
	PullMach_CARD_Logo,
};

}

#endif	//__OXCJ4_RESOURCE_DEF_H__
