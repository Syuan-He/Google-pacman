/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "character.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo
		void draw_text();
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		//地圖相關
		int window_shift[2] = { 25, 100 };				//地圖圖像原點位移
		//int origin_position_shift[2] = { 35, 110};	//地圖可移動範圍原點位移
		int map_len[2] = {17, 62};						//地圖大小
		//0為路, 1為牆, 2無分數的路, 3大力丸 
		int** gameMap;									//地圖(需載入)
		string map_dir = "Resources/google_map.txt";	//地圖載入地址
		void map_loader(string str);					//地圖載入
		
		//整體
		//int phase = 1;								//之後作為難度改變依據
		CMovingBitmap background;						//背景物件
		CMovingBitmap P1_icon;							//player1 圖標
		vector<CMovingBitmap> hearts_icon;				//生命圖標
		int heart_num = 3;								//生命數
		int portal_position[2][2] = { { 1, 8 }, { 60, 8 } }; //傳送門位置和傳送位置
		void show_image_by_phase();						//顯示物件
		void update_position(int dir, int* pos);		//更新指定物件座標
		void debugText();
		bool portal_detect(CMovingBitmap* obj, int* pos, int portal_pos[2][2]); //偵測是否進入重送門並移動指定物件
		bool objCanMove(int dir, int x, int y);			//是否可以向指定方向移動

		//分數相關
		vector<CMovingBitmap*> coins;					//所有豆子
		map<pair<int, int>, int> coins_map;
		vector<CMovingBitmap> power_pellets;			//所有power_pellets
		vector<CMovingBitmap> game_scores;				//分數條
		int score_digits = 5;							//分數條的位數
		int total_coin_nums = 0;						//所有豆子個數
		int score = 0;									//總分
		void get_point(CMovingBitmap* obj, int x, int y); //偵測是否吃到豆子
		void get_power(CMovingBitmap* obj);				//偵測是否吃到大力丸
		void update_score(int sc);						//更新分數條

		//pacman
		CMovingBitmap pacman;							//pacman物件
		int pacman_dir_now = 2;							//pacman的移動方向
		int pacman_dir_waitfor = 2;						//pacman期望的移動方向
		int pacman_position[2] = { 37, 15 };			//pacman的初始座標
		int pacman_total_step = 0;						//pacman的移動總步數(最大16(2倍原始地圖下的一步長))
		int pacman_velocity = 2;						//pacman的移動速度
		void move_pacman();								//移動pacman

		//ghost
		bool isVaildNode(int x, int y, int xx, int yy);
		int astar(int x0, int y0, int x1, int y1);
		//character red;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;									// 倒數之計數器
	};

}
