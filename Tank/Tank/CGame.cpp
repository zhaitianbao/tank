#include "pch.h"
#include "CGame.h"
#define KEYDOWN(vk) (GetAsyncKeyState(vk)&0x8000)

CGame::CGame()
{
	m_menuSelect.m_pParent = this;
	m_menuBackup.m_pParent = this;
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
	GameRunLogic();
	GameRunDraw();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)        // 处理鼠标移动事件
{
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnMouseMove(nFlags, point);
	}
	else {
		m_menuBackup.OnMouseMove(nFlags, point);
	}
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)        // 处理左键抬起事件
{
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else {
		m_menuBackup.OnLButtonUp(nFlags, point);
	}
	
}

// 设置当前游戏阶段
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();
}

/* 游戏初始化
   生成游戏对象，初始化地图和对象未知
*/
bool CGame::GameInit()
{
	srand(GetTickCount());
	return (this->*m_initFunc[m_eStep])();
}

bool CGame::GameInitMenu()
{
	return true;
}

bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);
	return true;
}

bool CGame::GameInitOne2Bot()
{
	for (;;)
	{
		m_map.LoadMap();
		// 玩家一
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			m_player01.SetSpeed(5);
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整player01位置失败！"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);
			}
		}

		// 机器人
		{
			m_bot = CBot(0, 0, _T("tank_bot.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整Bot位置失败！"));
			}
			else {
				m_bot.SetCenterPoint(ptCenter);
			}
		}

		m_listBullets.clear();

		// 判断是否合法
		{
			// 获取机器人和玩家的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) || !m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("获取坦克位置失败！"));
				goto __Init_End;
			}

			// 判断位置是否连通
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty())
			{
				goto __Init_End;
			}
		}
	}
__Init_End:
	return true;
}

bool CGame::GameInitOne2BotEnd()
{
	return true;
}

bool CGame::GameInitOne2OneMenu()
{

	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width/ 2.0f+100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);
	m_keymenu01.SetStop(false);

	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu02.SetCenterPoint(pt);
	m_keymenu02.SetStop(false);

	return true;
}

bool CGame::GameInitOne2One()
{
	for (;;)
	{
		m_map.LoadMap();
		// 玩家一
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			m_player01.SetSpeed(5);
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整player01位置失败！"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);
			}
		}

		// 玩家一
		{
			m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
			m_player02.SetSpeed(5);
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("调整player02位置失败！"));
			}
			else {
				m_player02.SetCenterPoint(ptCenter);
			}
		}

		m_listBullets.clear();

		// 判断是否合法
		{
			// 获取玩家的位置
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) || !m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("获取坦克位置失败！"));
				break;
			}

			// 判断位置是否连通
			VPath path;
			m_map.FindPath(startX, startY, targetX, targetY, path);
			if (!path.empty())
			{
				break;
			}
		}
	}
	return true;
}

bool CGame::GameInitOne2OneEnd()
{
	return true;
}

/* 游戏逻辑处理
 * 1.维护子弹状态
 * 2.维护机器人
 * 3.维护玩家坦克的状态
 * 碰撞检测包括：撞墙，子弹命中坦克
**/
void CGame::GameRunLogic()
{
	// 根据不同的阶段调用不同处理方法
	(this->*m_logicFunc[m_eStep])();
}

void CGame::GameRunLogicOnMenu()
{

}

void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M'))
	{
		m_keymenu01.SetStop();
	}
	if (m_keymenu01.GetStop())
	{
		SetStep(EGameTypeOne2Bot);
	}
}

