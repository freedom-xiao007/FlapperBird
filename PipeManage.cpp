#include "StdAfx.h"
#include "PipeManage.h"

PipeManage::PipeManage() {

}

PipeManage::PipeManage(int windowHeight, int windowWidth)
	:winHeight(windowHeight),winWidth(windowWidth)
{
	pipes.clear();
	speed = 5;
	interval = 100;
}


PipeManage::~PipeManage(void)
{
}

void PipeManage::manage() {
	//如果水管容器中没有水管，生成第一个水管
	if(pipes.empty()) {
		create();
	}

	//首先对存在的水管进行移动（既将坐标进行对应改变）
	for(int i=0; i < pipes.size(); i++) {
		int originPos = pipes[i].getPos();
		if(originPos-speed <= 0) {
			pipes[i].setPos(0);
		}
		else {
			pipes[i].setPos(originPos-speed);
		}
	}

	//判断是否可以产生新的水管，最后一根水管的位置窗口边界距离大于水管距离
	if(pipes.back().getPos() <= 250) {
		create();
	}

	//判断第一个水管移动是否到了开头，超过边界既删除掉
	if(pipes.front().getPos() <= 0) {
		pipes.erase(pipes.begin());
	}
}

void PipeManage::create() {
	Pipe pipe;
	if(pipes.empty()) {
		 pipe = Pipe(winWidth, winHeight, 0);
	}
	else {
		 pipe = Pipe(winWidth, winHeight, pipes.back().getTopHeight());
	}
	pipes.push_back(pipe);
}

std::vector<Pipe> PipeManage::getPipes() {
	return pipes;
}

void PipeManage::pipeClear() {
	pipes.clear();
}