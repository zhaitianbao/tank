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

	void SetHandle(HWND hWnd);                             // 设置输出窗口的句柄

	bool EnterFrame(DWORD dwTime);                         // 进入游戏帧

	void OnMouseMove(UINT nFlags, CPoint point);           // 处理鼠标移动事件

	void OnLButtonUp(UINT nFlags, CPoint point);           // 处理左键抬起事件

	// 游戏所处的阶段
	enum EGameType {
		EGameTypeMenu=0,          // 选择阶段
		EGameTypeOne2BotMenu,     // 人机大战菜单阶段
		EGameTypeOne2Bot,         // 人机大战
		EGameTypeOne2BotEnd,      // 人机大战结束
		EGameTypeOne2OneMenu,     // 双人大战菜单阶段
		EGameTypeOne2One,         // 双人大战
		EGameTypeOne2OneEnd,      // 双人大战结束
		EGameTypeCount,           // 7个阶段
	};

	// 设置当前阶段，并根据步骤初始化
	void SetStep(CGame::EGameType step);

private:
	HWND m_hWnd;                                           // 窗口

	/* 游戏初始化
	   生成游戏对象，初始化地图和对象未知
	*/
	bool GameInit();
	
	bool GameInitMenu();

	bool GameInitOne2BotMenu();

	bool GameInitOne2Bot();

	bool GameInitOne2BotEnd();

	bool GameInitOne2OneMenu();

	bool GameInitOne2One();

	bool GameInitOne2OneEnd();

	// 把上述方法放入数组
	bool (CGame::*m_initFunc[EGameTypeCount])() = {
		&CGame::GameInitMenu,
		&CGame::GameInitOne2BotMenu,
		&CGame::GameInitOne2Bot,
		&CGame::GameInitOne2BotEnd,
		&CGame::GameInitOne2OneMenu,
		&CGame::GameInitOne2One,
		&CGame::GameInitOne2OneEnd,
	};

	/* 游戏绘图处理
	   负责绘图游戏中的对象
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

	/* 游戏逻辑处理
	 * 1.维护子弹状态
	 * 2.维护机器人
	 * 3.维护玩家坦克的状态
	 * 碰撞检测包括：撞墙，子弹命中坦克
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

	/* 辅助方法
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

