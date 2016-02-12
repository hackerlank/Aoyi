#ifndef _OXCJ4_RESOURCE_DEF_H_
#define _OXCJ4_RESOURCE_DEF_H_

#include "CMD_Game.h"

namespace GameStar97
{

enum enSpriteResDef
{
	//背景
	Star97_Bg_Phone = 11,
	Star97_Bg_Pc = 12,

	//扑克
	Star97_Card_Back_Null = 400,
	Star97_CARD_Leech = 411,
	Star97_CARD_Orange = 412,
	Star97_CARD_Pawpaw = 413,
	Star97_CARD_Bell = 414,
	Star97_CARD_Tsama = 415,
	Star97_CARD_BarY = 416,
	Star97_CARD_BarR= 417,
	Star97_CARD_BarB = 418,
	Star97_CARD_Num7 = 419,

	Star97_CARD_Star20 = 421,
	Star97_CARD_Star21 = 422,
	Star97_CARD_Star22 = 423,
	Star97_CARD_Star23 = 424,
	Star97_CARD_Star24 = 425,
	Star97_CARD_Star25 = 426,
	Star97_CARD_Star26 = 427,
	Star97_CARD_Star27 = 428,
	Star97_CARD_Star28 = 429,

	Star97_CARD_Star30 = 431,
	Star97_CARD_Star31 = 432,
	Star97_CARD_Star32 = 433,
	Star97_CARD_Star33 = 434,
	Star97_CARD_Star34 = 435,
	Star97_CARD_Star35 = 436,
	Star97_CARD_Star36 = 437,
	Star97_CARD_Star37 = 438,
	Star97_CARD_Star38 = 439,

	Star97_CARD_Star40 = 441,
	Star97_CARD_Star41 = 442,
	Star97_CARD_Star42 = 443,
	Star97_CARD_Star43 = 444,
	Star97_CARD_Star44 = 445,
	Star97_CARD_Star45 = 446,
	Star97_CARD_Star46 = 447,
	Star97_CARD_Star47 = 448,
	Star97_CARD_Star48 = 449,

	Star97_CARD_Rate2 = 451,
	Star97_CARD_Rate3 = 452,
	Star97_CARD_Rate4 = 453,

	//按钮
	Star97_Bt_PC_Da_Jiang_N = 1001,
	Star97_Bt_PC_Da_Jiang_C = 1002,
	Star97_Bt_PC_Da_Jiang_D = 1003,
	Star97_Bt_PC_Zi_Dong_N = 1004,
	Star97_Bt_PC_Zi_Dong_C = 1005,
	Star97_Bt_PC_Zi_Dong_D = 1006,
	Star97_Bt_PC_Shang_Fen_N = 1007,
	Star97_Bt_PC_Shang_Fen_C = 1008,
	Star97_Bt_PC_Shang_Fen_D = 1009,
	Star97_Bt_PC_Ya_Fen_N = 1010,
	Star97_Bt_PC_Ya_Fen_C = 1011,
	Star97_Bt_PC_Ya_Fen_D = 1012,
	Star97_Bt_PC_Qu_Xiao_ZD_N = 1013,
	Star97_Bt_PC_Qu_Xiao_ZD_C = 1014,
	Star97_Bt_PC_Qu_Xiao_ZD_D = 1015,
	Star97_Bt_PC_Start_N = 1016,
	Star97_Bt_PC_Start_C = 1017,
	Star97_Bt_PC_Start_D = 1018,

	Star97_Bt_PH_Da_Jiang_N = 1041,
	Star97_Bt_PH_Da_Jiang_C = 1042,
	Star97_Bt_PH_Da_Jiang_D = 1043,
	Star97_Bt_PH_Zi_Dong_N = 1044,
	Star97_Bt_PH_Zi_Dong_C = 1045,
	Star97_Bt_PH_Zi_Dong_D = 1046,
	Star97_Bt_PH_Shang_Fen_N = 1047,
	Star97_Bt_PH_Shang_Fen_C = 1048,
	Star97_Bt_PH_Shang_Fen_D = 1049,
	Star97_Bt_PH_Ya_Fen_N = 1050,
	Star97_Bt_PH_Ya_Fen_C = 1051,
	Star97_Bt_PH_Ya_Fen_D = 1052,
	Star97_Bt_PH_Qu_Xiao_ZD_N = 1053,
	Star97_Bt_PH_Qu_Xiao_ZD_C = 1054,
	Star97_Bt_PH_Qu_Xiao_ZD_D = 1055,
	Star97_Bt_PH_Start_N = 1056,
	Star97_Bt_PH_Start_C = 1057,
	Star97_Bt_PH_Start_D = 1058,
	Star97_Bt_PH_Quit_N = 1059,
	Star97_Bt_PH_Quit_C = 1060,
	Star97_Bt_PH_Quit_D = 1061,
	Star97_Bt_PH_Setting = 1062,

	//游戏界面
	Star97_Bg_Auto_Tips = 1101,
	Star97_Bg_Bonus_Tag = 1102,
	Star97_Bg_Half_Tag = 1103,
	Star97_Bg_Extra_Reward = 1104,
	Star97_Bg_Line_Hori = 1105,
	Star97_Bg_Line_Verti = 1106,
	Star97_Bg_Line_D = 1107,
	Star97_Bg_Line_E = 1108,
	Star97_Bg_Flash_Frame = 1109,
	Star97_Bg_Game_Over = 1110,
	Star97_Bg_You_Win = 1111,
	Star97_Bg_Push_Bet = 1112,
	Star97_Bg_Push_Start = 1113,
	Star97_Bg_Insert_Coin = 1114,
	Star97_Bg_BigPrize_Bg = 1115,
	Star97_Bg_Shortcut_Key = 1116,

