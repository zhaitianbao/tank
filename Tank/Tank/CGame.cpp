#include "pch.h"
#include "CGame.h"

CGame::CGame()
{
}


CGame::~CGame()
{
}

void CGame::SetHandle(HWND hWnd)                          // 设置输出窗口的句柄
{
	m_hWnd = hWnd;
}

bool CGame::EnterFrame(DWORD dwTime)                      // 进入游戏帧
{
	GameRunDraw();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)        // 处理鼠标移动事件
{
	m_menuSelect.OnMouseMove(nFlags, point);
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)        // 处理左键抬起事件
{
	m_menuSelect.OnLButtonUp(nFlags, point);
}

void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);
	CRect rc;
	GetClientRect(m_hWnd, &rc);

	CDC *dc = CClientDC::FromHandle(hdc);

	CDC m_dcMemory;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc());
	gh.Clear(Color::White);
	gh.ResetClip();

	// 画入内存
	{
		m_menu.Draw(gh);
		m_menuSelect.Draw(gh);
	}

	// 复制到屏幕
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();
	return;
}