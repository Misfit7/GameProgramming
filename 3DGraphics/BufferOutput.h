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
    //queue<COORD> m_mousePos;
    int m_mousePosX;
    int m_mousePosY;
    queue<int> turnR;
    bool m_bConsoleInFocus = true; //����۽�

    //��Ļ��С
    int nScreenWidth;
    int nScreenHeight;
    RECT rect; //���ڷ�Χ
    HCURSOR c = GetCursor(); //���
    POINT* midWindow = new POINT(); //�����е�
    POINT* curPos = new POINT(); //���λ��
    HANDLE hConsole;
    HANDLE hConsoleIn; //����
    SMALL_RECT rectWindow;
    CHAR_INFO* screen;
    //��Ϸ��
    wstring gameName;
    //��Ϸ�߳�
    void GameThread();
    //��ͼ
    void Fill(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F); //���
    void DrawString(int x, int y, std::wstring c, short col = 0x000F); //����ַ�
    void DrawStringAlpha(int x, int y, std::wstring c, short col = 0x000F); //�������unicode�ַ�
    void Clip(int& x, int& y); //���Ʒ�Χ
    void DrawLine(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F); //������
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F); //����������
    void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F); //���������
    void DrawCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F); //����Բ��
    void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F); //���Բ��
    void DrawSprite(int x, int y, Sprite* sprite); //��������
    void DrawPartialSprite(int x, int y, Sprite* sprite, int ox, int oy, int w, int h); //���Ʋ�������
    //�����߿�ģ��
    void DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE, short c = PIXEL_SOLID);

};
