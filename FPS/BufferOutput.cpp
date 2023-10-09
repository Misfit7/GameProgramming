#include <bits\stdc++.h>

#include "BufferOutput.h"

using namespace std;

GameEngine::GameEngine() {
    //������
    gameName = L"Default";
    //����״̬�ڴ��ʼ��
    memset(m_keyOldState, 0, 256 * sizeof(short));
    memset(m_keyNewState, 0, 256 * sizeof(short));
    memset(m_keys, 0, 256 * sizeof(KeyState));
    m_mousePosX = 0;
    m_mousePosY = 0;
    //����豸���
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //�豸������
    hConsoleIn = GetStdHandle(STD_INPUT_HANDLE); //�豸������
};

int GameEngine::Build(int width, int height)
{
    nScreenWidth = width;
    nScreenHeight = height;

    //���
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    //�������
    SetConsoleActiveScreenBuffer(hConsole);
    //����λ�ü���С
    rectWindow = { 0, 0, static_cast<short>(nScreenWidth - 1), static_cast<short>(nScreenHeight - 1) };
    //�������
    SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    //��ʼ����Ļ�����
    screen = new CHAR_INFO[nScreenWidth * nScreenHeight];
    memset(screen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);

    //���ô��ڱ���
    SetConsoleTitle(gameName.c_str());
    //��������ڴ����ڲ��������
    setLimitCursor();

    return 1;
}

void GameEngine::setLimitCursor()
{
    //��������ڴ�����
    HWND hwndFound; //���ھ��
    while ((hwndFound = FindWindow(NULL, gameName.c_str())) == NULL) {}; //��ȡΨһ�����Ĵ���
    RECT rect; //���ڷ�Χ
    GetClientRect(hwndFound, &rect);
    //���ര�ڷ�Χ
    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;
    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;
    //��������Ļ��ӳ��
    MapWindowPoints(hwndFound, nullptr, &ul, 1);
    MapWindowPoints(hwndFound, nullptr, &lr, 1);
    //���ô�����������Ļ�ڵķ�Χ
    rect.left = ul.x;
    rect.top = ul.y + 42;
    rect.right = lr.x - 22;
    rect.bottom = lr.y - 12;
    //�������
    ClipCursor(&rect);
    //�������
    ShowCursor(0);
}

void GameEngine::setMouseVisable(bool visiable)
{
    CONSOLE_CURSOR_INFO cursor_info = { sizeof(CONSOLE_CURSOR_INFO),visiable };
    SetConsoleCursorInfo(hConsole, &cursor_info);
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

        //��������¼�
        INPUT_RECORD inBuf[32]; //��¼����
        DWORD events = 0;
        GetNumberOfConsoleInputEvents(hConsoleIn, &events); //����¼��ı��
        if (events > 0) //��׽������¼�
            ReadConsoleInput(hConsoleIn, inBuf, events, &events);
        //ֻ�ж�����ƶ��͵��
        for (DWORD i = 0; i < events; i++)
        {
            switch (inBuf[i].EventType) {
            case FOCUS_EVENT:
            {
                m_bConsoleInFocus = inBuf[i].Event.FocusEvent.bSetFocus;
                break;
            }

            case MOUSE_EVENT:
                switch (inBuf[i].Event.MouseEvent.dwEventFlags) //�¼�flagΪtrue
                {
                case MOUSE_MOVED: {
                    m_mousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X; //���Xλ��
                    m_mousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y; //���Yλ��
                    break;
                }
                case 0:
                {
                    for (int m = 0; m < 5; m++)
                    {
                        m_mouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0; //��갴��״̬
                    }
                    break;
                }
                default:
                    break;
                }
            default:
                break;
            }
        }

        //��갴��״̬����
        for (int m = 0; m < 5; m++)
        {
            m_mouse[m].bPressed = false;
            m_mouse[m].bReleased = false;
            if (m_mouseNewState[m] != m_mouseOldState[m])
            {
                if (m_mouseNewState[m])
                {
                    m_mouse[m].bPressed = true;
                    m_mouse[m].bHeld = true;
                }
                else
                {
                    m_mouse[m].bReleased = true;
                    m_mouse[m].bHeld = false;
                }
            }
            m_mouseOldState[m] = m_mouseNewState[m];
        }

        //�����û����º���
        OnUserUpdate(fElapsedTime);
        ////���״̬��Ϣ
        //wchar_t s[256];
        //swprintf_s(s, nScreenWidth * nScreenHeight, L"FPSGame - %s - FPS=%3.2f", gameName.c_str(), 1.0f / fElapsedTime); //���Ϊ֡��(һ���Ӵ�����ٴ�ѭ��)
        //SetConsoleTitleW(s);
        //�������
        WriteConsoleOutputW(hConsole, screen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &rectWindow);
    }
}

void GameEngine::Draw(int x, int y, short c, short col)
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
    delete[] m_keyOldState;
    delete[] m_keyNewState;
    delete[] m_keys;
    delete[]screen;
}