
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
		bird.down();
		pipesManage->manage();//水管的管理（第一次生成和移动）
	}
	isKeyUP = false;

	//像素鸟的绘制
	birdImage.Draw(cacheDC, 50, bird.getPos(), 25, 25);
	
	//水管的绘制
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

	//碰撞检测
	//如果鸟在最下方和最上面则死亡
	if(bird.getPos() <= 0) {
		reset();
	}
	else if(bird.getPos() >= windowsSize.Height()) {
		reset();
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
					break;
				}
				//如果与下方水管相撞
				else if(bird.getPos() >= windowsSize.Height()-bottomHeight) {
					reset();
					break;
				}
			}
		}
	}

	//在绘制完以后，时窗口区有效
	ValidateRect(&windowsSize);
	//释放缓冲DC
	cacheDC.DeleteDC();
	//释放对象
	cacheBitmap.DeleteObject();
	//释放DC
	ReleaseDC(pdc);
	// 不要为绘制消息而调用 CWnd::OnPaint()
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
	pipesManage = NULL;
	bird = Bird();
	isKeyUP = false;

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


	SetTimer(TIMER_PAINT, 100, NULL);

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
}