void CGame::GameRunLogicOnOne2Bot()
{
	// 状态维护
	// 移除无效子弹
	RemoveTimeoutBullets();

	// 检查子弹是否击中坦克
	for (auto &blt : m_listBullets)
	{
		if (m_bot.IsHitted(blt))
		{
			m_bot.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
			break;
		}
		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
			m_eStep = EGameTypeOne2BotEnd;
			blt.SetActive(false);
			break;
		}
	}

	ProcessHitBullets();

	AI();

	// 按键处理
	{
		if (KEYDOWN(VK_LEFT)) {				    // 左方向键按下
			m_player01.RotateLeft();			// 玩家一向左旋转
		}
		if (KEYDOWN(VK_RIGHT)) {				// 右方向键按下
			m_player01.RotateRight();			// 玩家一向右旋转
		}
		if (KEYDOWN(VK_UP)) {					// 上方向键按下
			// 坦克撞墙测试	
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// 如果撞墙了
					m_player01.ChangeDirection(true);			// 改变坦克方向
				}
				else {
					m_player01.Forward();						// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// 下方向键按下
				// 坦克撞墙检测试
			{
				if (m_map.IsHitTheWall(m_player01, false)) {	// 如果撞墙了
					m_player01.ChangeDirection(true);		  	// 改变坦克方向
				}
				else {
					m_player01.Backward();				  	// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN('M')) {									// M键按下
			CBullet blt;
			if (m_player01.Fire(blt)) {						// 发射子弹
				m_listBullets.push_back(blt);				// 加入到地图列表中
			}
		}
		
	}
}

void CGame::GameRunLogicOnOne2BotEnd()
{
	// 按键处理
	// 不再接受按键


	// 状态维护
	// 移除无效子弹
	RemoveTimeoutBullets();

	// 子弹维护
	ProcessHitBullets();

}

void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M'))
	{
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q'))
	{
		m_keymenu02.SetStop();
	}
	if (m_keymenu01.GetStop()&& m_keymenu02.GetStop())
	{
		SetStep(EGameTypeOne2One);
	}
}

void CGame::GameRunLogicOnOne2One()
{
	// 按键处理
	{
		if (KEYDOWN(VK_LEFT)) {				    // 左方向键按下
			m_player01.RotateLeft();			// 玩家一向左旋转
		}
		if (KEYDOWN(VK_RIGHT)) {				// 右方向键按下
			m_player01.RotateRight();			// 玩家一向右旋转
		}
		if (KEYDOWN(VK_UP)) {					// 上方向键按下
			// 坦克撞墙测试	
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// 如果撞墙了
					m_player01.ChangeDirection(true);			// 改变坦克方向
				}
				else {
					m_player01.Forward();						// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// 下方向键按下
				// 坦克撞墙检测试
			{
				if (m_map.IsHitTheWall(m_player01, false)) {	// 如果撞墙了
					m_player01.ChangeDirection(true);		  	// 改变坦克方向
				}
				else {
					m_player01.Backward();				  	// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN('M')) {									// M键按下
			CBullet blt;
			if (m_player01.Fire(blt)) {						// 发射子弹
				m_listBullets.push_back(blt);				// 加入到地图列表中
			}
		}
	}

	// 按键处理
	{
		if (KEYDOWN('A')) {				    // 左方向键按下
			m_player02.RotateLeft();			// 玩家一向左旋转
		}
		if (KEYDOWN('D')) {				// 右方向键按下
			m_player02.RotateRight();			// 玩家一向右旋转
		}
		if (KEYDOWN('W')) {					// 上方向键按下
			// 坦克撞墙测试	
			{
				if (m_map.IsHitTheWall(m_player02, true)) {		// 如果撞墙了
					m_player02.ChangeDirection(true);			// 改变坦克方向
				}
				else {
					m_player02.Forward();						// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN('S')) {								// 下方向键按下
				// 坦克撞墙检测试
			{
				if (m_map.IsHitTheWall(m_player02, false)) {	// 如果撞墙了
					m_player02.ChangeDirection(true);		  	// 改变坦克方向
				}
				else {
					m_player02.Backward();				  	// 没撞墙继续向前进
				}
			}
		}
		if (KEYDOWN('Q')) {									// M键按下
			CBullet blt;
			if (m_player02.Fire(blt)) {						// 发射子弹
				m_listBullets.push_back(blt);				// 加入到地图列表中
			}
		}
	}

	RemoveTimeoutBullets();

	// 检查子弹是否击中坦克
	for (auto &blt : m_listBullets)
	{
		if (!blt.IsActive())
		{
			continue;
		}
		if (m_player02.IsHitted(blt))
		{
			m_player02.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
			break;
		}
		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
			break;
		}
	}

	ProcessHitBullets();
}

void CGame::GameRunLogicOnOne2OneEnd()
{
	// 按键处理
	// 不再接受按键


	// 状态维护
	// 移除无效子弹
	RemoveTimeoutBullets();

	// 子弹维护
	ProcessHitBullets();
}

/* 游戏绘图处理
   负责绘图游戏中的对象
**/
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

	(this->*m_drawFunc[m_eStep])(gh);

	// 复制到屏幕
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd,hdc);
	return;
}

void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.Draw(gh);
}

void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.Draw(gh);
	m_menuBackup.Draw(gh);
	m_keymenu01.Draw(gh);
}

