#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <queue>
#include <cstdlib>
#include <time.h>

using namespace game_framework;

const int nextPos[4][2] = { {1,0},{0,-1},{-1,0},{0,1} };

void GameGhost::initialize() {
	Character::initialize();
	isChoas = 0;
	choasFlash = false;
	setDirLock = false;
}

void GameGhost::inHomeAnim() {
	
	if (GetTop() > 16 * (initial_pos[1]) + window_shift[1] +56) {
		dir_waitfor = 1;
	}
	else if (GetTop() < 16 * (initial_pos[1]) + window_shift[1] + 40) {
		dir_waitfor = 3;
	}
	

	switch (dir_waitfor)
	{
	case 1:
		this->SetTopLeft(this->GetLeft(), this->GetTop() - 1);
		break;
	case 3:
		this->SetTopLeft(this->GetLeft(), this->GetTop() + 1);
		break;
	}
	if (isChoas == 1) {
		if (GetTop() % 8 < 4) {
			if (choasFlash && clock() % 300 > 150) {
				this->SetFrameIndexOfBitmap(10);
			}
			else {
				this->SetFrameIndexOfBitmap(8);
			}
		}
		else {
			if (choasFlash && clock() % 300 > 150) {
				this->SetFrameIndexOfBitmap(11);
			}
			else {
				this->SetFrameIndexOfBitmap(9);
			}
		}
	}
	else {
		if (GetTop() % 8 < 4) {
			this->SetFrameIndexOfBitmap(dir_waitfor * 2);
		}
		else {
			this->SetFrameIndexOfBitmap(dir_waitfor * 2 + 1);
		}
	}
}
void GameGhost::outDoorAnim() {

	if (!setDirLock && GetTop() < 16 * (initial_pos[1]) + window_shift[1] + 48) {
		dir_waitfor = 3;
	}
	else if (!setDirLock && GetTop() > 16 * (initial_pos[1]) + window_shift[1] + 48) {
		dir_waitfor = 1;
	}
	else if (!setDirLock && GetLeft() < 16 * (initial_pos[0]) + window_shift[0] - 22) {
		dir_waitfor = 0;
	}
	else if (!setDirLock && GetLeft() > 16 * (initial_pos[0]) + window_shift[0] - 22) {
		dir_waitfor = 2;
	}
	else if (!setDirLock && GetLeft() == 16 * (initial_pos[0]) + window_shift[0] - 22) {
		setDirLock = true;
		dir_waitfor = 1;
		if (isChoas == 2) {
			isChoas = 0;
		}
	}
	else if (GetTop() <= 16 * initial_pos[1] + window_shift[1]) {
		dir_waitfor = 2;
		if (GetLeft() <= 16 * (initial_pos[0] - 2) + window_shift[0]) {
			inHome = false;
			setPos(initial_pos[0], initial_pos[1]);
			total_step = 0;
			dir_now = 2;				//目前移動方向
			dir_waitfor = 2;
			if (isChoas != 1) {
				waitVelocity = 6;
			}
			update_moving_schedule();
			setDirLock = false;

		}
	}

	switch (dir_waitfor)
	{
	case 0:
		this->SetTopLeft(this->GetLeft() + 1, this->GetTop());
		break;
	case 1:
		this->SetTopLeft(this->GetLeft(), this->GetTop() - 1);
		break;
	case 2:
		this->SetTopLeft(this->GetLeft() - 1, this->GetTop());
		break;
	case 3:
		this->SetTopLeft(this->GetLeft(), this->GetTop() + 1);
		break;
	}
	if (isChoas == 1) {
		if (GetTop() % 8 < 4) {
			if (choasFlash && clock() % 300 > 150) {
				this->SetFrameIndexOfBitmap(10);
			}
			else {
				this->SetFrameIndexOfBitmap(8);
			}
		}
		else {
			if (choasFlash && clock() % 300 > 150) {
				this->SetFrameIndexOfBitmap(11);
			}
			else {
				this->SetFrameIndexOfBitmap(9);
			}
		}
	}
	else if (isChoas == 2) {
		this->SetFrameIndexOfBitmap(12 + dir_waitfor);
	}
	else {
		if (GetTop() % 8 < 4) {
			this->SetFrameIndexOfBitmap(dir_waitfor * 2);
		}
		else {
			this->SetFrameIndexOfBitmap(dir_waitfor * 2 + 1);
		}
	}
}
bool GameGhost::outDoorRule(time_t eatPointTime) {
	return getPointNum == waitPoints || eatPointTime + 4*(ghostID-1) == time(NULL);
}

