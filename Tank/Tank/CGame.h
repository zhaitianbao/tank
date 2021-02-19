#pragma once

#include "CGameMenuBackground.h"
#include "CGameMenuPanel.h"
#include "GameMenuPanel2.h"
#include "CTankEntry.h"
#include "CPlayer.h"
#include "CBot.h"
#include "GameMap.h"
#include "CBullet.h"
#include "Wall.h"
#include "GameMap.h"
#include "KeyMenuPlayer01.h"
#include "KeyMenuPlayer02.h"
#include <windef.h>
#include <iostream>
#include <Afxwin.h>

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;

class CGame
{
public:
	CGame();

	~CGame();

	void SetHandle(HWND hWnd);                             // ����������ڵľ��

	bool EnterFrame(DWORD dwTime);                         // ������Ϸ֡

	void OnMouseMove(UINT nFlags, CPoint point);           // ��������ƶ��¼�

	void OnLButtonUp(UINT nFlags, CPoint point);           // �������̧���¼�

	// ��Ϸ�����Ľ׶�
	enum EGameType {
		EGameTypeMenu=0,          // ѡ��׶�
		EGameTypeOne2BotMenu,     // �˻���ս�˵��׶�
		EGameTypeOne2Bot,         // �˻���ս
		EGameTypeOne2BotEnd,      // �˻���ս����
		EGameTypeOne2OneMenu,     // ˫�˴�ս�˵��׶�
		EGameTypeOne2One,         // ˫�˴�ս
		EGameTypeOne2OneEnd,      // ˫�˴�ս����
		EGameTypeCount,           // 7���׶�
	};

	// ���õ�ǰ�׶Σ������ݲ����ʼ��
	void SetStep(CGame::EGameType step);

private:
	HWND m_hWnd;                                           // ����

	/* ��Ϸ��ʼ��
	   ������Ϸ���󣬳�ʼ����ͼ�Ͷ���δ֪
	*/
	bool GameInit();
	
	bool GameInitMenu();

	bool GameInitOne2BotMenu();

	bool GameInitOne2Bot();

	bool GameInitOne2BotEnd();

	bool GameInitOne2OneMenu();

	bool GameInitOne2One();

	bool GameInitOne2OneEnd();

	// ������������������
	bool (CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd,
	};

	/* ��Ϸ��ͼ����
	   �����ͼ��Ϸ�еĶ���
	**/
	void GameRunDraw();

	void GameRunDrawOnMenu(Graphics &gh);

	void GameRunDrawOnOne2BotMenu(Graphics &gh);

	void GameRunDrawOnOne2Bot(Graphics &gh);

	void GameRunDrawOnOne2BotEnd(Graphics &gh);

	void GameRunDrawOnOne2OneMenu(Graphics &gh);

	void GameRunDrawOnOne2One(Graphics &gh);

	void GameRunDrawOnOne2OneEnd(Graphics &gh);

	void(CGame::*m_drawFunc[EGameTypeCount])(Graphics &) = {
		&CGame::GameRunDrawOnMenu,
		&CGame::GameRunDrawOnOne2BotMenu,
		&CGame::GameRunDrawOnOne2Bot,
		&CGame::GameRunDrawOnOne2BotEnd,
		&CGame::GameRunDrawOnOne2OneMenu,
		&CGame::GameRunDrawOnOne2One,
		&CGame::GameRunDrawOnOne2OneEnd,
	};

	/* ��Ϸ�߼�����
	 * 1.ά���ӵ�״̬
	 * 2.ά��������
	 * 3.ά�����̹�˵�״̬
	 * ��ײ��������ײǽ���ӵ�����̹��
	**/
	void GameRunLogic();

	void GameRunLogicOnMenu();

	void GameRunLogicOnOne2BotMenu();

	void GameRunLogicOnOne2Bot();

	void GameRunLogicOnOne2BotEnd();

	void GameRunLogicOnOne2OneMenu();

	void GameRunLogicOnOne2One();

	void GameRunLogicOnOne2OneEnd();

	void(CGame::*m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicOnMenu,
	    &CGame::GameRunLogicOnOne2BotMenu,
		&CGame::GameRunLogicOnOne2Bot,
		&CGame::GameRunLogicOnOne2BotEnd,
	    &CGame::GameRunLogicOnOne2OneMenu,
		&CGame::GameRunLogicOnOne2One,
		&CGame::GameRunLogicOnOne2OneEnd,
	};

	/* ��������
	*/
	void RemoveTimeoutBullets();

	void ProcessHitBullets();

	void AI();

	void DrawFps(Graphics &gh);

	int m_fps{ 0 };

	EGameType m_eStep{ EGameTypeMenu };

	CPlayer m_player01;

	CPlayer m_player02;

	CBot m_bot;

	std::list<CBullet> m_listBullets;

	CGameMap m_map{ 10,10,780,580 };

	CGameMenuBackground m_menu;

	CGameMenuPanel m_menuSelect;

	CGameMenuPanel2 m_menuBackup;

	CKeyMenuPlayer01 m_keymenu01;

	CKeyMenuPlayer02 m_keymenu02;

};

