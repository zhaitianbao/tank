#pragma once

#include <tuple>
#include <vector>
#include "GameEntry.h"

using namespace std;

class CGame;

class CGameMenuPanel:public CGameEntryMoveable
{
public:
	CGameMenuPanel();
	~CGameMenuPanel();

    // 存放一个CGame类的指针在本类
	void SetParent(CGame *g)
	{
		m_pParent = g;
	};

	CGame *m_pParent{ nullptr };

	virtual void Draw(Graphics &gh) const;

	RectF m_rect{ 0,0,800,600 };

	void OnMouseMove(UINT nFlags, CPoint point);

	void OnLButtonUp(UINT nFlags, CPoint point);

	Image *m_imgBackground;

	struct {                                              // 菜单项
		vector<tuple<Image *, RectF>> vImgInfoPtr;
		int index;
	}m_menuItems[2];
	
};

