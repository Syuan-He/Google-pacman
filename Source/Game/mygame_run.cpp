#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <string>

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
	move_pacman(); //移動pacman
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//載入地圖map
	map_loader(map_dir);
	
	//載入地圖圖檔
	background.LoadBitmapByString({
		"Resources/googleMap0.bmp",
		"Resources/googleMap1.bmp"
	});
	//初始化地圖位置
	background.SetTopLeft(0 + window_shift[0], 0 + window_shift[1]);

	//載入pacman圖檔
	pacman.LoadBitmapByString({ 
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
	//初始化pacman位置
	pacman.SetTopLeft(554 + window_shift[0], 234 + window_shift[1]);
	//初始化pacman的第一張圖
	pacman.SetFrameIndexOfBitmap(0);

	//載入coins, power pellets
	for (int i = 0; i < map_len[0]; i++) {
		for (int j = 0; j < map_len[1]; j++) {
			if (gameMap[i][j] == 0) {
				CMovingBitmap t;
				t.LoadBitmapA("Resources/words/coin.bmp");
				t.SetTopLeft(16 * j + origin_position_shift[0] + 12 + window_shift[0], 16 * i + origin_position_shift[1] + 12 + window_shift[1]);
				coins.push_back(t);
				total_coin_nums ++;
			}
			if (gameMap[i][j] == 3){
				CMovingBitmap t;
				t.LoadBitmapA("Resources/words/dot.bmp");
				t.SetTopLeft(16 * j + origin_position_shift[0] + 10 + window_shift[0], 16 * i + origin_position_shift[1] + 10 + window_shift[1]);
				power_pellets.push_back(t);
			}
		}
	}

	//分數條初始化
	for (int i = 0; i < score_digits; i++) {
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
		t.SetTopLeft(i * 16 + window_shift[0], window_shift[1] - 30);
		game_scores.push_back(t);
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//監測方向鍵
	switch (nChar) {
	case VK_RIGHT:
		pacman_dir_waitfor = 0;
		break;
	case VK_LEFT:
		pacman_dir_waitfor = 2;
		break;
	case VK_UP:
		pacman_dir_waitfor = 1;
		break;
	case VK_DOWN:
		pacman_dir_waitfor = 3;
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
	show_image_by_phase(); //顯示物件
	debugText();
	get_point(&pacman); //偵測是否吃到豆子
	get_power(&pacman);//偵測是否吃到大力丸
}

void CGameStateRun::debugText() {
	CDC *pDC = CDDraw::GetBackCDC();
	string strPacPos = "", strPacPoi = "";

	strPacPos += to_string(pacman_position[0]) + ", " + to_string(pacman_position[1]);	//position in array
	strPacPoi += to_string(pacman.GetLeft()) + ", " + to_string(pacman.GetTop());

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 20 + window_shift[0], 270 + window_shift[1], strPacPos);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 20 + window_shift[0], 300 + window_shift[1], strPacPoi);

	CDDraw::ReleaseBackCDC();
}