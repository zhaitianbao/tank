#pragma once
#include "GameEntry.h"
#include "GameTimer.h"

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;

class CGame;
class CKeyMenuPlayer01 : public CGameEntryMoveable
{
public:
    CKeyMenuPlayer01();
    ~CKeyMenuPlayer01();

    // »­
    virtual void Draw(Graphics &gh) const;

    // Í£Ö¹¶¯»­
    void SetStop(bool bStop = true)
    {
        m_bStop = bStop;
    };
    bool GetStop() const
    {
        return m_bStop;
    }

private:
    mutable CGameTimeval m_time{300};
    // ²Ëµ¥ Í¼Æ¬
    Image *m_img[2];

    // ÊÇ·ñÍ£Ö¹¶¯»­×´Ì¬
    bool m_bStop{false};
};

