
// FlappyBird_MFC.h : FlappyBird_MFC Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFlappyBird_MFCApp:
// �йش����ʵ�֣������ FlappyBird_MFC.cpp
//

class CFlappyBird_MFCApp : public CWinAppEx
{
public:
	CFlappyBird_MFCApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFlappyBird_MFCApp theApp;
