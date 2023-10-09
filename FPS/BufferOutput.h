#pragma once
#pragma comment(lib, "winmm.lib")
#include <bits/stdc++.h>

#include "Color.h"

using namespace std;

class GameEngine
{
public:
    GameEngine();
    int Build(int width, int height);
    void Start();
    int ScreenWidth() { return nScreenWidth; }
    int ScreenHeight() { return nScreenHeight; }
    void Draw(int x, int y, short c = PIXEL_SOLID, short col = FG_WHITE);

    //需重写
    virtual bool OnUserCreate() = 0;
    virtual bool OnUserUpdate(float fElapsedTime) = 0;
    virtual bool OnUserDestroy() { return true; }

    ~GameEngine();

protected:
    //按键状态
    struct KeyState
    {
        bool bPressed;
        bool bReleased;
        bool bHeld;
    } m_keys[256], m_mouse[5];
    //键盘
    short m_keyOldState[256] = { 0 };
    short m_keyNewState[256] = { 0 };
    //鼠标
    bool m_mouseOldState[5] = { 0 };
    bool m_mouseNewState[5] = { 0 };
    int m_mousePosX;
    int m_mousePosY;

    //屏幕大小
    int nScreenWidth;
    int nScreenHeight;
    HANDLE hConsole;
    HANDLE hConsoleIn; //输入
    SMALL_RECT rectWindow;
    CHAR_INFO* screen;
    //游戏名
    wstring gameName;
    //游戏线程
    void GameThread();
};
