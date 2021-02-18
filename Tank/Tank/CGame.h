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

	void SetHandle(HWND hWnd);                             // ����������ڵľ��

	bool EnterFrame(DWORD dwTime);                         // ������Ϸ֡

	void OnMouseMove(UINT nFlags, CPoint point);           // ��������ƶ��¼�

	void OnLButtonUp(UINT nFlags, CPoint point);           // �������̧���¼�

private:
	HWND m_hWnd;                                           // ����

	// ��Ϸ��ͼ����
	// ����滭��Ϸ�еĶ���
	void GameRunDraw();

	// ���fps
	void DrawFps(Graphics &gh);

	// ��¼��Ϸÿ�����֡
	int m_fps{ 0 };
};

