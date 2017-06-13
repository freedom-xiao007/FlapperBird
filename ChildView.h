
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

#include "PipeManage.h"
#include "Bird.h"
#include <fstream>


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
	bool isFirstDraw;//�Ƿ��ǵ�һ�λ���
	bool isKeyUP;//�жϻ���֮ǰ�Ƿ񰴽�

	bool collisionDetect(CRect bird, std::vector<Pipe> pipes);
	void reset();

	//Q����
	float qLearning[400][600][2];//�ֱ���������ˮ�ܵĴ�ֱ���롢ˮƽ��������ֶ���
	float argment;//ѧϰ����
	int reward , failed;//�����ͳͷ�ֵ
	int currentAction, previousAction;//���ں͹�ȥ��ȡ���ж�
	int currentPipeX, currentPipeY;//�����ˮ�����꣨û��ͨ���ģ�
	int prePipeX, prePipeY;//��ȥ�������ˮ�����꣨û��ͨ���ģ�
	std::ofstream QFile;
	int index;

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

