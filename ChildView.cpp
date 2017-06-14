
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "FlappyBird_MFC.h"
#include "ChildView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TEST

#define TIMER_PAINT 1
#define TIMER_HEROMOVE 2

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	delete(pipesManage);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}

void CChildView::OnPaint() 
{
	//CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	//DC 是初始化
	CDC *pdc = this->GetDC();
	GetClientRect(windowsSize);
	//如果没有水管，产生新的水管
	if(pipesManage == NULL) {
		pipesManage = new PipeManage(windowsSize.Height(), windowsSize.Width());
		pipesManage->manage();
	}
	//创建缓冲DC
	cacheDC.CreateCompatibleDC(NULL);
	cacheBitmap.CreateCompatibleBitmap(pdc, windowsSize.Width(), windowsSize.Height());
	cacheDC.SelectObject(&cacheBitmap);

#ifndef TEST
	QFile.open("log.txt", std::ios::app);
	QFile << "DC初始化完成" << "\n"; 
	QFile.close();
#endif
	//if(!isKeyUP) {
	//	if(!isFirstDraw) {
	//		bird.down();
	//	}
	//}
	//isKeyUP = false;

	//游戏的绘制
	//背景的绘制
	background.Draw(cacheDC, windowsSize);
	//像素鸟的绘制
	birdImage.Draw(cacheDC, 50, bird.getPos(), 25, 25);
	//水管的绘制
	//pipesManage->manage();//水管的管理（第一次生成和移动）
	//std::vector<Pipe> pipes = pipesManage->getPipes(); 
	pipes = pipesManage->getPipes();
	for(int i=0; i < pipes.size(); i++) {
		int pipePos = pipes[i].getPos();
		int topHeight = pipes[i].getTopHeight();
		int bottomHeight = pipes[i].getBottomHeight();

		pipe.Draw(cacheDC, pipePos, 0, 25, topHeight);
		topPipe.Draw(cacheDC, pipePos, topHeight, 25, 25);
		bottomPipe.Draw(cacheDC, pipePos, topHeight+25+75, 25, 25);
		pipe.Draw(cacheDC, pipePos, topHeight+25+75+25, 25, bottomHeight);
	}
	//整体绘制
	pdc->BitBlt(0, 0, windowsSize.Width(), windowsSize.Height(), &cacheDC, 0, 0, SRCCOPY);

#ifndef TEST
	QFile.open("log.txt", std::ios::app);
	QFile << "图像绘制完成" << "\n"; 
	QFile.close();
#endif

	//绘制后是处理
	//在绘制完以后，时窗口区有效
	ValidateRect(&windowsSize);
	//释放缓冲DC
	cacheDC.DeleteDC();
	//释放对象
	cacheBitmap.DeleteObject();
	//释放DC
	ReleaseDC(pdc);
	// 不要为绘制消息而调用 CWnd::OnPaint()

#ifndef TEST
	QFile.open("log.txt", std::ios::app);
	QFile << "释放完成" << "\n"; 
	QFile.close();
#endif
	
	//获取距离小鸟最近的水管坐标
	getNearest();

	//碰撞检测
	if(collisionDetect()) {
		//没有死亡则更新Q矩阵并自动游戏
		updateQ(true);
		autoPlay();
	}
	else {
		//死亡更新 Q 矩阵
		updateQ(false);
	}
}

