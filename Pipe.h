#pragma once
#include <vector>

class Pipe
{
public:
	Pipe(void);
	Pipe(int pos, int wh, int preH);
	~Pipe(void);

	int getPos();//获取水管的 Y 坐标
	void setPos(int newPos);//设置水管的 Y 坐标
	int getTopHeight();
	int getBottomHeight();

	//CImage getPipe();
	//CImage getTopPipe();
	//CImage getBottomPipe();

private:
	//CImage pipe, topPipe, bottomPipe;
	int pos;//水管的 Y 坐标
	int topHeight;//上方水管的高度
	int bottomHeight;//下方水管的高度
	int wh;
};