	//设置界面
	Star97_Set_Setting_Bg = 1201,
	Star97_Set_Close = 1202,
	Star97_Set_Dian = 1203,
	Star97_Set_Tiao = 1204,
	Star97_Set_Tiao_Bg = 1205,
	Star97_Set_TiaoS = 1206,
	Star97_Set_TiaoS_Bg = 1207,
	Star97_Set_Title = 1208,

	//loading界面
	Star97_Load_Loading_Bg = 1301,
	Star97_Load_Ming = 1302,
	Star97_Load_Xing = 1303,
	Star97_Load_9 = 1304,
	Star97_Load_7 = 1305,

	//房间资源
	Star97_Room_Titlebk = 1901,
	Star97_Room_Table = 1902,
	Star97_Room_Sit = 1903,
	Star97_Room_Sit_D = 1904,
	Star97_Room_Sit_Name_Bk = 1905,
	Star97_Path_Room_Ready = 1906,
	Star97_Room_Table_P = 1907,
	Star97_Room_Table_Num_Font = 1908,

	//atlas文字
	Star97_Font_User_Score = 201,
	Star97_Font_Common = 202,
	Star97_Font_Extra = 203,
	Star97_Font_Handsel = 204,
	Star97_Font_Push_Score = 205,
	Star97_Font_Record = 206,
	Star97_Font_Table_Winlose = 207,
	Star97_Font_Extra_Info = 208,

	//font文字
	Star97_Font_Room_Name = 302,
	Star97_Font_Room_Sit_Name = 303,
};

enum enSoundResDef
{
	//背景音乐
	Star97_Sound_Back_Music = 2000,

	//音效
	Star97_Sound_Bet_Score = 2001,
	Star97_Sound_Fruit_Roll = 2002,
	Star97_Sound_Player_GetScore = 2003,
	Star97_Sound_Player_Lose = 2004,
	Star97_Sound_Player_Win = 2005,
	Star97_Sound_Win_Big_Prize = 2006,
};

//物体图标列表
const int g_vObjCardResList[9] = {
	Star97_CARD_Leech, Star97_CARD_Orange, Star97_CARD_Pawpaw,
	Star97_CARD_Bell, Star97_CARD_Tsama, Star97_CARD_BarY,
	Star97_CARD_BarR, Star97_CARD_BarB, Star97_CARD_Num7,
};

//明星图标列表
const int g_vStarResList[3][CELL_NUM] = {
	{
		Star97_CARD_Star20,Star97_CARD_Star21,Star97_CARD_Star22,
		Star97_CARD_Star23,Star97_CARD_Star24,Star97_CARD_Star25,
		Star97_CARD_Star26,Star97_CARD_Star27,Star97_CARD_Star28,
	},
	{
		Star97_CARD_Star30,Star97_CARD_Star31,Star97_CARD_Star32,
		Star97_CARD_Star33,Star97_CARD_Star34,Star97_CARD_Star35,
		Star97_CARD_Star36,Star97_CARD_Star37,Star97_CARD_Star38,
	},
	{
		Star97_CARD_Star40,Star97_CARD_Star41,Star97_CARD_Star42,
		Star97_CARD_Star43,Star97_CARD_Star44,Star97_CARD_Star45,
		Star97_CARD_Star46,Star97_CARD_Star47,Star97_CARD_Star48,
	},
};

//明星倍率列表
const int g_vRateResList[3] = {Star97_CARD_Rate2, Star97_CARD_Rate3, Star97_CARD_Rate4,};

//中奖线资源
const int g_vLineResList[LINE_NUM] = {
	Star97_Bg_Line_Hori, Star97_Bg_Line_Hori, Star97_Bg_Line_Hori,
	Star97_Bg_Line_D, Star97_Bg_Line_E,
	Star97_Bg_Line_Verti, Star97_Bg_Line_Verti, Star97_Bg_Line_Verti,
};

//图标顺序
#define Sort_Icon_Count		28		//顺序图标个数
const int g_vSortIconResList[Sort_Icon_Count] = {
	Star97_CARD_Tsama, Star97_CARD_BarY, Star97_CARD_Bell, Star97_CARD_Orange,
	Star97_CARD_Tsama, Star97_CARD_Pawpaw, Star97_CARD_Orange, Star97_CARD_Bell, 
	Star97_CARD_BarR, Star97_CARD_Pawpaw, Star97_CARD_Tsama, Star97_CARD_Leech,
	Star97_CARD_Orange, Star97_CARD_BarY, Star97_CARD_Pawpaw, Star97_CARD_Orange, 
	Star97_CARD_Bell, Star97_CARD_BarR, Star97_CARD_Tsama, Star97_CARD_Orange, 
	Star97_CARD_Pawpaw, Star97_CARD_BarY, Star97_CARD_Bell, Star97_CARD_Leech, 
	Star97_CARD_BarB, Star97_CARD_Orange, Star97_CARD_Pawpaw, Star97_CARD_Num7, 
};

}

#endif	//__OXCJ4_RESOURCE_DEF_H__
