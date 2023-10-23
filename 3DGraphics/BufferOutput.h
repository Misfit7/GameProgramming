#pragma once
#pragma comment(lib, "winmm.lib")
#include <bits/stdc++.h>

#include "Color.h"
#include "Mesh.h"

using namespace std;

class GameEngine
{
public:
    GameEngine();
    int Build(int width, int height);
    void Start();
    int ScreenWidth() { return nScreenWidth; }
    int ScreenHeight() { return nScreenHeight; }
    //void mouseposition() { cout << m_mousePosX << ' ' << m_mousePosY << endl; };
    void setGameName(wstring s) { gameName = s; };
    void hideCursor();
    void showCursor();
    void setLimitCursor();
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
    //queue<COORD> m_mousePos;
    int m_mousePosX;
    int m_mousePosY;
    queue<int> turnR;
    bool m_bConsoleInFocus = true; //输入聚焦

    //屏幕大小
    int nScreenWidth;
    int nScreenHeight;
    RECT rect; //窗口范围
    HCURSOR c = GetCursor(); //光标
    POINT* midWindow = new POINT(); //窗口中点
    POINT* curPos = new POINT(); //光标位置
    HANDLE hConsole;
    HANDLE hConsoleIn; //输入
    SMALL_RECT rectWindow;
    CHAR_INFO* screen;
    //游戏名
    wstring gameName;
    //游戏线程
    void GameThread();
    //绘图
    void Fill(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F); //填充
    void DrawString(int x, int y, std::wstring c, short col = 0x000F); //输出字符
    void DrawStringAlpha(int x, int y, std::wstring c, short col = 0x000F); //输出不是unicode字符
    void Clip(int& x, int& y); //限制范围
    void DrawLine(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F); //绘制线
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F); //绘制三角形
    void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F); //填充三角形
    void DrawCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F); //绘制圆形
    void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F); //填充圆形
    void DrawSprite(int x, int y, Sprite* sprite); //绘制像素
    void DrawPartialSprite(int x, int y, Sprite* sprite, int ox, int oy, int w, int h); //绘制部分像素
    //绘制线框模型
    void DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE, short c = PIXEL_SOLID);

};
