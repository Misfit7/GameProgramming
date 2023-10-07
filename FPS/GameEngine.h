#pragma once
#pragma comment(lib, "winmm.lib")
#include <bits/stdc++.h>

using namespace std;

enum COLOUR
{
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007,
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
};

//����
enum PIXEL_TYPE
{
    PIXEL_SOLID = 0x2588,
    PIXEL_THREEQUARTERS = 0x2593,
    PIXEL_HALF = 0x2592,
    PIXEL_QUARTER = 0x2591,
};

class GameEngine
{
public:
    GameEngine() {};
    void Build(int width, int height);
    void Start();
    int ScreenWidth() { return nScreenWidth; }
    int ScreenHeight() { return nScreenHeight; }
    void Draw(int x, int y, short c, short col);

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
    } m_keys[256];
    short m_keyOldState[256] = { 0 };
    short m_keyNewState[256] = { 0 };

private:
    //��Ļ��С
    int nScreenWidth;
    int nScreenHeight;
    HANDLE hConsole;
    SMALL_RECT rectWindow;
    CHAR_INFO* screen;
    wstring gameName;
    void GameThread();
};

void GameEngine::Build(int width, int height)
{
    nScreenWidth = width;
    nScreenHeight = height;
    //������Ļ�����
    screen = new CHAR_INFO[nScreenWidth * nScreenHeight];
    memset(screen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    rectWindow = { 0, 0, static_cast<short>(nScreenWidth - 1), static_cast<short>(nScreenHeight - 1) };
    //������
    gameName = L"Default";
    //����״̬�ڴ��ʼ��
    std::memset(m_keyOldState, 0, 256 * sizeof(short));
    std::memset(m_keyNewState, 0, 256 * sizeof(short));
    std::memset(m_keys, 0, 256 * sizeof(KeyState));
}

//����
void GameEngine::Start() {
    thread t = thread(&GameEngine::GameThread, this);

    t.join();
}

//��Ϸ�߳�
void GameEngine::GameThread() {
    //�����û���������
    OnUserCreate();

    //�������ʱ���
    auto tp1 = chrono::system_clock::now(); //start
    auto tp2 = chrono::system_clock::now(); //end

    //ѭ��ˢ����Ϸ
    while (1)
    {
        tp2 = chrono::system_clock::now(); //ʱ��end
        chrono::duration<float> elapsedTime = tp2 - tp1; //ʱ���
        tp1 = tp2; //���¾�ʱ����ʼ
        float fElapsedTime = elapsedTime.count(); //תΪ����

        //ѭ����ⰴ��״̬
        for (int i = 0; i < 256; i++)
        {
            m_keyNewState[i] = GetAsyncKeyState(i); //����״̬

            m_keys[i].bPressed = false; //����
            m_keys[i].bReleased = false;

            if (m_keyNewState[i] != m_keyOldState[i]) //״̬�ı�
            {
                if (m_keyNewState[i] & 0x8000) //����
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
        //�����û����º���
        OnUserUpdate(fElapsedTime);
        //���״̬��Ϣ
        //wchar_t s[256];
        //swprintf_s(s, nScreenWidth * nScreenHeight, L"FPSGame - %s - FPS=%3.2f", gameName.c_str(), 1.0f / fElapsedTime); //���Ϊ֡��(һ���Ӵ������ٴ�ѭ��)
        //SetConsoleTitle(s);
        //�������
        WriteConsoleOutputW(hConsole, screen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &rectWindow);
    }
}

void GameEngine::Draw(int x, int y, short c = PIXEL_SOLID, short col = FG_WHITE)
{
    //�������������
    if (x >= 0 && x < nScreenWidth && y >= 0 && y < nScreenHeight)
    {
        screen[y * nScreenWidth + x].Char.UnicodeChar = c;
        screen[y * nScreenWidth + x].Attributes = col;
    }
}

GameEngine::~GameEngine()
{
    /*delete[] m_keyOldState;
    delete[] m_keyNewState;
    delete[] m_keys;*/
    delete[]screen;
}