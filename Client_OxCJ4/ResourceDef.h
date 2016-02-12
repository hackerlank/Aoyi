#ifndef _OXCJ4_RESOURCE_DEF_H_
#define _OXCJ4_RESOURCE_DEF_H_

namespace GameOxCJ4
{
enum enSpriteResDef
{
	//背景
	Ox_Bk_Begin = 11,
	Ox_Bk_End = 11,

	//扑克
	Ox_Card_0 = 400,
	Ox_CARD_11 = 411,
	Ox_CARD_12 = 412,
	Ox_CARD_13 = 413,
	Ox_CARD_14 = 414,
	Ox_CARD_15 = 415,
	Ox_CARD_16 = 416,
	Ox_CARD_17 = 417,
	Ox_CARD_18 = 418,
	Ox_CARD_19 = 419,
	Ox_CARD_1a = 510,
	Ox_CARD_1b = 511,
	Ox_CARD_1c = 512,
	Ox_CARD_1d = 513,

	Ox_CARD_21 = 421,
	Ox_CARD_22 = 422,
	Ox_CARD_23 = 423,
	Ox_CARD_24 = 424,
	Ox_CARD_25 = 425,
	Ox_CARD_26 = 426,
	Ox_CARD_27 = 427,
	Ox_CARD_28 = 428,
	Ox_CARD_29 = 429,
	Ox_CARD_2a = 520,
	Ox_CARD_2b = 521,
	Ox_CARD_2c = 522,
	Ox_CARD_2d = 523,

	Ox_CARD_31 = 431,
	Ox_CARD_32 = 432,
	Ox_CARD_33 = 433,
	Ox_CARD_34 = 434,
	Ox_CARD_35 = 435,
	Ox_CARD_36 = 436,
	Ox_CARD_37 = 437,
	Ox_CARD_38 = 438,
	Ox_CARD_39 = 439,
	Ox_CARD_3a = 530,
	Ox_CARD_3b = 531,
	Ox_CARD_3c = 532,
	Ox_CARD_3d = 533,

	Ox_CARD_41 = 441,
	Ox_CARD_42 = 442,
	Ox_CARD_43 = 443,
	Ox_CARD_44 = 444,
	Ox_CARD_45 = 445,
	Ox_CARD_46 = 446,
	Ox_CARD_47 = 447,
	Ox_CARD_48 = 448,
	Ox_CARD_49 = 449,
	Ox_CARD_4a = 540,
	Ox_CARD_4b = 541,
	Ox_CARD_4c = 542,
	Ox_CARD_4d = 543,

	Ox_CARD_51 = 451,
	Ox_CARD_52 = 452,

	//游戏界面
	Ox_Bg_Banker_Flag = 1001,
	Ox_Bg_Calling_Banker = 1002,
	Ox_Bg_Jetton = 1003,
	Ox_Bg_Nao_Zhong = 1004,
	Ox_Bg_Win_Flag = 1005,
	Ox_Bg_Lose_Flag = 1006,
	Ox_Bg_Seatbk_h = 1007,
	Ox_Bg_Seatbk_v = 1008,
	Ox_Bg_User_Ready = 1009,
	Ox_Bg_Over_Show = 1010,
	Ox_Bg_Xia_Zhu = 1011,
	Ox_Bg_Result_Ox0 = 1012,
	Ox_Bg_Result_Ox1 = 1013,
	Ox_Bg_Result_Ox2 = 1014,
	Ox_Bg_Result_Ox3 = 1015,
	Ox_Bg_Result_Ox4 = 1016,
	Ox_Bg_Result_Ox5 = 1017,
	Ox_Bg_Result_Ox6 = 1018,
	Ox_Bg_Result_Ox7 = 1019,
	Ox_Bg_Result_Ox8 = 1020,
	Ox_Bg_Result_Ox9 = 1021,
	Ox_Bg_Result_Ox10 = 1022,
	Ox_Bg_Confirm = 1023,
	Ox_Bg_Cai_Jinchi = 1024,
	Ox_Bg_Ji_Lu = 1025,
	Ox_Bg_Xiang_Qing = 1026,
	Ox_Bg_Qian_Dai = 1027,
	Ox_Bg_Null_Bk = 1028,