//節點
typedef struct NODE {
	int x, y,
		f, g, h;
	NODE(int a, int b) { x = a, y = b; }
	bool operator<(const NODE &a) const {
		return f == a.f ? g > a.g : f > a.f;
	}
}Node;

//曼哈頓距離
int manhattan(int x, int y, int x1, int y1) {
	return (abs(x - x1) + abs(y - y1)) * 10;
}

//畢氏定理距離
float pythagorean(int x, int y, int x1, int y1) {
	return sqrt((float)((x - x1)*(x - x1) + (y - y1)*(y - y1)));
}

//節點是否可進入
bool GameGhost::isVaildNode(int x, int y, int xx, int yy) {
	if (x < 0 || x >= gameMap.map_len[1] || y < 0 || y >= gameMap.map_len[0])
		return false;
	if (gameMap[y][x] == 1)
		return false;
	if (x != xx && y != yy && (gameMap[yy][x] == 1 || gameMap[y][xx] == 1))
		return false;
	return true;
}

//AStare
int GameGhost::astar(int x0, int y0, int x1, int y1) {
	if (x0 == x1 && y0 == y1) {
		return 4;
	}

	// variable
	int y_len = gameMap.map_len[0], x_len = gameMap.map_len[1];
	vector<vector<bool>> close = vector<vector<bool>>(x_len, vector<bool>(y_len));
	vector<vector<int>> valueF = vector<vector<int>>(x_len, vector<int>(y_len));
	priority_queue<Node> open;
	vector<vector<vector<int>>> pre = vector<vector<vector<int>>>(x_len, vector< vector<int>>(y_len, vector<int>(3)));
	Node node(x0, y0);
	node.g = 0;
	node.h = manhattan(x0, y0, x1, y1);
	node.f = node.g + node.h;
	valueF[x0][y0] = node.f;
	open.push(node);

	//main
	while (!open.empty()) {
		Node node_current = open.top();
		open.pop();
		close[node_current.x][node_current.y] = true;
		if (node_current.x == x1 && node_current.y == y1)
			break;
		for (int i = 0; i < 4; i++) {
			Node node_next(node_current.x + nextPos[i][0], node_current.y + nextPos[i][1]);
			if (isVaildNode(node_next.x, node_next.y, node_current.x, node_current.y) && !close[node_next.x][node_next.y]) {
				node_next.g = node_current.g + int(sqrt(pow(nextPos[i][0], 2)) + sqrt(pow(nextPos[i][1], 2)));
				node_next.h = manhattan(node_next.x, node.y, x1, y1);
				node_next.f = node_next.g + node_next.h;
				if (node_next.f < valueF[node_next.x][node_next.y] || valueF[node_next.x][node_next.y] == 0) {

					pre[node_next.x][node_next.y][0] = node_current.x;
					pre[node_next.x][node_next.y][1] = node_current.y;
					pre[node_current.x][node_current.y][2] = i;
					valueF[node_next.x][node_next.y] = node_next.f;
					open.push(node_next);
				}
			}
		}
	}
	int x = x1, y = y1, count = 0;
	while (pre[x][y][0] != x0 || pre[x][y][1] != y0) {
		int temp = pre[x][y][0];
		y = pre[x][y][1];
		x = temp;
		count++;
		if (count >= 1000)
			break;
	}
	for (int i = 0; i < 4; i++) {
		if (nextPos[i][0] == x - x0 && nextPos[i][1] == y - y0)
			return i;
	}

	return -1;
}
int GameGhost::selectDir(int dir, int x1, int y1) {
	float dist = 0, nextDist;
	int dirNext;
	dist = pythagorean(position[0] + nextPos[(dir + 3) % 4][0], position[1] + nextPos[(dir + 3) % 4][1], x1, y1);
	dirNext = (dir + 3) % 4;
	for (int i = 4; i < 6; i++) {
		nextDist = pythagorean(position[0] + nextPos[(dir + i) % 4][0], position[1] + nextPos[(dir + i) % 4][1], x1, y1);
		if (nextDist > dist) {
			dirNext = (dir + i) % 4;
			dist = nextDist;
		}
	}
	for (int i = 3; i < 6; i++) {
		nextDist = pythagorean(position[0] + nextPos[(dir + i) % 4][0], position[1] + nextPos[(dir + i) % 4][1], x1, y1);
		if (CanMove((dir + i) % 4) && nextDist < dist) {
			dirNext = (dir + i) % 4;
			dist = nextDist;
		}
	}
	return dirNext;
}

