#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <queue>

using namespace game_framework;

typedef struct NODE {
	int x, y,
		f, g, h;
	NODE(int a, int b) { x = a, y = b; }
	bool operator<(const NODE &a) const {
		return f == a.f ? g > a.g : f > a.f;
	}
}Node;


int manhattan(int x, int y, int x1, int y1) {
	return (abs(x - x1) + abs(y - y1)) * 10;
}
bool CGameStateRun::isVaildNode(int x, int y, int xx, int yy) {
	if (x < 0 || x >= map_len[1] || y < 0 || y >= map_len[0])
		return false;
	if (gameMap[y][x] == 1)
		return false;
	if(x != xx && y != yy &&(gameMap[yy][x] == 1 || gameMap[y][xx] == 1))
		return false;
	return true;
}
int CGameStateRun::astar(int x0, int y0, int x1, int y1) {
	if (x0 == x1 && y0 == y1) {
		return 4;

	// 設定變數
	bool close[sizeof(gameMap[0]) / sizeof(gameMap[0][0])][sizeof(gameMap) / sizeof(gameMap[0])] = {};
	int valueF[sizeof(gameMap[0]) / sizeof(gameMap[0][0])][sizeof(gameMap) / sizeof(gameMap[0])] = {};
	priority_queue<Node> open;
	int pre[sizeof(gameMap[0]) / sizeof(gameMap[0][0])][sizeof(gameMap) / sizeof(gameMap[0])][3] = {};
	const int nextPos[4][2] = { {1,0},{0,-1},{-1,0},{0,1} };
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
			//偵測周圍4個點是否可以走
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
		int x = x1, y = y1, count = 0, temp;
		while (pre[x][y][0] != x0 || pre[x][y][1] != y0) {
			temp = pre[x][y][0];
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
	}
	return -1;
}