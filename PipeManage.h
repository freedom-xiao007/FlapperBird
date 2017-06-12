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
	std::vector<Pipe> pipes;//���ڴ洢��ʾ��ˮ��
	int speed;//ˮ���ƶ����ٶ�
	int interval;//ˮ��֮��ļ����ˮƽ�����
	int winHeight;
	int winWidth;
};

