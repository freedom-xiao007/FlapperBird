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
	//���ˮ��������û��ˮ�ܣ����ɵ�һ��ˮ��
	if(pipes.empty()) {
		create();
	}

	//���ȶԴ��ڵ�ˮ�ܽ����ƶ����Ƚ�������ж�Ӧ�ı䣩
	for(int i=0; i < pipes.size(); i++) {
		int originPos = pipes[i].getPos();
		if(originPos-speed <= 0) {
			pipes[i].setPos(0);
		}
		else {
			pipes[i].setPos(originPos-speed);
		}
	}

	//�ж��Ƿ���Բ����µ�ˮ�ܣ����һ��ˮ�ܵ�λ�ô��ڱ߽�������ˮ�ܾ���
	if(pipes.back().getPos() <= 250) {
		create();
	}

	//�жϵ�һ��ˮ���ƶ��Ƿ��˿�ͷ�������߽��ɾ����
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