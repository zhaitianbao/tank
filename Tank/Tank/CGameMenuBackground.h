#pragma once

#include "GameEntry.h"

class CGame;

class CGameMenuBackground:public CGameEntryMoveable
{
public:
	CGameMenuBackground();
	~CGameMenuBackground();

	void SetParent(CGame *g)
	{
		m_pParent = g;
	};

	CGame *m_pParent{ nullptr };

	virtual void Draw(Graphics &gh) const;

	RectF m_rect{ 0,0,800,600 };

	Image *m_imgBackground;
};

