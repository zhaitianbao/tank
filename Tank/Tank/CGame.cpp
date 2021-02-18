#include "pch.h"
#include "CGame.h"

CGame::CGame()
{
}


CGame::~CGame()
{
}

void CGame::SetHandle(HWND hWnd)                          // ����������ڵľ��
{
	m_hWnd = hWnd;
}

bool CGame::EnterFrame(DWORD dwTime)                      // ������Ϸ֡
{
	GameRunDraw();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)        // ��������ƶ��¼�
{
	m_menuSelect.OnMouseMove(nFlags, point);
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)        // �������̧���¼�
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

	// �����ڴ�
	{
		m_menu.Draw(gh);
		m_menuSelect.Draw(gh);
	}

	// ���Ƶ���Ļ
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();
	return;
}