#include "pch.h"
#include "CGame.h"

CGame::CGame()
{
	GameInitOne2Bot();
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
	GameRunLogic();
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
	/*	m_menu.Draw(gh);
		m_menuSelect.Draw(gh);*/
		m_map.Draw(gh);
		// 画坦克(玩家一)
		m_player01.Draw(gh);
		// 画子弹(所有存在于地图上的坦克)
		for (auto &blt : m_listBullets) {
			blt.Draw(gh);
		}
	}

	// 复制到屏幕
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();
	return;
}

void CGame::GameRunLogic()
{
#define KEYDOWN(vk) (GetAsyncKeyState(vk)&0x8000)
	// 按键处理
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
			// 碰撞检测
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
			// 碰撞检测
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

	// 移除超时的子弹
	{
		// 查找超时的子弹
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

		// 从本地删除子弹
		m_listBullets.erase(itRemove, m_listBullets.end());
	}

	// 子弹撞墙处理
	{
		for (auto &blt : m_listBullets)
		{
			m_map.HitWallProcess(blt);
			blt.Move();
		}
	}

	//检查坦克爆炸
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
				AfxMessageBox(_T("调整player01位置失败"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);
			}
		}
		m_listBullets.clear();

	}
	return true;
}