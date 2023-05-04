#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	Sleep(200);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	ShowInitProgress(66, "Initialize...");
	Sleep(200);
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//

	Background.LoadBitmapA("Resources/initialize/main_background.bmp");
	Background.SetTopLeft(0, 0);
	
	vector<string>t = { "Resources/initialize/main_menu_play.bmp", "Resources/initialize/main_menu_back.bmp", "Resources/initialize/main_menu_setting.bmp"};
	Menu_main.load_menu(t);
	Menu_main.menu[2].SetTopLeft(0, 500);

	Menu_main.target.LoadBitmapByString({
		"Resources/initialize/main_target_0.bmp",
		"Resources/words/NULL.bmp",
		}, RGB(0, 0, 0));
	Menu_main.target.SetTopLeft(Menu_main.target.window_shift[0], Menu_main.target.window_shift[1]);
	Menu_main.target.SetAnimation(400, false);

	Setting_background.LoadBitmapA("Resources/initialize/setting_background.bmp");
	Setting_background.SetTopLeft(0, 0);

	Menu_setting.target.LoadBitmapA("Resources/initialize/setting_target.bmp", RGB(0, 0, 0));
	Menu_setting.target.SetTopLeft(147, 152);
	Menu_setting.menu.set_nums(6);

}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (menu_now == 0) {
		switch (nChar) {
		case VK_UP:
			Menu_main.set_target_pos(-1);
			break;
		case VK_DOWN:
			Menu_main.set_target_pos(1);
			break;
		case VK_RETURN:
			switch (Menu_main.get_target_pos()) {
			case 0:
				menu_now = 1;
				break;
			case 1:
				GotoGameState(GAME_STATE_OVER);
				break;
			case 2:
				menu_now = 2;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	else if (menu_now == 2) {
		switch (nChar) {
		case VK_UP:
			if (Menu_setting.set_target_pos(-1, 0)) {
				Menu_setting.target.SetTopLeft(Menu_setting.target.GetLeft(), Menu_setting.target.GetTop() - 34);
			}
			break;
		case VK_DOWN:
			if (Menu_setting.set_target_pos(1, 0)) {
				Menu_setting.target.SetTopLeft(Menu_setting.target.GetLeft(), Menu_setting.target.GetTop() + 34);
			}
			break;
		case VK_RETURN:
			switch (Menu_setting.get_target_pos()) {
			case 4:
			case 5:
				menu_now = 0;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
	
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
			// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	if (menu_now == 0) {
		Background.ShowBitmap();
		Menu_main.show_menu();
	}
	else if (menu_now == 1) {
		GotoGameState(GAME_STATE_RUN);
	}
	else if (menu_now == 2) {
		Setting_background.ShowBitmap();
		Menu_setting.target.ShowBitmap();
	}
	//draw_text();
}