void CChildView::transparentPNG(CImage *png)  
{  
	for(int i = 0; i <png->GetWidth(); i++)  
	{  
		for(int j = 0; j <png->GetHeight(); j++)  
		{  
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i , j));  
			pucColor[0] = pucColor[0] *pucColor[3] / 255;  
			pucColor[1] = pucColor[1] *pucColor[3] / 255;  
			pucColor[2] = pucColor[2] *pucColor[3] / 255;  
		}  
	}  
}  

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent) {
	case TIMER_PAINT:
		OnPaint();
		break;

	//case ON_WM_CLOSE:
	//	QFile.open("result.txt", std::ios::trunc);
	//	QFile << "close" << "\n"; 
	//	QFile.close();
	//	break;

	default:
		break;
	}
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ÔÚ´ËÌí¼ÓÄú×¨ÓÃµÄ´´½¨´úÂë
	QFile.open("log.txt", std::ios::trunc);
	QFile << "文本初始化" << "\n"; 
	QFile.close();

	index = 0;
	pipesManage = NULL;
	bird = Bird();
	isFirstDraw = true;
	isKeyUP = false;
	isHaveNearestPipe = false;
	nearestX = -1;
	nearestY = -1;
	prePipeX = -1;
	prePipeY = -1;

	//初始化Q矩阵和相关参数
	for(int i=0; i < 500; i++) {
		for(int j=0; j < 1000; j++) {
			qLearning[i][j][0] = 0;
			qLearning[i][j][1] = 0;
		}
	}

	argment = 0.7;
	reward = 1;
	failed = -100;

	currentAction = 0;
	previousAction = 0;
	currentPipeX = 0;
	currentPipeY = 0;
	prePipeX = 0;
	prePipeY = 0;

	//图像资源加载
	CString imagePath;
	imagePath = L".\\res\\sky.png";
	background.Load(imagePath);
	imagePath = L".\\res\\bird.png";
	birdImage.Load(imagePath);
	imagePath = L".\\res\\pipe.png";
	pipe.Load(imagePath);
	imagePath = L".\\res\\pipe-up.png";
	bottomPipe.Load(imagePath);
	imagePath = L".\\res\\pipe-down.png";
	topPipe.Load(imagePath);

	SetTimer(TIMER_PAINT, 1, NULL);

	return 0;
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	if(nChar == VK_SPACE) {
		bird.up();
		isKeyUP = true;
		OnPaint();
	}
	else if(nChar == VK_ESCAPE) {
		OnClose();
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::reset() {
	pipesManage->pipeClear();
	bird.reset();
	isFirstDraw = true;
}

void CChildView::getNearest() {
	//确定第一个为通过的水管坐标
	//std::vector<Pipe> pipes = pipesManage->getPipes();
	//初始化为没有，下面找到则设置为有
	isHaveNearestPipe = false;
	//找到与小鸟最近的水管坐标
	for(int i=0; i < pipes.size(); i++) {
		int pipePos = pipes[i].getPos();
		int topHeight = pipes[i].getTopHeight()+25;
		int bottomHeight = pipes[i].getBottomHeight()+25;

		if(pipePos >= 75 ) {
			nearestX = pipePos+25;
			nearestY = topHeight+125;
			isHaveNearestPipe = true;
			break;
		}
	}
}

bool CChildView::collisionDetect() {
	//如果鸟在最下方和最上面则死亡
	if(bird.getPos() <= 0) {
#ifndef TEST
		QFile.open("log.txt", std::ios::app);
		QFile << "小鸟撞到天上死亡" << bird.getPos() << "\n"; 
		QFile.close();
#endif

		reset();
		return false;
	}
	else if(bird.getPos() >= windowsSize.Height()) {
#ifndef TEST
		QFile.open("log.txt", std::ios::app);
		QFile << "小鸟撞到地上死亡" << bird.getPos() << "\n"; 
		QFile.close();
#endif

		reset();
		return false;
	}
	else {
		//分别检测每跟水管
		for(int i=0; i < pipes.size(); i++) {
			int pipePos = pipes[i].getPos();
			int topHeight = pipes[i].getTopHeight()+25;
			int bottomHeight = pipes[i].getBottomHeight()+25;

			//首先水管位置与鸟接触
			if(pipePos >= 50 && pipePos <= 75) {
				//如果与上方水管想撞
				if(bird.getPos() <= topHeight) {
#ifndef TEST
					QFile.open("log.txt", std::ios::app);
					QFile << "小鸟跟上方水管相撞死亡" << bird.getPos() << "<=" << topHeight 
						<< ":" << currentPipeX << "," << currentPipeY << "\n"; 
					QFile.close();
#endif

					reset();
					return false;
				}
				//如果与下方水管相撞
				else if(bird.getPos() >= windowsSize.Height()-bottomHeight) {
#ifndef TEST
					QFile.open("log.txt", std::ios::app);
					QFile << "小鸟跟下方水管相撞死亡" << bird.getPos() << ">=" << windowsSize.Height()-bottomHeight 
						<< ":" << currentPipeX << "," << currentPipeY << "\n"; 
					QFile.close();
#endif

					reset();
					return false;
				}
			}
		}
	}
	return true;
}

void CChildView::autoPlay() {
	//水管的移动
	pipesManage->manage();

	//获取当前的相对坐标（状态）
	currentPipeX = nearestX - 75 + 100;
	currentPipeY = nearestY - bird.getPos() + 600;
	
	if(currentPipeX < 0 || currentPipeX >= 500) {
#ifndef TEST
		QFile.open("log.txt", std::ios::app);
		QFile << "X 数组越界" << currentPipeX << "\n"; 
		QFile.close();
#endif
	}
	if(currentPipeY < 0 || currentPipeY >= 1000) {
#ifndef TEST
		QFile.open("log.txt", std::ios::app);
		QFile << "Y 数组越界" << currentPipeY << "\n"; 
		QFile.close();
#endif
	}

	//获取当前状态向下一个状态转换的各个动作相应的期望值
	float jupmValue = qLearning[currentPipeX][currentPipeY][0];
	float waityValue = qLearning[currentPipeX][currentPipeY][1];

	//将当前坐标状态进行保存
	prePipeX = currentPipeX;
	prePipeY = currentPipeY;

	//选择接下来的动作
	if(jupmValue <= waityValue) {
		bird.down();
		previousAction = 1;
	}
	else if(jupmValue > waityValue) {
		bird.up();
		previousAction = 0;
		isKeyUP = true;
	}
} 

void CChildView::updateQ(bool isLive) {
	currentPipeX = nearestX - 75 + 100;
	currentPipeY = nearestY - bird.getPos()+600;

	if(currentPipeX < 0 || currentPipeX >= 500) {
#ifndef TEST
		QFile.open("log.txt", std::ios::app);
		QFile << "X 数组越界" << currentPipeX << "\n"; 
		QFile.close();
#endif
	}
	if(currentPipeY < 0 || currentPipeY >= 1000) {
#ifndef TEST
		QFile.open("log.txt", std::ios::app);
		QFile << "Y 数组越界" << currentPipeY << "\n"; 
		QFile.close();
#endif
	}

	if(isLive) {
		//小鸟存活了下来
		float previousValue = qLearning[prePipeX][prePipeY][previousAction];
		//选择当前所有状态的最大值
		float currentValue;
		if(qLearning[currentPipeX][currentPipeY][0] > qLearning[currentPipeX][currentPipeY][1]) {
			currentValue = qLearning[currentPipeX][currentPipeY][0];
		}
		else {
			currentValue = qLearning[currentPipeX][currentPipeY][1];
		}
		//更新Q居阵的值
		qLearning[prePipeX][prePipeY][previousAction] = (1-argment)* previousValue
			+ argment * (reward + currentValue);

#ifndef TEST
		QFile.open("QArray.txt", std::ios::app);
		QFile << "Live: " << prePipeX << "," << prePipeY << "=" 
			<< qLearning[prePipeX][prePipeY][0] << ","
			<< qLearning[prePipeX][prePipeY][1] << "\n";
		QFile.close();
#endif
	}
	else {
		//小鸟死亡
		float previousValue = qLearning[prePipeX][prePipeY][previousAction];
		//选择当前所有状态的最大值
		float currentValue;
		if(qLearning[currentPipeX][currentPipeY][0] > qLearning[currentPipeX][currentPipeY][1]) {
			currentValue = qLearning[currentPipeX][currentPipeY][0];
		}
		else {
			currentValue = qLearning[currentPipeX][currentPipeY][1];
		}
		//更新Q居阵的值
		qLearning[prePipeX][prePipeY][previousAction] = (1-argment)* previousValue
			+ argment * (failed + currentValue);

#ifndef TEST
		QFile.open("QArray.txt", std::ios::app);
		QFile << "Deaded：" << prePipeX << "," << prePipeY << "=" 
			<< qLearning[prePipeX][prePipeY][0] << ","
			<< qLearning[prePipeX][prePipeY][1] << "\n";
		QFile.close();
#endif
	}
}


void CChildView::OnClose()
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	QFile.open("result.txt", std::ios::trunc);

	//存储Q矩阵
	for(int i=0; i < 500; i++) {
		for(int j=0; j < 1000; j++) {
			QFile << qLearning[i][j][0] << " " << qLearning[i][j][1];
		}
		QFile << "\n";
	}

	QFile.close();

	CWnd::OnClose();
}
