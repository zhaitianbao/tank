#pragma once

#include <windef.h>
#include <iostream>
#include <Afxwin.h>

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;

class CGame
{
public:
	CGame() {};

	~CGame() {};

	void SetHandle(HWND hWnd);                             // 设置输出窗口的句柄

	bool EnterFrame(DWORD dwTime);                         // 进入游戏帧

	void OnMouseMove(UINT nFlags, CPoint point);           // 处理鼠标移动事件

	void OnLButtonUp(UINT nFlags, CPoint point);           // 处理左键抬起事件

private:
	HWND m_hWnd;                                           // 窗口

	// 游戏绘图处理
	// 负责绘画游戏中的对象
	void GameRunDraw();

	// 输出fps
	void DrawFps(Graphics &gh);

	// 记录游戏每秒多少帧
	int m_fps{ 0 };
};

