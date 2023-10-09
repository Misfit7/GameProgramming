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

    //����д
    virtual bool OnUserCreate() = 0;
    virtual bool OnUserUpdate(float fElapsedTime) = 0;
    virtual bool OnUserDestroy() { return true; }

    ~GameEngine();

protected:
    //����״̬
    struct KeyState
    {
        bool bPressed;
        bool bReleased;
        bool bHeld;
    } m_keys[256], m_mouse[5];
    //����
    short m_keyOldState[256] = { 0 };
    short m_keyNewState[256] = { 0 };
    //���
    bool m_mouseOldState[5] = { 0 };
    bool m_mouseNewState[5] = { 0 };
    int m_mousePosX;
    int m_mousePosY;

    //��Ļ��С
    int nScreenWidth;
    int nScreenHeight;
    HANDLE hConsole;
    HANDLE hConsoleIn; //����
    SMALL_RECT rectWindow;
    CHAR_INFO* screen;
    //��Ϸ��
    wstring gameName;
    //��Ϸ�߳�
    void GameThread();
};
