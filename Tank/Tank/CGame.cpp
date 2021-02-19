#include "pch.h"
#include "CGame.h"

CGame::CGame()
{
	GameInitOne2Bot();
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
	GameRunLogic();
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
	/*	m_menu.Draw(gh);
		m_menuSelect.Draw(gh);*/
		m_map.Draw(gh);
		// ��̹��(���һ)
		m_player01.Draw(gh);
		// ���ӵ�(���д����ڵ�ͼ�ϵ�̹��)
		for (auto &blt : m_listBullets) {
			blt.Draw(gh);
		}
	}

	// ���Ƶ���Ļ
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();
	return;
}

void CGame::GameRunLogic()
{
#define KEYDOWN(vk) (GetAsyncKeyState(vk)&0x8000)
	// ��������
	{
		if (KEYDOWN(VK_LEFT))
		{
			m_player01.RotateLeft();
		}
		if (KEYDOWN(VK_RIGHT))
		{
			m_player01.RotateRight();
		}
		if (KEYDOWN(VK_UP))
		{
			// ��ײ���
			{
				if (m_map.IsHitTheWall(m_player01, true))
				{
					m_player01.ChangeDirection(true);
				}
				else {
					m_player01.Forward();
				}
			}	
		}
		if (KEYDOWN(VK_DOWN))
		{
			// ��ײ���
			{
				if (m_map.IsHitTheWall(m_player01, false))
				{
					m_player01.ChangeDirection(true);
				}
				else {
					m_player01.Backward();
				}
			}
		}
		if (KEYDOWN('M'))
		{
			CBullet blt;
			if (m_player01.Fire(blt))
			{
				m_listBullets.push_back(blt);
			}
		}
	}

	for (auto &blt : m_listBullets)
	{
		blt.Move();
	}

	// �Ƴ���ʱ���ӵ�
	{
		// ���ҳ�ʱ���ӵ�
		auto itRemove = std::remove_if(m_listBullets.begin(),
			m_listBullets.end(),
			[](CBullet &blt)->bool {
			return blt.IsTimeout();
		});
		for (auto it = itRemove; it!= m_listBullets.end(); ++it)
		{
			it->SetActive(false);
			it->GetOwner()->AddBullet(*it);
		}

		// �ӱ���ɾ���ӵ�
		m_listBullets.erase(itRemove, m_listBullets.end());
	}

	// �ӵ�ײǽ����
	{
		for (auto &blt : m_listBullets)
		{
			m_map.HitWallProcess(blt);
			blt.Move();
		}
	}

	//���̹�˱�ը
	for (auto &blt : m_listBullets)
	{
		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
		}
		break;
	}
}

bool CGame::GameInitOne2Bot()
{
	{
		m_map.LoadMap();
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player01λ��ʧ��"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		m_listBullets.clear();

	}
	return true;
}