void GameGhost::move(int x1, int y1, int portal_mode) {	
	//if pacman had took a step(one step = 16px)
	if (total_step == velocity) {
		//updte pacman's position on the map
		update_position(dir_now);

		//為避免圖片偏移，在ghost完整移動完才更新速度的設定
		if (waitVelocity != velocity) {
			velocity = waitVelocity;
			update_moving_schedule();
		}

		//更新移動方向
		if (setDirLock) {	//確保turnBack時 dir 與 dir_waitfor 不會被追蹤程式覆蓋掉
			setDirLock = false;
		}
		else {
			switch (isChoas) {

				//混亂狀態
			case 1:
				if (CanMove((dir_now + 1) % 4) || CanMove((dir_now + 3) % 4)) {
					dir_waitfor = rand() % 4;
				}
				break;

				//goHome
			case 2:
				if (position[0] == initial_pos[0] && position[1] == initial_pos[1]) {
					inHome = true;
				}
				dir_waitfor = selectDir(dir_now, initial_pos[0], initial_pos[1]);
				break;

				//正常狀態
			case 0:
				switch (chaseMode) {
				case 0:
					dir_waitfor = selectDir(dir_now, x1, y1);
					break;
				case 1:
					dir_waitfor = astar(position[0], position[1], x1, y1);
					break;
				}
				break;
			}
		}

		//if the position that pacman prefer is executable 
		if (CanMove(dir_waitfor)) {
			//change the diraction to the new one
			dir_now = dir_waitfor;
		}
		//reset pacman's total step
		total_step = 0;

		pair<int, int> t = gameMap.portal_detect(position[0], position[1]);
		if (t.first != -1 && (portal_mode == 0? true: time(NULL) - enter_portal_t > 1)) {
			enter_portal_t = time(NULL);
			position[0] = t.first;
			position[1] = t.second;
			this->SetTopLeft(16 * (position[0] - 2) + window_shift[0], 16 * position[1] + window_shift[1]);
		}
	}
	// 在鬼卡住？時讓鬼走反路
	else if (!CanMove(dir_now) && total_step == 0 && (position[0] != x1 || position[1] != y1)) {
		turnBack();
	}
	//pacman's animetion when it move
	if (isChoas == 1) {
		if (total_step % (velocity / 2) < velocity / 4) {
			if (choasFlash && clock() % 300 > 150) {
				this->SetFrameIndexOfBitmap(10);
			}
			else {
				this->SetFrameIndexOfBitmap(8);
			}
		}
		else {
			if (choasFlash && clock() % 300 > 150) {
				this->SetFrameIndexOfBitmap(11);
			}
			else {
				this->SetFrameIndexOfBitmap(9);
			}
		}
	}
	else if (isChoas == 2) {
		this->SetFrameIndexOfBitmap(12 + dir_now);
	}
	else {
		if (total_step < velocity / 2) {
			this->SetFrameIndexOfBitmap(dir_now * 2);
		}
		else {
			this->SetFrameIndexOfBitmap(dir_now * 2 + 1);
		}
	}

	//if the diraction now is executable keep going
	if (CanMove(dir_now)) {
		switch (dir_now)
		{
		case 0:
			this->SetTopLeft(this->GetLeft() + moving_schedule[total_step], this->GetTop());
			break;
		case 1:
			this->SetTopLeft(this->GetLeft(), this->GetTop() - moving_schedule[total_step]);
			break;
		case 2:
			this->SetTopLeft(this->GetLeft() - moving_schedule[total_step], this->GetTop());
			break;
		case 3:
			this->SetTopLeft(this->GetLeft(), this->GetTop() + moving_schedule[total_step]);
			break;
		default:
			break;
		}
		total_step++;
	}
	else if (CanMove(dir_waitfor)) {
		dir_now = dir_waitfor;
	}
	if (isChoas == 2 && position[0] == initial_pos[0] && position[1] == initial_pos[1]) {
		inHome = true;
	}
}
void GameGhost::turnBack() {
	dir_waitfor = (dir_now + 2) % 4;
	setDirLock = true;
}

