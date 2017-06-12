
// ChildView.h : CChildView 类的接口
//


#pragma once

#include "PipeManage.h"
#include "Bird.h"


// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	CImage birdImage;
	CImage background;
	CImage topPipe, bottomPipe, pipe;
	CRect windowsSize;
	CRect birdPos;
	CDC cacheDC; //缓冲DC
	CBitmap cacheBitmap; //缓冲位图
	PipeManage *pipesManage;
	Bird bird;
	bool isKeyUP;//判断绘制之前是否按建

// 操作
public:
	void transparentPNG(CImage *png);  

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

