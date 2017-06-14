#include "StdAfx.h"
#include "Pipe.h"

#include <time.h>

Pipe::Pipe(void)
{
}

Pipe::Pipe(int pos, int wh, int preH)
	:pos(pos)
{
	wh = wh;
	//采用随机数生成上方水管的高度
	srand((unsigned)time(NULL));
	topHeight = rand()%(wh-125);
	if(abs(topHeight-preH) <= 15) {
		srand((unsigned)time(NULL)+1000);
		topHeight = rand()%(wh-125);
	}
	//topHeight = 100;
	bottomHeight = (wh-125) - topHeight;

	//CString imagePath;
	//imagePath = L".\\res\\pipe.png";
	//pipe.Load(imagePath);
	//imagePath = L".\\res\\pipe-down.png";
	//bottomPipe.Load(imagePath);
	//imagePath = L".\\res\\pipe-up.png";
	//topPipe.Load(imagePath);
}


Pipe::~Pipe(void)
{
}

int Pipe::getPos() {
	return pos;
}

void Pipe::setPos(int newPos) {
	pos = newPos;
}

int Pipe::getTopHeight() {
	return topHeight;
}

int Pipe::getBottomHeight() {
	return bottomHeight;
}
//
//CImage Pipe::getPipe() {
//	return pipe;
//}
//
//CImage Pipe::getTopPipe() {
//	return topPipe;
//}
//
//CImage Pipe::getBottomPipe() {
//	return bottomPipe;
//}