void GameGhost::setChaseMode(int mode){
	chaseMode = mode;
}
void GameGhost::setEdgePoint(int x, int y) {
	edgePoint[0] = x;
	edgePoint[1] = y;
}

void GameGhost::set_enter_portal_t() {
	enter_portal_t = time(NULL);
}

int GameGhost::getInitPos(int n) {
	return initial_pos[n];
}

time_t GameGhost::get_enter_portal_t() {
	return enter_portal_t;
}

bool CGameStateRun::isScatterTime() {
	return (time(NULL) - modePlayTime) % (scatterTime + chaseTime) == 0;
}
bool CGameStateRun::isChaseTime() {
	return (time(NULL) - modePlayTime) % (scatterTime + chaseTime) >= scatterTime;
}
bool CGameStateRun::isChoasTime() {
	return (time(NULL) - choasTime) < choasTimeLong;
}

void CGameStateRun::ghostMoveControl() {
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
		ghostCatchCount = 0;
		preGhostCatchCount = 0;
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
}
void CGameStateRun::ghostMove(GameGhost *obj, bool chaseRule) {
	if (obj->stayHome) {
		obj->inHomeAnim();
		if (obj->outDoorRule(eatPointTime)) {
			obj->stayHome = false;
			obj->getPointNum = 0;
		}
	}
	else if (obj->inHome) {
		obj->outDoorAnim();
	}
	else {
		if (chaseRule) {
			switch (obj->ghostID) {
			case 1:
				obj->move(Pacman[0] + 4 * nextPos[Pacman.getDirNow()][0], Pacman[1] + 4 * nextPos[Pacman.getDirNow()][1]);
				break;
			case 2:
				obj->move(2 * Pacman[0] + 4 * nextPos[Pacman.getDirNow()][0] - ghosts[0][0], 2 * ghosts[0][1] + 4 * nextPos[Pacman.getDirNow()][1] - ghosts[0][1]);
				break;
			case 3:
				if (pythagorean((*obj)[0], (*obj)[1], Pacman[0], Pacman[1]) > 8.0) {
					obj->move(Pacman[0], Pacman[1]);
				}
				else {
					obj->move(obj->edgePoint[0], obj->edgePoint[1]);
				}
				break;
			default:
				obj->move(Pacman[0], Pacman[1]);
				break;
			}
		}
		else {
			obj->move(obj->edgePoint[0], obj->edgePoint[1]);
		}
	}
}
void CGameStateRun::ghostTurnBack() {
	for (GameGhost &obj : ghosts) {
		if(!obj.inHome)
			obj.turnBack();
	}
}

void CGameStateRun::initialGhosts() {
	for (GameGhost &obj : ghosts) {
		obj.initialize();
	}
	//*
	ghosts[0].inHome = false;
	ghosts[1].inHome = true;
	ghosts[2].inHome = true;
	ghosts[3].inHome = true;

	ghosts[0].stayHome = false;
	ghosts[1].stayHome = false;
	ghosts[2].stayHome = true;
	ghosts[3].stayHome = true;

	ghosts[1].waitPoints = 0;
	ghosts[2].waitPoints = 7;
	ghosts[3].waitPoints = 15;
	
	ghosts[1].SetTopLeft(16 * (ghosts[1].getInitPos(0)) + ghosts[1].window_shift[0] - 22, 16 * (ghosts[1].getInitPos(1) + 3) + ghosts[1].window_shift[1]);
	ghosts[2].SetTopLeft(16 * (ghosts[2].getInitPos(0)) + ghosts[2].window_shift[0] - 55, 16 * (ghosts[2].getInitPos(1) + 3) + ghosts[2].window_shift[1]);
	ghosts[3].SetTopLeft(16 * (ghosts[3].getInitPos(0)) + ghosts[3].window_shift[0] + 9, 16 * (ghosts[3].getInitPos(1) + 3) + ghosts[3].window_shift[1]);
	ghosts[1].set_dir_waitfor(3);
	ghosts[2].set_dir_waitfor(1);
	ghosts[3].set_dir_waitfor(1);//*/
}

void GameBoss::set_is_using(bool use) {
	is_using = use;
}

bool GameBoss::get_is_using() {
	return is_using;
}