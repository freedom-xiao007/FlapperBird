
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "FlappyBird_MFC.h"
#include "ChildView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	CDC *pdc = this->GetDC();
	GetClientRect(windowsSize);

	if(pipesManage == NULL) {
		pipesManage = new PipeManage(windowsSize.Height(), windowsSize.Width());
	}

	//创建缓冲DC
	cacheDC.CreateCompatibleDC(NULL);
	cacheBitmap.CreateCompatibleBitmap(pdc, windowsSize.Width(), windowsSize.Height());
	cacheDC.SelectObject(&cacheBitmap);

	background.Draw(cacheDC, windowsSize);

	if(!isKeyUP) {
		if(!isFirstDraw) {
			bird.down();
		}
	}
	isKeyUP = false;

	//像素鸟的绘制
	birdImage.Draw(cacheDC, 50, bird.getPos(), 25, 25);
	
	//水管的绘制
	pipesManage->manage();//水管的管理（第一次生成和移动）
	std::vector<Pipe> pipes = pipesManage->getPipes(); 
	for(int i=0; i < pipes.size(); i++) {
		int pipePos = pipes[i].getPos();
		int topHeight = pipes[i].getTopHeight();
		int bottomHeight = pipes[i].getBottomHeight();

		pipe.Draw(cacheDC, pipePos, 0, 25, topHeight);
		topPipe.Draw(cacheDC, pipePos, topHeight, 25, 25);
		bottomPipe.Draw(cacheDC, pipePos, topHeight+25+75, 25, 25);
		pipe.Draw(cacheDC, pipePos, topHeight+25+75+25, 25, bottomHeight);
	}

	pdc->BitBlt(0, 0, windowsSize.Width(), windowsSize.Height(), &cacheDC, 0, 0, SRCCOPY);

	//在绘制完以后，时窗口区有效
	ValidateRect(&windowsSize);
	//释放缓冲DC
	cacheDC.DeleteDC();
	//释放对象
	cacheBitmap.DeleteObject();
	//释放DC
	ReleaseDC(pdc);
	// 不要为绘制消息而调用 CWnd::OnPaint()
	
	//确定第一个为通过的水管坐标
	bool havePipe = true;
	int x = -1;
	int y = -1;
	if(pipes.empty()) {
		havePipe = false;
	}
	else {
		//找到与小鸟最近的水管坐标
		for(int i=0; i < pipes.size(); i++) {
			int pipePos = pipes[i].getPos();
			int topHeight = pipes[i].getTopHeight()+25;
			int bottomHeight = pipes[i].getBottomHeight()+25;

			if(pipePos >= 75 ) {
				x = pipePos+25;
				y = topHeight+125;
				break;
			}
		}
	}

	//如果是第一次绘制则随机选择下一次动作
	if(isFirstDraw) {
		isFirstDraw = false;
	
		//保存坐标状态
		if(havePipe) {
			prePipeX = x - 75;
			prePipeY = y - bird.getPos() + 300;
		}

		//等于0，就跳，否则什么都不做
		//if(rand()%2 == 1) {
		//	bird.up();//等于0就跳
		//	previousAction = 0;
		//}
		//else {
		//	previousAction = 1;
		//}

		//选择接下来的动作
		float jupmValue = qLearning[prePipeX][prePipeY][0];
		float waityValue = qLearning[prePipeX][prePipeY][1];

		if(jupmValue == waityValue) {
			//if(rand()%2 == 1) {
			//	bird.up();
			//	previousAction = 0;
			//}
			//else {
			//	previousAction = 1;
			//}
			previousAction = 1;
		}
		else if(jupmValue > waityValue) {
			bird.up();
			previousAction = 0;
			isKeyUP = true;
		}
		else {
			previousAction = 1;
		}

		index = 0;
		QFile.open("QArray.txt", std::ios::app);
		QFile << prePipeX << "," << prePipeY << "="
			<< jupmValue << "," << waityValue << "\n";
		QFile.close();
		++index;

		return;
	}
	//如果不是，需要碰撞检测，没有碰撞则选择下一次行动
	else {
		//碰撞检测
		//如果鸟在最下方和最上面则死亡
		if(bird.getPos() <= 0) {
			reset();

			//更新Q矩阵
			currentPipeX = x - 75;
			currentPipeY = y - bird.getPos()+300;
	
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

			QFile.open("QArray.txt", std::ios::app);
			QFile << prePipeX << "," << prePipeY << "=" 
				<< qLearning[prePipeX][prePipeY][0] << ","
				<< qLearning[prePipeX][prePipeY][1] << "\n";
			QFile.close();
			
			return;
		}
		else if(bird.getPos() >= windowsSize.Height()) {
			reset();

			//更新Q矩阵
			currentPipeX = x - 75;
			currentPipeY = y - bird.getPos()+300;
	
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

			QFile.open("QArray.txt", std::ios::app);
			QFile << prePipeX << "," << prePipeY << "=" 
				<< qLearning[prePipeX][prePipeY][0] << ","
				<< qLearning[prePipeX][prePipeY][1] << "\n";
			QFile.close();
			
			return;
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
						reset();

						//更新Q矩阵
						currentPipeX = x - 75;
						currentPipeY = y - bird.getPos()+300;
			
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

						QFile.open("QArray.txt", std::ios::app);
						QFile << prePipeX << "," << prePipeY << "=" 
							<< qLearning[prePipeX][prePipeY][0] << ","
							<< qLearning[prePipeX][prePipeY][1] << "\n";
						QFile.close();
						
						return;
						break;
					}
					//如果与下方水管相撞
					else if(bird.getPos() >= windowsSize.Height()-bottomHeight) {
						reset();

						//更新Q矩阵
						currentPipeX = x - 75;
						currentPipeY = y - bird.getPos()+300;
				
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

						QFile.open("QArray.txt", std::ios::app);
						QFile << prePipeX << "," << prePipeY << "=" 
							<< qLearning[prePipeX][prePipeY][0] << ","
							<< qLearning[prePipeX][prePipeY][1] << "\n";
						QFile.close();
						
						return;
						break;
					}
				}
			}

			//小鸟存活了下来
			//更新Q矩阵
			currentPipeX = x - 75;
			currentPipeY = y - bird.getPos()+300;

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

			QFile.open("QArray.txt", std::ios::app);
			QFile << prePipeX << "," << prePipeY << "=" 
				<< qLearning[prePipeX][prePipeY][0] << ","
				<< qLearning[prePipeX][prePipeY][1] << "\n";
			QFile.close();
			
		}
	}

	
	//如果存活下来，选择接下来的动作
	float jupmValue = qLearning[currentPipeX][currentPipeY][0];
	float waityValue = qLearning[currentPipeX][currentPipeY][1];
	prePipeX = currentPipeX;
	prePipeY = currentPipeY;

	if(jupmValue <= waityValue) {
		//if(rand()%2 == 1) {
		//	bird.up();
		//	previousAction = 0;
		//}
		//else {
		//	previousAction = 1;
		//}
		previousAction = 1;
	}
	else if(jupmValue > waityValue) {
		bird.up();
		previousAction = 0;
		isKeyUP = true;
	}


	QFile.open("QArray.txt", std::ios::app);
	QFile << currentPipeX << "," << currentPipeY << "=" 
		<< qLearning[currentPipeX][currentPipeY][0] << ","
		<< qLearning[currentPipeX][currentPipeY][1] << "\n";
	QFile.close();
	++index;
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

	default:
		break;
	}
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ÔÚ´ËÌí¼ÓÄú×¨ÓÃµÄ´´½¨´úÂë
	index = 0;
	pipesManage = NULL;
	bird = Bird();
	isFirstDraw = true;
	isKeyUP = false;

	//初始化Q矩阵和相关参数
	for(int i=0; i < 400; i++) {
		for(int j=0; j < 600; j++) {
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


	SetTimer(TIMER_PAINT, 50, NULL);

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

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::reset() {
	pipesManage->pipeClear();
	bird.reset();
	isFirstDraw = true;
}
