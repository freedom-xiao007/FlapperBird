#pragma once
#include <vector>
#include "Pipe.h"

class PipeManage
{
public:
	PipeManage();
	PipeManage(int windowHeight, int windowWidth);
	~PipeManage(void);

	void manage();
	void create();
	std::vector<Pipe> getPipes();

private:
	std::vector<Pipe> pipes;//用于存储显示的水管
	int speed;//水管移动的速度
	int interval;//水管之间的间隔（水平间隔）
	int winHeight;
	int winWidth;
};

