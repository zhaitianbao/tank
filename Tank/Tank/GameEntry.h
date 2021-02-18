#pragma once

#include <gdiplus.h>
#include <math.h>
#include <cmath>

#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;

#ifndef PI
#define PI 3.1415926f
#endif
class CGameEntry
{
public: 
	CGameEntry()                                      // ���캯��
	{

	}
	virtual ~CGameEntry()                             // ��������
	{

	}
public:
	virtual void Draw(Gdiplus::Graphics &gh) const = 0; // ���ڻ����Լ��Ĵ��麯��
};

class CGameEntryMoveable :public CGameEntry
{
public:
	CGameEntryMoveable()
	{

	}

	virtual ~CGameEntryMoveable()
	{

	}

	// ���ƹ��췽��
	CGameEntryMoveable(const CGameEntryMoveable &rhs)
	{
		if (this != &rhs)
		{
			this->m_bActive = rhs.m_bActive;
			this->m_rect = rhs.m_rect;
			this->m_fSpeed = rhs.m_fSpeed;
			this->m_direction = rhs.m_direction;
			this->m_directionTurn = rhs.m_directionTurn;
		}
	}

	// ���ظ�������
	CGameEntryMoveable &operator=(const CGameEntryMoveable &rhs)
	{
		if (this != &rhs)
		{
			this->m_bActive = rhs.m_bActive;
			this->m_rect = rhs.m_rect;
			this->m_fSpeed = rhs.m_fSpeed;
			this->m_direction = rhs.m_direction;
			this->m_directionTurn = rhs.m_directionTurn;
		}
		return *this;
	}

	virtual void Draw(Gdiplus::Graphics &gh)const = 0;

	/**********************���ƶ�����Ĺ�ͬ����***********************/
	// ��ת
	virtual void RotateRight()
	{
		m_direction += m_directionTurn;       // ��ǰ�Ƕ�����
		if (m_direction >= 360)               // ������һ��֮�󣬼�ȥ360�ȣ�ʹ��ʼ�ձ�����С�Ƕ�
		{
			m_direction -= 360;
		}
	}

	// ��ת
	virtual void RotateLeft()
	{
		m_direction -= m_directionTurn;       // ��ǰ�Ƕ�����
		if (m_direction <0)               // ������һ��֮�󣬼�ȥ360�ȣ�ʹ��ʼ�ձ�����С�Ƕ�
		{
			m_direction =static_cast<float>(360-m_directionTurn);
		}
	}

	// ǰ��
	virtual void Forward()
	{
		// ���ݵ�ǰ�Ƕȣ���xy�����Ͻ�������
		m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}

	// ����
	virtual void Backward()
	{
		// ���ݵ�ǰ�Ƕȣ���xy�����Ͻ�������
		m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}

	// ��ȡˮƽ������ٶȷ���
	virtual float GetXSpeed()const
	{
		return static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	}

	// ��ȡ��ֱ������ٶȷ���
	virtual float GetYSpeed()const
	{
		return static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	}

	// ��һ��ǰ����λ��
	virtual RectF ForwardNextRect()const
	{
		RectF rc = m_rect;
		rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
		return rc;
	}

	// ��һ�����˵ĵ�λ��
	virtual RectF BackwardNextRect()const
	{
		RectF rc = m_rect;
		rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
		rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
		return rc;
	}

	// ��ȡͷ��λ�ã�ͼƬΪ���Σ�ͷ��λ�þ��ǽǶ�Ϊ0ʱ
	// �������ĵ�Ϊ�뾶���Խ���/2����������X���ϵ�λ��
	virtual PointF GetHeadPos()const
	{
		PointF ptCenter = GetCenterPoint();
		PointF ptHead = ptCenter;
		float fRadius = std::sqrt(std::pow(m_rect.Width / 2, 2) + std::pow(m_rect.Height/ 2, 2));
		ptHead.X += fRadius * sin(GetDirectionArc());
		ptHead.Y -= fRadius * cos(GetDirectionArc());
		return ptHead;
	}

	// ��ȡ���ĵ�
	virtual PointF GetCenterPoint()const
	{
		PointF center = PointF(m_rect.X + m_rect.Width / 2, m_rect.Y + m_rect.Height / 2);
		return center;
	}

	// �������ĵ�
	virtual void SetCenterPoint(const PointF &ptCenter)
	{
		PointF cen = GetCenterPoint();
		cen.X = ptCenter.X - cen.X;
		cen.Y = ptCenter.Y - cen.Y;
		m_rect.Offset(cen);
	}

	// ���ýǶ�
	virtual void SetDirectionArc(float dir)
	{
		m_direction = dir * 180.0f / PI;
	}

	// ���ýǶȣ���
	virtual void SetDirection(float dir)
	{
		m_direction = dir;
	}

	// ��ȡ��ǰ�Ƕ�
	virtual float GetDirectionArc()const
	{
		return PI * m_direction / 180.0f;
	}

	// ��ȡ��ǰ�Ƕȣ���
	virtual float GetDirection()const
	{
		return m_direction;
	}

	// ������ת�Ƕ�
	virtual void SetDirectionTurnArc(float dir)
	{
		m_directionTurn = PI * dir / 180.0f;
	}

	// ������ת�Ƕ�
	virtual void SetDirectionTurn(float dir)
	{
		m_directionTurn = dir;
	}

	// ��ȡ��ǰ��ת�Ƕ�
	virtual float GetDirectionTurnArc()const
	{
		return PI * m_directionTurn / 180.0f;
	}

	// ��ȡ��ǰ��ת�Ƕ�
	virtual float GetDirectionTurn()const
	{
		return m_directionTurn;
	}

	// �Ƿ���Ч
	virtual bool IsActive()const
	{
		return m_bActive;
	}

	// �����Ƿ���Ч
	virtual void SetActive(bool bActive)
	{
		m_bActive = bActive;
	}

	// ռ�÷�Χ
	virtual void SetRect(const RectF rect)
	{
		m_rect = rect;
	}

	// ռ�÷�Χ
	virtual RectF GetRect()const
	{
		return m_rect;
	}

	// �����ƶ��ٶ�
	virtual void SetSpeed(float speed)
	{
		m_fSpeed = speed;
	}

	// ��ȡ�ƶ��ٶ�
	virtual float GetSpeed()const
	{
		return m_fSpeed;
	}

private:
	bool m_bActive{ false };                               // �Ƿ���Ч
	RectF m_rect{ 0,0,100,100 };                           // ռ�õ�����
	float m_fSpeed{ 10 };                                  // �ƶ��ٶ�
	float m_direction{ 0 };                                // ��ǰ�ĽǶ�
	float m_directionTurn{ 5 };                            // ÿ����ת�ĽǶ�
};