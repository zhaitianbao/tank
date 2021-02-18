
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Tank.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/************************************************************/
#define GAME_WIN_W (800)
#define GAME_WIN_H (600)


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
#define MY_STYLE (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|\
                  WS_MINIMIZEBOX|FWS_ADDTOTITLE)
	// 创建窗口
	Create(NULL, _T("ztb.坦克PK"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));

	// 设置客户区大小
	{
		CRect rcCli;
		GetClientRect(rcCli);                        // 获取客户区的大小
		RECT rcFrame = { 0,0,
			m_iWidth + m_iWidth - rcCli.right,
		    m_iHeight + m_iHeight - rcCli.bottom 
		};
		MoveWindow(&rcFrame, TRUE);

	}
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(ETimeIdGameLoop, 0, NULL);
	m_game.SetHandle(GetSafeHwnd());

	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	   case ETimeIdGameLoop: {                                          // 游戏循环ID
	 	  static DWORD dwLastUpdate = GetTickCount();                  // 记录本次时刻
		  if (GetTickCount() - dwLastUpdate >= 20) {                   // 判断时间间隔
			  m_game.EnterFrame(GetTickCount());                       // 进入游戏帧处理
			  dwLastUpdate = GetTickCount();                           // 记录时间间隔
		  }
		// 否则什么都不做
	   }
	   default:
	       break;

	}

	CFrameWnd::OnTimer(nIDEvent);
}
