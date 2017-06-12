
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

#include "PipeManage.h"
#include "Bird.h"


// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	CImage birdImage;
	CImage background;
	CImage topPipe, bottomPipe, pipe;
	CRect windowsSize;
	CRect birdPos;
	CDC cacheDC; //����DC
	CBitmap cacheBitmap; //����λͼ
	PipeManage *pipesManage;
	Bird bird;
	bool isKeyUP;//�жϻ���֮ǰ�Ƿ񰴽�

// ����
public:
	void transparentPNG(CImage *png);  

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

