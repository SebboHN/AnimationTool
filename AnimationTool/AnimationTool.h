
// AnimationTool.h : main header file for the AnimationTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CAnimationToolApp:
// See AnimationTool.cpp for the implementation of this class
//

class CAnimationToolApp : public CWinAppEx
{
public:
	CAnimationToolApp() noexcept;
	static CDocument* myWindowDoc;
	static CDocument* GenericGetActiveDocument();
	CWinThread* RenderLoop;
	CWinThread* ResizeLoop;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAnimationToolApp theApp;
