﻿
// Tank.h: Tank 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;

// CTankApp:
// 有关此类的实现，请参阅 Tank.cpp
//

class CTankApp : public CWinApp
{
public:
	CTankApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	// 引入GDIPlus所需要的变量
	ULONG_PTR m_tokenGdiplus;
	GdiplusStartupInput input;
	GdiplusStartupOutput output;
};

extern CTankApp theApp;
