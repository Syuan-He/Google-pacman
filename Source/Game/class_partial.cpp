#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

//設定初始容器內的參數
TwoEleContainer::TwoEleContainer(int x, int y) {
	container[0] = x;
	container[1] = y;
}

//設定容器內的參數
void TwoEleContainer::set_value(int x, int y) {
	container[0] = x;
	container[1] = y;
}

//取得容器內的參數
int& TwoEleContainer::operator[](int index) {
	return container[index];
}

//設定初始位移(相對視窗)
UIObject::UIObject(int x, int y) {
	window_shift.set_value(x, y);
}

//設定初始位移(相對視窗)和物件數量
MultUIObj::MultUIObj(int num, int x, int y) {
	window_shift.set_value(x, y);
	obj_num = num;
}

//加入物件
void MultUIObj::add_obj(const CMovingBitmap& o) {
	objs.push_back(o);
}

//取得物件數量
int MultUIObj::get_nums() {
	return obj_num;
}

//設定物件數量(mode 0: 採累加, mode 1: 採直接設定)
void MultUIObj::set_nums(int increase, int mode) {
	if (mode == 0) {
		obj_num += increase;
	}
	else if (mode == 1) {
		obj_num = increase;
	}
}

//回傳第index個CMovingBitmap
CMovingBitmap& MultUIObj::operator[](int index) {
	return objs[index];
}