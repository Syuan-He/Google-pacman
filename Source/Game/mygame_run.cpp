#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <time.h>

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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	//階段2才能移動
	if (phase == 1) {
		Pacman.move();
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//地圖陣列初始化
	Map.map_loader("Resources/google_map.txt");

	//加入參考地圖
	Pacman.set_game_map(Map);
	Ghost.set_game_map(Map);

	//背景初始化
	Background.LoadBitmapByString({
		"Resources/googleMap0.bmp",
		"Resources/googleMap1.bmp",
	});
	Background.SetTopLeft(Background.window_shift[0], Background.window_shift[1]);
	Background.SetFrameIndexOfBitmap(0);

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
	});
	Pacman.SetTopLeft((Pacman[0] - 2) * 16 + Pacman.window_shift[0], Pacman[1] * 16 + Pacman.window_shift[1]);
	Pacman.SetFrameIndexOfBitmap(0);

	//豆子,大力丸初始化
	for (int i = 0; i < Map.map_len[0]; i++) {
		for (int j = 0; j < Map.map_len[1]; j++) {
			//為0的道路加入豆子
			if (Map[i][j] == 0) {
				CMovingBitmap* t = new CMovingBitmap;
				t -> LoadBitmapA("Resources/words/coin.bmp");
				t -> SetTopLeft(16 * (j - 2) + 6 + Map.window_shift[0], 16 * i + 6 + Map.window_shift[1]);
				Score.add_coin(t);
				Score.add_map_point(pair<int, int>(j, i), Score.get_coin_nums());
				Score.set_coin_nums(1);
			}
			//為3的道路加入大力丸
			else if (Map[i][j] == 3) {
				CMovingBitmap t;
				t.LoadBitmapA("Resources/words/dot.bmp");
				t.SetTopLeft(16 * (j - 2) + 4 + Map.window_shift[0], 16 * i + 4 + Map.window_shift[1]);
				Score.add_power_pellets(t);
			}
		}
	}

	//P1初始化
	P1_icon.LoadBitmapA("Resources/words/P1.bmp");
	P1_icon.SetTopLeft(P1_icon.window_shift[0], P1_icon.window_shift[1]);

	//Ready圖標初始化
	Ready_icon.LoadBitmapA("Resources/words/ready.bmp");
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
	//偵測是否吃到大力丸
	Score.get_power(Pacman);

	//debug
	debugText(Pacman);
}