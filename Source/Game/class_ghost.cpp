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

void GameGhost::move(int x1, int y1) {

	//if pacman had took a step(one step = 16px)
	if (total_step == 16) {
		//updte pacman's position on the map
		update_position(dir_now);
		if (setDirLock) {
			setDirLock = false;
		}
		else if (isChoas) {
			if (CanMove((dir_now + 1) % 4) || CanMove((dir_now + 3) % 4)) {
				dir_waitfor = rand() % 4;
			}
		}
		else {
			dir_waitfor = selectDir(dir_now, x1, y1);
		}

		//if the position that pacman prefer is executable 
		if (CanMove(dir_waitfor)) {
			//change the diraction to the new one
			dir_now = dir_waitfor;
			//set pacman's picture to the orther one that match with the diraction
		}
		//reset pacman's total step
		total_step = 0;
	}
	else if (!CanMove(dir_now) && total_step == 0 && (position[0] != x1 || position[1] != y1)) {
		turnBack();
		//dir_waitfor = astar(position[0], position[1], x1, y1);
	}
	//pacman's animetion when it move
	if (isChoas) {
		if (choasFlash && total_step % 16 < 4) {
			this->SetFrameIndexOfBitmap(11);
		}
		else if (total_step % 16 < 8) {
			this->SetFrameIndexOfBitmap(8);
		}
		else if (choasFlash && total_step % 16 < 12) {
			this->SetFrameIndexOfBitmap(10);
		}
		else {
			this->SetFrameIndexOfBitmap(9);
		}
	}
	else {
		if (total_step % 16 < 8) {
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
			this->SetTopLeft(this->GetLeft() + velocity, this->GetTop());
			break;
		case 1:
			this->SetTopLeft(this->GetLeft(), this->GetTop() - velocity);
			break;
		case 2:
			this->SetTopLeft(this->GetLeft() - velocity, this->GetTop());
			break;
		case 3:
			this->SetTopLeft(this->GetLeft(), this->GetTop() + velocity);
			break;
		default:
			break;
		}
		total_step += velocity;
	}
	//if pacman hit the wall (include portal)
	else {
		//check that if pacman hit a portal
		int* t = gameMap.portal_detect(position[0], position[1]);
		if (t != nullptr) {
			position[0] = t[0];
			position[1] = t[1];
			this->SetTopLeft(16 * (position[0] - 2) + window_shift[0], 16 * position[1] + window_shift[1]);
		}
		//check that if the position that pacman prefer is executable 
		else if (CanMove(dir_waitfor)) {
			//change the diraction
			dir_now = dir_waitfor;
		}
	}
}

void GameGhost::turnBack() {
	dir_waitfor = (dir_now + 2) % 4;
	setDirLock = true;
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

int GameGhost::getDirWait() {
	return dir_waitfor;
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
void CGameStateRun::ghostChase() {
	Blinky.move(Pacman[0], Pacman[1]);
	Pinky.move(Pacman[0] + 4 * nextPos[Pacman.getDirNow()][0], Pacman[1] + 4 * nextPos[Pacman.getDirNow()][1]);
	Inky.move(2 * Pacman[0] + 4 * nextPos[Pacman.getDirNow()][0] - Blinky[0], 2 * Pacman[1] + 4 * nextPos[Pacman.getDirNow()][1] - Blinky[1]);
	if (pythagorean(Clyde[0], Clyde[1], Pacman[0], Pacman[1]) > 8.0) {
		Clyde.move(Pacman[0], Pacman[1]);
	}
	else {
		Clyde.move(3, 16);
	}
}
void CGameStateRun::ghostScatter() {
	Blinky.move(58, 0);
	Pinky.move(3, 0);
	Inky.move(58, 16);
	Clyde.move(3, 16);
}
void CGameStateRun::ghostTurnBack() {
	Blinky.turnBack();
	Pinky.turnBack();
	Inky.turnBack();
	Clyde.turnBack();
}