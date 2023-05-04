#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <time.h>
#include <memory>

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////
CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

time_t choasTimeChange;
void CGameStateRun::OnMove()							// 移動遊戲元素
{
	//階段2才能移動
	if (phase == 1) {
		Pacman.move();

		//模式改變前的方向改變
		if (modeLock && modeCount < 7 && (isScatterTime() || isChaseTime() || isChoasTime())) {
			ghostTurnBack();
			modeCount++;
			modeLock = false;
			if (modeCount == 4) {
				scatterTime = 5;
			}
		}
		else if (!modeLock && !(isScatterTime() || isChaseTime() || isChoasTime())) {
			modeLock = true;
		}
		else if (!isChoasTime()) {
			Blinky.isChoas = false;
			Pinky.isChoas = false;
			Inky.isChoas = false;
			Clyde.isChoas = false;
			Blinky.choasFlash = false;
			Pinky.choasFlash = false;
			Inky.choasFlash = false;
			Clyde.choasFlash = false;
			Blinky.setVelocity(2);
			Pinky.setVelocity(2);
			Inky.setVelocity(2);
			Clyde.setVelocity(2);
		}
		else if ((time(NULL) - choasTime) > choasTimeLong - 3) {
			Blinky.choasFlash = true;
			Pinky.choasFlash = true;
			Inky.choasFlash = true;
			Clyde.choasFlash = true;
		}
		else if (choasTimeChange != time(NULL)) {
			choasTimeChange = time(NULL);
			modePlayTime++;
		}

		if (modeCount >= 7 || isChaseTime()) {
			ghostChase();
		}
		else {
			ghostScatter();
		}

		if (Score.get_coin_nums() == 0) {
			phase = 4;
		}
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//地圖陣列初始化
	Map.map_loader("Resources/GameMap/GameMap_0");

	//加入參考地圖
	Pacman.set_game_map(Map);
	Blinky.set_game_map(Map);
	Pinky.set_game_map(Map);
	Inky.set_game_map(Map);
	Clyde.set_game_map(Map);

	//背景初始化
	Map.Background.SetTopLeft(Map.Background.window_shift[0], Map.Background.window_shift[1]);
	Map.Background.SetFrameIndexOfBitmap(0);

	//pacman初始化
	Pacman.LoadBitmapByString({
		"Resources/pacman/pacman0.bmp",
		"Resources/pacman/pacman1.bmp",
		"Resources/pacman/pacman2.bmp",
		"Resources/pacman/pacman3.bmp",
		"Resources/pacman/pacman4.bmp",
		"Resources/pacman/pacman5.bmp",
		"Resources/pacman/pacman6.bmp",
		"Resources/pacman/pacman7.bmp",
		"Resources/pacman/pacman8.bmp",
		"Resources/die/die0.bmp",
		"Resources/die/die1.bmp",
		"Resources/die/die2.bmp",
		"Resources/die/die3.bmp",
		"Resources/die/die4.bmp",
		"Resources/die/die5.bmp",
		"Resources/die/die6.bmp",
		"Resources/die/die7.bmp",
		"Resources/die/die8.bmp",
		"Resources/die/die9.bmp",
		"Resources/die/die10.bmp",
		"Resources/words/NULL.bmp",
	}, RGB(0, 0, 0));
	Pacman.set_inital(37, 15, 0);
	Pacman.initialize();
	//豆子,大力丸初始化
	for (int i = 0; i < Map.map_len[0]; i++) {
		for (int j = 0; j < Map.map_len[1]; j++) {
			//為0的道路加入豆子
			if (Map[i][j] == 0) {
				unique_ptr<CMovingBitmap> t(new CMovingBitmap);
				t -> LoadBitmapA("Resources/words/coin.bmp");
				t -> SetTopLeft(16 * (j - 2) + 6 + Map.Background.window_shift[0], 16 * i + 6 + Map.Background.window_shift[1]);
				Score.add_coin(*t);
				Score.set_coin_nums(1);
			}
			//為3的道路加入大力丸
			else if (Map[i][j] == 3) {
				CMovingBitmap t;
				t.LoadBitmapA("Resources/words/dot.bmp");
				t.SetTopLeft(16 * (j - 2) + 4 + Map.Background.window_shift[0], 16 * i + 4 + Map.Background.window_shift[1]);
				Score.add_power_pellets(t);
			}
		}
	}

	//Blinky 初始化
	Blinky.LoadBitmapByString({
		"Resources/red/red0.bmp",
		"Resources/red/red1.bmp",
		"Resources/red/red2.bmp",
		"Resources/red/red3.bmp",
		"Resources/red/red4.bmp",
		"Resources/red/red5.bmp",
		"Resources/red/red6.bmp",
		"Resources/red/red7.bmp",
		"Resources/choas/choas0.bmp",
		"Resources/choas/choas1.bmp",
		"Resources/choas/choas2.bmp",
		"Resources/choas/choas3.bmp"
		}, RGB(0, 0, 0));
	Blinky.set_inital(37, 4, 0);
	Blinky.initialize();

	//Pinky 初始化
	Pinky.LoadBitmapByString({
		"Resources/pink/pink6.bmp",
		"Resources/pink/pink7.bmp",
		"Resources/pink/pink0.bmp",
		"Resources/pink/pink1.bmp",
		"Resources/pink/pink4.bmp",
		"Resources/pink/pink5.bmp",
		"Resources/pink/pink2.bmp",
		"Resources/pink/pink3.bmp",
		"Resources/choas/choas0.bmp",
		"Resources/choas/choas1.bmp",
		"Resources/choas/choas2.bmp",
		"Resources/choas/choas3.bmp"
		}, RGB(0, 0, 0));
	Pinky.set_inital(37, 4, 0);
	Pinky.initialize();

	//Inky 初始化
	Inky.LoadBitmapByString({
		"Resources/blue/blue6.bmp",
		"Resources/blue/blue7.bmp",
		"Resources/blue/blue0.bmp",
		"Resources/blue/blue1.bmp",
		"Resources/blue/blue4.bmp",
		"Resources/blue/blue5.bmp",
		"Resources/blue/blue2.bmp",
		"Resources/blue/blue3.bmp",
		"Resources/choas/choas0.bmp",
		"Resources/choas/choas1.bmp",
		"Resources/choas/choas2.bmp",
		"Resources/choas/choas3.bmp"
		}, RGB(0, 0, 0));
	Inky.set_inital(37, 4, 0);
	Inky.initialize();

	//Clyde 初始化
	Clyde.LoadBitmapByString({
		"Resources/orange/orange6.bmp",
		"Resources/orange/orange7.bmp",
		"Resources/orange/orange0.bmp",
		"Resources/orange/orange1.bmp",
		"Resources/orange/orange4.bmp",
		"Resources/orange/orange5.bmp",
		"Resources/orange/orange2.bmp",
		"Resources/orange/orange3.bmp",
		"Resources/choas/choas0.bmp",
		"Resources/choas/choas1.bmp",
		"Resources/choas/choas2.bmp",
		"Resources/choas/choas3.bmp"
		}, RGB(0, 0, 0));
	Clyde.set_inital(37, 4, 0);
	Clyde.initialize();

	//P1初始化
	P1_icon.LoadBitmapA("Resources/words/P1.bmp");
	P1_icon.SetTopLeft(P1_icon.window_shift[0], P1_icon.window_shift[1]);

	//Ready圖標初始化
	Ready_icon.LoadBitmapByString({
		"Resources/words/ready.bmp",
		"Resources/words/game_over.bmp",
		}, RGB(0, 0, 0));
	Ready_icon.SetFrameIndexOfBitmap(0);
	Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);

	//分數條初始化
	for (int i = 0; i < Score.game_scores.get_nums(); i++) {
		CMovingBitmap t;
		t.LoadBitmapByString({
			"Resources/words/0.bmp",
			"Resources/words/1.bmp",
			"Resources/words/2.bmp",
			"Resources/words/3.bmp",
			"Resources/words/4.bmp",
			"Resources/words/5.bmp",
			"Resources/words/6.bmp",
			"Resources/words/7.bmp",
			"Resources/words/8.bmp",
			"Resources/words/9.bmp",
			});
		t.SetFrameIndexOfBitmap(0);
		t.SetTopLeft(i * 16 + Score.game_scores.window_shift[0], Score.game_scores.window_shift[1]);
		Score.game_scores.add_obj(t);
	}

	//血條初始化
	for (int i = 0; i < Pacman.hearts_icon.get_nums(); i++) {
		CMovingBitmap t;
		t.LoadBitmapA("Resources/pacman/pacman5.bmp");
		t.SetTopLeft(i * 32 + Pacman.hearts_icon.window_shift[0], Pacman.hearts_icon.window_shift[1]);
		Pacman.hearts_icon.add_obj(t);
	}

	//遊戲開始時間
	exc_time_begin = time(NULL);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//鍵盤方向鍵取得
	switch (nChar) {
	case VK_RIGHT:
		Pacman.set_dir_waitfor(0);
		break;
	case VK_LEFT:
		Pacman.set_dir_waitfor(2);
		break;
	case VK_UP:
		Pacman.set_dir_waitfor(1);
		break;
	case VK_DOWN:
		Pacman.set_dir_waitfor(3);
		break;
	default:
		break;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	//顯示物件
	show_obj_by_phase();
	//偵測是否吃到豆子
	Score.get_point(Pacman);
	//偵測是否吃到大力丸、鬼進入混亂模式
	if (Score.get_power(Pacman)) {
		Blinky.isChoas = true;
		Pinky.isChoas = true;
		Inky.isChoas = true;
		Clyde.isChoas = true;
		Blinky.choasFlash = false;
		Pinky.choasFlash = false;
		Inky.choasFlash = false;
		Clyde.choasFlash = false;
		Blinky.setVelocity(1);
		Pinky.setVelocity(1);
		Inky.setVelocity(1);
		Clyde.setVelocity(1);
		ghostTurnBack();
		choasTime = time(NULL);
		choasTimeChange = choasTime;
	}
	pacman_get_catch();

	//debug
	debugText();
}