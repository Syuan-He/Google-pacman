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
#include "object.h"
#include "character.h"
#include "score.h"
#include "../Library/audio.h"
#include <time.h>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_BEGIN,			// 0
		AUDIO_MOVE,				// 1
		AUDIO_DIE,				// 2
		AUDIO_EAT_FRUIT,		// 3
		AUDIO_EAT_GHOST,		// 4
		AUDIO_MUTIPLAYER,		// 5
		AUDIO_INTERMISSION,		// 6
		AUDIO_SIREN,			// 7
		AUDIO_POWERUP			// 8
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
		UIObject Background;
		GameMenu Menu_main{ 438, 370, 563, 384, 60 };
		UIObject Setting_background;
		GameMenu Menu_setting;
		UIObject Loading_icon;

		time_t loading_time_begin;
		int menu_now = 0;
		int button_kick = -1;

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
		GameMap Map;
		GamePacman Pacman;
		GameGhost Blinky;				// red one
		GameGhost Pinky;				// pink one
		GameGhost Inky;					// blue one
		GameGhost Clyde;				// orange onez
		GameBoss Boss;
		vector<GameGhost> ghosts = { Blinky, Pinky, Inky, Clyde };
		GameScore Score;
		UIObject P1_icon{25, 40};		//player1 圖標
		UIObject Ready_icon;			//Ready 圖標

		CAudio* Game_audio = CAudio::Instance();

		time_t exc_time_begin;			//遊戲起始時間

		void ghostMoveControl();
		time_t modePlayTime;
		int scatterTime = 7;
		int chaseTime = 20;
		int ghostCatchTime = 0;
		int preGhostCatchTime = 0;
		int modeCount;
		bool modeLock;
		bool isScatterTime();
		bool isChaseTime();
		bool isChoasTime();
		void ghostChase();
		void ghostScatter();
		void ghostTurnBack();
		void initialGhosts();

		time_t choasTime;
		time_t choasTimeChange;
		int choasTimeLong = 10;

		int phase = 0;							//階段
		int level = 0;							//遊戲關卡
		int end_level = 5;						//最終關
		vector<int> score_his;					//紀錄分數
		void show_obj_by_phase();				//顯示物件
		void pacman_get_catch(int mode = 0);	//pacman是否被鬼抓到
		void change_level(int level);			//切換關卡

		bool debug_mod = true;
		bool invincible = false;
		void debugText();
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
		int counter;	// 倒數之計數器
	};

}
