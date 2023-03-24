#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

void CGameStateRun::move_pacman() {
	//if pacman had took a step(one step = 16px)
	if (pacman_total_step == 16) {
		//updte pacman's position on the map
		update_position(pacman_dir_now, pacman_position);
		
		//if the position that pacman prefer is executable 
		if (objCanMove(pacman_dir_waitfor, pacman_position[0], pacman_position[1])) {
			//change the diraction to the new one
			pacman_dir_now = pacman_dir_waitfor;
			//set pacman's picture to the orther one that match with the diraction
		}
		//reset pacman's total step
		pacman_total_step = 0;
	}
	//pacman's animetion when it move
	if (pacman_total_step % 16 < 5) {
		pacman.SetFrameIndexOfBitmap(pacman_dir_now * 2 + 1);
	}
	else if(pacman_total_step % 16 < 10){
		pacman.SetFrameIndexOfBitmap(pacman_dir_now * 2 + 2);
	}
	else {
		pacman.SetFrameIndexOfBitmap(0);
	}
	//if the diraction now is executable keep going
	if (objCanMove(pacman_dir_now, pacman_position[0], pacman_position[1])) {
		switch (pacman_dir_now)
		{
			case 0:
				pacman.SetTopLeft(pacman.GetLeft() + pacman_velocity, pacman.GetTop());
				break;
			case 1:
				pacman.SetTopLeft(pacman.GetLeft(), pacman.GetTop() - pacman_velocity);
				break;
			case 2:
				pacman.SetTopLeft(pacman.GetLeft() - pacman_velocity, pacman.GetTop());
				break;
			case 3:
				pacman.SetTopLeft(pacman.GetLeft(), pacman.GetTop() + pacman_velocity);
				break;
			default:
				break;
		}
		pacman_total_step += pacman_velocity;
	}
	//if pacman hit the wall (include portal)
	else {
		//check that if pacman hit a portal
		if (portal_detect(&pacman, pacman_position, portal_position)) {}
		//check that if the position that pacman prefer is executable 
		else if (objCanMove(pacman_dir_waitfor, pacman_position[0], pacman_position[1])) {
			//change the diraction
			pacman_dir_now = pacman_dir_waitfor;
		}
	}

	
}