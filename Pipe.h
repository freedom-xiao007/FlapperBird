#pragma once
#include <vector>

class Pipe
{
public:
	Pipe(void);
	Pipe(int pos, int wh, int preH);
	~Pipe(void);

	int getPos();//��ȡˮ�ܵ� Y ����
	void setPos(int newPos);//����ˮ�ܵ� Y ����
	int getTopHeight();
	int getBottomHeight();

	//CImage getPipe();
	//CImage getTopPipe();
	//CImage getBottomPipe();

private:
	//CImage pipe, topPipe, bottomPipe;
	int pos;//ˮ�ܵ� Y ����
	int topHeight;//�Ϸ�ˮ�ܵĸ߶�
	int bottomHeight;//�·�ˮ�ܵĸ߶�
	int wh;
};

