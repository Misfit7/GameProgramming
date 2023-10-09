#include <bits\stdc++.h>

#include "BufferOutput.h"

using namespace std;

GameEngine::GameEngine() {
    //程序名
    gameName = L"Default";
    //按键状态内存初始化
    memset(m_keyOldState, 0, 256 * sizeof(short));
    memset(m_keyNewState, 0, 256 * sizeof(short));
    memset(m_keys, 0, 256 * sizeof(KeyState));
    m_mousePosX = 0;
    m_mousePosY = 0;
    //获得设备句柄
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //设备输出句柄
    hConsoleIn = GetStdHandle(STD_INPUT_HANDLE); //设备输入句柄
};

int GameEngine::Build(int width, int height)
{
    nScreenWidth = width;
    nScreenHeight = height;

    //句柄
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    //创建输出
    SetConsoleActiveScreenBuffer(hConsole);
    //窗口位置及大小
    rectWindow = { 0, 0, static_cast<short>(nScreenWidth - 1), static_cast<short>(nScreenHeight - 1) };
    //鼠标输入
    SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    //初始化屏幕输出流
    screen = new CHAR_INFO[nScreenWidth * nScreenHeight];
    memset(screen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);

    return 1;
}

//启动
void GameEngine::Start() {
    thread t = thread(&GameEngine::GameThread, this);

    t.join();
}

//游戏线程
void GameEngine::GameThread() {
    //载入用户创建函数
    OnUserCreate();

    //计算持续时间差
    auto tp1 = chrono::system_clock::now(); //start
    auto tp2 = chrono::system_clock::now(); //end

    //循环刷新游戏
    while (1)
    {
        tp2 = chrono::system_clock::now(); //时间end
        chrono::duration<float> elapsedTime = tp2 - tp1; //时间差
        tp1 = tp2; //更新旧时间起始
        float fElapsedTime = elapsedTime.count(); //转为浮点

        //循环检测按键状态
        for (int i = 0; i < 256; i++)
        {
            m_keyNewState[i] = GetAsyncKeyState(i); //更新状态

            m_keys[i].bPressed = false; //重置
            m_keys[i].bReleased = false;

            if (m_keyNewState[i] != m_keyOldState[i]) //状态改变
            {
                if (m_keyNewState[i] & 0x8000) //按下
                {
                    m_keys[i].bPressed = !m_keys[i].bHeld;
                    m_keys[i].bHeld = true;
                }
                else
                {
                    m_keys[i].bReleased = true;
                    m_keys[i].bHeld = false;
                }
            }
            m_keyOldState[i] = m_keyNewState[i];
        }

        //鼠标输入事件
        INPUT_RECORD inBuf[32]; //记录输入
        DWORD events = 0;
        GetNumberOfConsoleInputEvents(hConsoleIn, &events); //鼠标事件的编号
        if (events > 0) //捕捉到鼠标事件
            ReadConsoleInput(hConsoleIn, inBuf, events, &events);
        //只判断鼠标移动和点击
        for (DWORD i = 0; i < events; i++)
        {
            switch (inBuf[i].EventType) {

            }
        }

        //载入用户更新函数
        OnUserUpdate(fElapsedTime);
        //输出状态信息
        //wchar_t s[256];
        //swprintf_s(s, nScreenWidth * nScreenHeight, L"FPSGame - %s - FPS=%3.2f", gameName.c_str(), 1.0f / fElapsedTime); //最后为帧数(一秒钟处理多少次循环)
        //SetConsoleTitle(s);
        //输出画面
        WriteConsoleOutputW(hConsole, screen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &rectWindow);
    }
}

void GameEngine::Draw(int x, int y, short c, short col)
{
    //更改输出流数组
    if (x >= 0 && x < nScreenWidth && y >= 0 && y < nScreenHeight)
    {
        screen[y * nScreenWidth + x].Char.UnicodeChar = c;
        screen[y * nScreenWidth + x].Attributes = col;
    }
}

GameEngine::~GameEngine()
{
    delete[] m_keyOldState;
    delete[] m_keyNewState;
    delete[] m_keys;
    delete[]screen;
}