	//按钮
	Ox_Bt_Zhun_Bei_N = 1201,
	Ox_Bt_Zhun_Bei_C = 1202,
	Ox_Bt_Call_Banker_N = 1203,
	Ox_Bt_Call_Banker_C = 1204,
	Ox_Bt_Bu_Jiao_N = 1205,
	Ox_Bt_Bu_Jiao_C = 1206,
	Ox_Bt_Jetton_Bk_N = 1207,
	Ox_Bt_Jetton_Bk_C = 1208,
	Ox_Bt_Jetton_Bk_D = 1209,
	Ox_Bt_Pai_Xu_N = 1210,
	Ox_Bt_Pai_Xu_C = 1211,
	Ox_Bt_Ti_Shi_N = 1212,
	Ox_Bt_Ti_Shi_C = 1213,
	Ox_Bt_Show_Card_N = 1214,
	Ox_Bt_Show_Card_C = 1215,
	Ox_Bt_Show_Card_D = 1216,
	Ox_Bt_Tuo_Guan_N = 1217,
	Ox_Bt_Tuo_Guan_C = 1218,
	Ox_Bt_Qu_Xiao_TG_N = 1219,
	Ox_Bt_Qu_Xiao_TG_C = 1220,
	Ox_Bt_Out_N = 1221,
	Ox_Bt_Out_C = 1222,
	Ox_Bt_Option_N = 1223,
	Ox_Bt_Option_C = 1224,
	Ox_Bt_Quit_N = 1225,
	Ox_Bt_Quit_C = 1226,
	Ox_Bt_Cancel_N = 1227,
	Ox_Bt_Cancel_C = 1228,

	//房间资源
	Ox_Room_Titlebk = 1901,
	Ox_Room_Table = 1902,
	Ox_Room_Sit = 1903,
	Ox_Room_Sit_D = 1904,
	Ox_Room_Sit_Name_Bk = 1905,
	Ox_Path_Room_Ready = 1906,
	Ox_Room_Table_P = 1907,
	Ox_Room_Table_Num_Font = 1908,

	//atlas文字
	Ox_Font_Ui = 201,
	Ox_Font_Clock = 202,
	Ox_Font_Jetton_Value = 204,
	Ox_Font_User_Score = 206,
	Ox_Font_Score_Num_Add = 207,
	Ox_Font_Score_Num_Dec = 208,
	Ox_Font_Cai_Jin_Chi = 209,
	Ox_Font_Week_Score = 210,

	//font文字
	Ox_Font_Room_Name = 302,
	Ox_Font_Room_Sit_Name = 303,
};

enum enSoundResDef
{
	Ox_Sound_BackGround = 2000,

	//男声
	Ox_Sound_Man_Niu0 = 2010,
	Ox_Sound_Man_Niu1 = 2011,
	Ox_Sound_Man_Niu2 = 2012,
	Ox_Sound_Man_Niu3 = 2013,
	Ox_Sound_Man_Niu4 = 2014,
	Ox_Sound_Man_Niu5 = 2015,
	Ox_Sound_Man_Niu6 = 2016,
	Ox_Sound_Man_Niu7 = 2017,
	Ox_Sound_Man_Niu8 = 2018,
	Ox_Sound_Man_Niu9 = 2019,
	Ox_Sound_Man_NiuNiu = 2020,

	//女声
	Ox_Sound_Wom_Niu0 = 2050,
	Ox_Sound_Wom_Niu1 = 2051,
	Ox_Sound_Wom_Niu2 = 2052,
	Ox_Sound_Wom_Niu3 = 2053,
	Ox_Sound_Wom_Niu4 = 2054,
	Ox_Sound_Wom_Niu5 = 2055,
	Ox_Sound_Wom_Niu6 = 2056,
	Ox_Sound_Wom_Niu7 = 2057,
	Ox_Sound_Wom_Niu8 = 2058,
	Ox_Sound_Wom_Niu9 = 2059,
	Ox_Sound_Wom_NiuNiu = 2060,

	//音效
	Ox_Sound_Add_Score = 2090,
	Ox_Sound_Send_Card = 2091,
	Ox_Sound_Open_Card = 2092,
	Ox_Sound_Game_Win = 2093,
	Ox_Sound_Game_Lost = 2094,
	Ox_Sound_Game_Warn = 2095,
};

const int g_vCardResList[] = {
	Ox_CARD_11, Ox_CARD_12, Ox_CARD_13, Ox_CARD_14, Ox_CARD_15, Ox_CARD_16, Ox_CARD_17, Ox_CARD_18, Ox_CARD_19, Ox_CARD_1a, Ox_CARD_1b, Ox_CARD_1c, Ox_CARD_1d,
	Ox_CARD_21, Ox_CARD_22, Ox_CARD_23, Ox_CARD_24, Ox_CARD_25, Ox_CARD_26, Ox_CARD_27, Ox_CARD_28, Ox_CARD_29, Ox_CARD_2a, Ox_CARD_2b, Ox_CARD_2c, Ox_CARD_2d,
	Ox_CARD_31, Ox_CARD_32, Ox_CARD_33, Ox_CARD_34, Ox_CARD_35, Ox_CARD_36, Ox_CARD_37, Ox_CARD_38, Ox_CARD_39, Ox_CARD_3a, Ox_CARD_3b, Ox_CARD_3c, Ox_CARD_3d,
	Ox_CARD_41, Ox_CARD_42, Ox_CARD_43, Ox_CARD_44, Ox_CARD_45, Ox_CARD_46, Ox_CARD_47, Ox_CARD_48, Ox_CARD_49, Ox_CARD_4a, Ox_CARD_4b, Ox_CARD_4c, Ox_CARD_4d,
	Ox_CARD_51, Ox_CARD_52,
};

}

#endif	//__OXCJ4_RESOURCE_DEF_H__
