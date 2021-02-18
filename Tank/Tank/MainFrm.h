
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

#include "CGame.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame() noexcept;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:

// 实现
public:
	virtual ~CMainFrame();

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	int m_iWidth{ 800 };
	int m_iHeight{ 600 };

	enum ETimerId{ ETimeIdGameLoop=1};                   // 定时器ID
	CGame m_game;                                        // 游戏对象

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


