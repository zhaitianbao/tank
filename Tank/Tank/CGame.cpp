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

void CGame::SetHandle(HWND hWnd)                          // ����������ڵľ��
{
	m_hWnd = hWnd;
}

bool CGame::EnterFrame(DWORD dwTime)                      // ������Ϸ֡
{
	GameRunLogic();
	GameRunDraw();
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)        // ��������ƶ��¼�
{
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnMouseMove(nFlags, point);
	}
	else {
		m_menuBackup.OnMouseMove(nFlags, point);
	}
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)        // �������̧���¼�
{
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnLButtonUp(nFlags, point);
	}
	else {
		m_menuBackup.OnLButtonUp(nFlags, point);
	}
	
}

// ���õ�ǰ��Ϸ�׶�
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();
}

/* ��Ϸ��ʼ��
   ������Ϸ���󣬳�ʼ����ͼ�Ͷ���δ֪
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
		// ���һ
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			m_player01.SetSpeed(5);
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player01λ��ʧ�ܣ�"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);
			}
		}

		// ������
		{
			m_bot = CBot(0, 0, _T("tank_bot.png"));
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����Botλ��ʧ�ܣ�"));
			}
			else {
				m_bot.SetCenterPoint(ptCenter);
			}
		}

		m_listBullets.clear();

		// �ж��Ƿ�Ϸ�
		{
			// ��ȡ�����˺���ҵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_bot, startX, startY) || !m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("��ȡ̹��λ��ʧ�ܣ�"));
				goto __Init_End;
			}

			// �ж�λ���Ƿ���ͨ
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
		// ���һ
		{
			m_player01 = CPlayer(0, 0, _T("tank_player1.png"));
			m_player01.SetSpeed(5);
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player01λ��ʧ�ܣ�"));
			}
			else {
				m_player01.SetCenterPoint(ptCenter);
			}
		}

		// ���һ
		{
			m_player02 = CPlayer(0, 0, _T("tank_player2.png"));
			m_player02.SetSpeed(5);
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(_T("����player02λ��ʧ�ܣ�"));
			}
			else {
				m_player02.SetCenterPoint(ptCenter);
			}
		}

		m_listBullets.clear();

		// �ж��Ƿ�Ϸ�
		{
			// ��ȡ��ҵ�λ��
			int startX, startY, targetX, targetY;
			if (!m_map.FindObjPosition(m_player02, startX, startY) || !m_map.FindObjPosition(m_player01, targetX, targetY))
			{
				AfxMessageBox(_T("��ȡ̹��λ��ʧ�ܣ�"));
				break;
			}

			// �ж�λ���Ƿ���ͨ
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

/* ��Ϸ�߼�����
 * 1.ά���ӵ�״̬
 * 2.ά��������
 * 3.ά�����̹�˵�״̬
 * ��ײ��������ײǽ���ӵ�����̹��
**/
void CGame::GameRunLogic()
{
	// ���ݲ�ͬ�Ľ׶ε��ò�ͬ������
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
	// ״̬ά��
	// �Ƴ���Ч�ӵ�
	RemoveTimeoutBullets();

	// ����ӵ��Ƿ����̹��
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

	// ��������
	{
		if (KEYDOWN(VK_LEFT)) {				    // ���������
			m_player01.RotateLeft();			// ���һ������ת
		}
		if (KEYDOWN(VK_RIGHT)) {				// �ҷ��������
			m_player01.RotateRight();			// ���һ������ת
		}
		if (KEYDOWN(VK_UP)) {					// �Ϸ��������
			// ̹��ײǽ����	
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// ���ײǽ��
					m_player01.ChangeDirection(true);			// �ı�̹�˷���
				}
				else {
					m_player01.Forward();						// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// �·��������
				// ̹��ײǽ�����
			{
				if (m_map.IsHitTheWall(m_player01, false)) {	// ���ײǽ��
					m_player01.ChangeDirection(true);		  	// �ı�̹�˷���
				}
				else {
					m_player01.Backward();				  	// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN('M')) {									// M������
			CBullet blt;
			if (m_player01.Fire(blt)) {						// �����ӵ�
				m_listBullets.push_back(blt);				// ���뵽��ͼ�б���
			}
		}
		
	}
}

void CGame::GameRunLogicOnOne2BotEnd()
{
	// ��������
	// ���ٽ��ܰ���


	// ״̬ά��
	// �Ƴ���Ч�ӵ�
	RemoveTimeoutBullets();

	// �ӵ�ά��
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
	// ��������
	{
		if (KEYDOWN(VK_LEFT)) {				    // ���������
			m_player01.RotateLeft();			// ���һ������ת
		}
		if (KEYDOWN(VK_RIGHT)) {				// �ҷ��������
			m_player01.RotateRight();			// ���һ������ת
		}
		if (KEYDOWN(VK_UP)) {					// �Ϸ��������
			// ̹��ײǽ����	
			{
				if (m_map.IsHitTheWall(m_player01, true)) {		// ���ײǽ��
					m_player01.ChangeDirection(true);			// �ı�̹�˷���
				}
				else {
					m_player01.Forward();						// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN(VK_DOWN)) {								// �·��������
				// ̹��ײǽ�����
			{
				if (m_map.IsHitTheWall(m_player01, false)) {	// ���ײǽ��
					m_player01.ChangeDirection(true);		  	// �ı�̹�˷���
				}
				else {
					m_player01.Backward();				  	// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN('M')) {									// M������
			CBullet blt;
			if (m_player01.Fire(blt)) {						// �����ӵ�
				m_listBullets.push_back(blt);				// ���뵽��ͼ�б���
			}
		}
	}

	// ��������
	{
		if (KEYDOWN('A')) {				    // ���������
			m_player02.RotateLeft();			// ���һ������ת
		}
		if (KEYDOWN('D')) {				// �ҷ��������
			m_player02.RotateRight();			// ���һ������ת
		}
		if (KEYDOWN('W')) {					// �Ϸ��������
			// ̹��ײǽ����	
			{
				if (m_map.IsHitTheWall(m_player02, true)) {		// ���ײǽ��
					m_player02.ChangeDirection(true);			// �ı�̹�˷���
				}
				else {
					m_player02.Forward();						// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN('S')) {								// �·��������
				// ̹��ײǽ�����
			{
				if (m_map.IsHitTheWall(m_player02, false)) {	// ���ײǽ��
					m_player02.ChangeDirection(true);		  	// �ı�̹�˷���
				}
				else {
					m_player02.Backward();				  	// ûײǽ������ǰ��
				}
			}
		}
		if (KEYDOWN('Q')) {									// M������
			CBullet blt;
			if (m_player02.Fire(blt)) {						// �����ӵ�
				m_listBullets.push_back(blt);				// ���뵽��ͼ�б���
			}
		}
	}

	RemoveTimeoutBullets();

	// ����ӵ��Ƿ����̹��
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
	// ��������
	// ���ٽ��ܰ���


	// ״̬ά��
	// �Ƴ���Ч�ӵ�
	RemoveTimeoutBullets();

	// �ӵ�ά��
	ProcessHitBullets();
}

/* ��Ϸ��ͼ����
   �����ͼ��Ϸ�еĶ���
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

	// ���Ƶ���Ļ
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

/* ��������
*/
void CGame::RemoveTimeoutBullets()
{
	// ������ҷ���
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
		float fDirNext = 0; //��������һ���ķ���
		if (!m_map.FindNextDirection(&fDirNext, startX, startY,
			targetX, targetY)) {
			return;
		}
		//��ȡ������̹�˵����ĵ�
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startX, startY);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

		if (!rc.Contains(ptTankCenter)) { 		// �ж� ̹���Ƿ񼺾��ߵ������ĵ�λ����
			m_bot.Forward(); 					// û�е������ĵ㣬����ǰ��
			return;
		}
		else {
			m_bot.SetDirection(fDirNext);		// ���û����˵ķ���		
			float dir;
			// �ж��Ƿ���Դ������
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt))
				{
					m_listBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();					// ������ǰ��
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

	// ���FPS
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);
		SolidBrush brush(Color(0x00, 0x00, 0xFF));
		Gdiplus::Font font(_T("����"), 10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}