void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_bot.Draw(gh);

	for (auto b : m_listBullets)
	{
		b.Draw(gh);
	}

	DrawFps(gh);
}

void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_bot.Draw(gh);

	for (auto b : m_listBullets)
	{
		b.Draw(gh);
	}

	DrawFps(gh);

	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.Draw(gh);
	m_menuBackup.Draw(gh);
	m_keymenu01.Draw(gh);
	m_keymenu02.Draw(gh);
}

void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_player02.Draw(gh);

	for (auto b : m_listBullets)
	{
		b.Draw(gh);
	}

	DrawFps(gh);
}

void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);
	m_map.Draw(gh);

	m_player01.Draw(gh);
	m_player02.Draw(gh);

	for (auto b : m_listBullets)
	{
		b.Draw(gh);
	}


	DrawFps(gh);

	if (m_player01.IsBombEnd() || m_player02.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

/* 辅助方法
*/
void CGame::RemoveTimeoutBullets()
{
	// 定义查找方法
	auto itRemove = std::remove_if(m_listBullets.begin(), m_listBullets.end(), []
	(CBullet &blt)->bool {return blt.IsTimeout();
	});

	for (auto it = itRemove; it != m_listBullets.end(); it++)
	{
		it->SetActive(false);
		it->GetOwner()->AddBullet(*it);
	}

	m_listBullets.erase(itRemove, m_listBullets.end());
}

void CGame::ProcessHitBullets()
{
	for (auto &blt : m_listBullets)
	{
		m_map.HitWallProcess(blt);
		blt.Move();
	}
}

void CGame::AI()
{
	static CGameTimer acTimer(-1, 150);
	if (acTimer.IsTimeval())
	{
		int startX, startY, targetX, targetY;
		if (!m_map.FindObjPosition(m_bot, startX, startY) ||
			!m_map.FindObjPosition(m_player01, targetX, targetY)) {
			return;
		}
		float fDirNext = 0; //机器人下一步的方向
		if (!m_map.FindNextDirection(&fDirNext, startX, startY,
			targetX, targetY)) {
			return;
		}
		//获取机器人坦克的中心点
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

		if (!rc.Contains(ptTankCenter)) { 		// 判断 坦克是否己经走到了中心点位置了
			m_bot.Forward(); 					// 没有到达中心点，继续前进
			return;
		}
		else {
			m_bot.SetDirection(fDirNext);		// 设置机器人的方向		
			float dir;
			// 判断是否可以打中玩家
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt))
				{
					m_listBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();					// 机器人前进
		}
	}

}

void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;
	m_fps++;
	static DWORD dwLast = GetTickCount();
	if (GetTickCount() - dwLast >= 1000)
	{
		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();
	}

	// 输出FPS
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);
		SolidBrush brush(Color(0x00, 0x00, 0xFF));
		Gdiplus::Font font(_T("宋体"), 10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}