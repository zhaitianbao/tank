#pragma once

#include "CGameMenuBackground.h"
#include "CGameMenuPanel.h"

#include <windef.h>
#include <iostream>
#include <Afxwin.h>

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;

class CGame
{
public:
	CGame();

	~CGame();

	void SetHandle(HWND hWnd);                             // ����������ڵľ��

	bool EnterFrame(DWORD dwTime);                         // ������Ϸ֡

	void OnMouseMove(UINT nFlags, CPoint point);           // ��������ƶ��¼�

	void OnLButtonUp(UINT nFlags, CPoint point);           // �������̧���¼�

private:
	HWND m_hWnd;                                           // ����

	/* ��Ϸ��ͼ����
	   �����ͼ��Ϸ�еĶ���
	*/
	void GameRunDraw();

	CGameMenuBackground m_menu;

	CGameMenuPanel m_menuSelect;
};

