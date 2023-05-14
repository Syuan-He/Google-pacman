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
	//遊戲開始時間
	exc_time_begin = time(NULL);
	Game_audio -> Play(0);
}

time_t choasTimeChange;
void CGameStateRun::OnMove()							// 移動遊戲元素
{
	//階段2才能移動
	if (phase == 1) {
		if (flag != ghostCatchTime) {
			Sleep(1250);
			flag = ghostCatchTime;
		}
		Pacman.move();

		/*
		//ghosts[1].inHomeAnim();
		//ghosts[2].inHomeAnim();
		
		ghosts[0].outDoorAnim();
		//*/
		//模式改變前的方向改變
		//*
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
		else if ((time(NULL) - choasTime) == choasTimeLong) {
			ghostCatchTime = 0;
			flag = 0;
			Game_audio->Stop(AUDIO_POWERUP);
			for (GameGhost &obj : ghosts) {
				if (obj.isChoas != 2) {
					obj.isChoas = false;
					obj.choasFlash = false;
					obj.setVelocity(6);
				}
			}
		}
		else if ((time(NULL) - choasTime) > choasTimeLong - 3) {
			for (GameGhost &obj : ghosts) {
				obj.choasFlash = true;
			}
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
			Pacman.SetFrameIndexOfBitmap(0);
			Map.Background.SetAnimation(300, false);
			exc_time_begin = time(NULL);
		}//*/
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//更換關卡
	change_level(level);
	
	//分數初始化
	Score.initialize(Map);

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
	Pacman.initialize();

	//Blinky 初始化
	ghosts[0].LoadBitmapByString({
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
		"Resources/choas/choas3.bmp",
		"Resources/eyes/eyes3.bmp",
		"Resources/eyes/eyes0.bmp",
		"Resources/eyes/eyes2.bmp",
		"Resources/eyes/eyes1.bmp",
		"Resources/words/200.bmp",
		"Resources/words/400.bmp",
		"Resources/words/800.bmp",
		"Resources/words/1600.bmp",
		}, RGB(0, 0, 0));
	ghosts[0].initialize();

	//Pinky 初始化
	ghosts[1].LoadBitmapByString({
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
		"Resources/choas/choas3.bmp",
		"Resources/eyes/eyes3.bmp",
		"Resources/eyes/eyes0.bmp",
		"Resources/eyes/eyes2.bmp",
		"Resources/eyes/eyes1.bmp",
		"Resources/words/200.bmp",
		"Resources/words/400.bmp",
		"Resources/words/800.bmp",
		"Resources/words/1600.bmp",
		}, RGB(0, 0, 0));
	ghosts[1].initialize();

	//Inky 初始化
	ghosts[2].LoadBitmapByString({
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
		"Resources/choas/choas3.bmp",
		"Resources/eyes/eyes3.bmp",
		"Resources/eyes/eyes0.bmp",
		"Resources/eyes/eyes2.bmp",
		"Resources/eyes/eyes1.bmp",
		"Resources/words/200.bmp",
		"Resources/words/400.bmp",
		"Resources/words/800.bmp",
		"Resources/words/1600.bmp",
		}, RGB(0, 0, 0));
	ghosts[2].initialize();

	//Clyde 初始化
	ghosts[3].LoadBitmapByString({
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
		"Resources/choas/choas3.bmp",
		"Resources/eyes/eyes3.bmp",
		"Resources/eyes/eyes0.bmp",
		"Resources/eyes/eyes2.bmp",
		"Resources/eyes/eyes1.bmp",
		"Resources/words/200.bmp",
		"Resources/words/400.bmp",
		"Resources/words/800.bmp",
		"Resources/words/1600.bmp",
		}, RGB(0, 0, 0));
	ghosts[3].initialize();

	initialGhosts();

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
		unique_ptr<CMovingBitmap> t(new CMovingBitmap);
		t -> LoadBitmapByString({
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
		t -> SetFrameIndexOfBitmap(0);
		t -> SetTopLeft(i * 16 + Score.game_scores.window_shift[0], Score.game_scores.window_shift[1]);
		Score.game_scores.add_obj(*t);
	}

	//血條初始化
	for (int i = 0; i < Pacman.hearts_icon.get_nums(); i++) {
		unique_ptr<CMovingBitmap> t(new CMovingBitmap);
		t -> LoadBitmapA("Resources/pacman/pacman5.bmp");
		t -> SetTopLeft(i * 32 + Pacman.hearts_icon.window_shift[0], Pacman.hearts_icon.window_shift[1]);
		Pacman.hearts_icon.add_obj(*t);
	}

	Game_audio->Load(AUDIO_BEGIN, "Resources/audio/pacman_beginning.wav");
	Game_audio->Load(AUDIO_MOVE, "Resources/audio/pacman_wakka.wav");
	Game_audio->Load(AUDIO_DIE, "Resources/audio/pacman_death.wav");
	Game_audio->Load(AUDIO_EAT_FRUIT, "Resources/audio/pacman_eatfruit.wav");
	Game_audio->Load(AUDIO_EAT_GHOST, "Resources/audio/pacman_eatghost.wav");
	Game_audio->Load(AUDIO_MUTIPLAYER, "Resources/audio/pacman_extrapac.wav");
	Game_audio->Load(AUDIO_INTERMISSION, "Resources/audio/pacman_intermission.wav");
	Game_audio->Load(AUDIO_SIREN, "Resources/audio/pacman_siren.wav");
	Game_audio->Load(AUDIO_POWERUP, "Resources/audio/pacman_power_up.wav");
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
	//按T 以啟用debug mod
	case 0x54:
		debug_mod = !debug_mod;
		break;
	default:
		break;
	}
	if (debug_mod) {
		switch (nChar){
		//按W 進入下一關
		case 0x57:
			phase = 4;
			break;
		//按S 進入上一關
		case 0x53:
			phase = 4;
			if (level >= 1) {
				level -= 2;
			}
			break;
		//按I 以啟用無敵模式
		case 0x49:
			invincible = !invincible;
			break;
		default:
			break;
		}
	}
	else {
		invincible = false;
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
	//偵測是否吃到豆子
	if (Score.get_point(Pacman)) {
		//播放音效
		Game_audio -> Resume();
		//重置計數器
		Pacman.reset_step_counter();
	}
	//再走為一步前不得取消音效
	else if (Pacman.get_step_counter() >= Pacman.get_velocity()) {
		Game_audio->Pause_one(AUDIO_MOVE);
		Pacman.reset_step_counter();
	}	
	//偵測是否吃到大力丸、鬼進入混亂模式
	if (Score.get_power(Pacman)) {
		Game_audio -> Play(AUDIO_POWERUP, true);
		ghostCatchTime = 0;
		flag = 0;
		for (GameGhost &obj : ghosts) {
			if (obj.isChoas != 2) {
				obj.isChoas = true;
				obj.choasFlash = false;
				//減緩鬼的移動速度
				obj.setVelocity(12);
				obj.update_moving_schedule();
			}
		}
		ghostTurnBack();
		choasTime = time(NULL);
		choasTimeChange = choasTime;
	}

	//pacman是否被鬼抓到
	pacman_get_catch();

	//debug
	if(debug_mod) debugText();
	//顯示物件
	show_obj_by_phase();
}
