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

    //设置窗口标题
    SetConsoleTitle(gameName.c_str());
    //限制鼠标在窗口内并隐藏鼠标
    setLimitCursor();

    return 1;
}

void GameEngine::setLimitCursor()
{
    //限制鼠标在窗口内
    HWND hwndFound; //窗口句柄
    while ((hwndFound = FindWindow(NULL, gameName.c_str())) == NULL) {}; //获取唯一命名的窗口
    //根据句柄获取窗口
    GetClientRect(hwndFound, &rect);
    //分类窗口范围
    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;
    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;
    //与整个屏幕的映射
    MapWindowPoints(hwndFound, nullptr, &ul, 1);
    MapWindowPoints(hwndFound, nullptr, &lr, 1);
    //设置窗口在整个屏幕内的范围
    rect.left = ul.x;
    rect.top = ul.y + 42;
    rect.right = lr.x - 22;
    rect.bottom = lr.y - 12;
    //限制鼠标
    ClipCursor(&rect);
    //隐藏鼠标
    hideCursor();
    //窗口中点
    midWindow->x = (rect.left + rect.right) / 2;
    midWindow->y = (rect.top + rect.bottom) / 2;
}

void GameEngine::hideCursor()//隐藏光标
{
    HANDLE h_GAME = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(h_GAME, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(h_GAME, &cursor_info);
}
void GameEngine::showCursor()//显示光标
{
    HANDLE h_GAME = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(h_GAME, &cursor_info);
    cursor_info.bVisible = true;
    SetConsoleCursorInfo(h_GAME, &cursor_info);
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
            case FOCUS_EVENT:
            {
                m_bConsoleInFocus = inBuf[i].Event.FocusEvent.bSetFocus;
                break;
            }

            case MOUSE_EVENT:
                switch (inBuf[i].Event.MouseEvent.dwEventFlags) //事件flag为true
                {
                case MOUSE_MOVED: {
                    //m_mousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X; //鼠标X位置
                    //m_mousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y; //鼠标Y位置
                    /*COORD pos = { inBuf[i].Event.MouseEvent.dwMousePosition.X,inBuf[i].Event.MouseEvent.dwMousePosition.Y };
                    m_mousePos.emplace(pos);*/
                    if (m_bConsoleInFocus)
                    {
                        GetCursorPos(curPos);
                        if (curPos->x < midWindow->x)
                            turnR.emplace(-1);
                        else if (curPos->x > midWindow->x)
                            turnR.emplace(1);
                    }
                    break;
                }
                case 0:
                {
                    for (int m = 0; m < 5; m++)
                    {
                        m_mouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0; //鼠标按键状态
                    }
                    break;
                }
                default:
                    break;
                }
            default:
                if (m_bConsoleInFocus)
                {
                    SetCursorPos(midWindow->x, midWindow->y);
                }
                break;
            }
        }

        //鼠标按键状态设置
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

        //载入用户更新函数
        OnUserUpdate(fElapsedTime);
        ////输出状态信息
        //wchar_t s[256];
        //swprintf_s(s, nScreenWidth * nScreenHeight, L"FPSGame - %s - FPS=%3.2f", gameName.c_str(), 1.0f / fElapsedTime); //最后为帧数(一秒钟处理多少次循环)
        //SetConsoleTitleW(s);
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

void GameEngine::Fill(int x1, int y1, int x2, int y2, short c, short col)
{
    Clip(x1, y1);
    Clip(x2, y2);
    for (int x = x1; x < x2; x++)
        for (int y = y1; y < y2; y++)
            Draw(x, y, c, col);
}

void GameEngine::DrawString(int x, int y, std::wstring c, short col)
{
    for (size_t i = 0; i < c.size(); i++)
    {
        screen[y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
        screen[y * nScreenWidth + x + i].Attributes = col;
    }
}

void GameEngine::DrawStringAlpha(int x, int y, std::wstring c, short col)
{
    for (size_t i = 0; i < c.size(); i++)
    {
        if (c[i] != L' ')
        {
            screen[y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
            screen[y * nScreenWidth + x + i].Attributes = col;
        }
    }
}

void GameEngine::Clip(int& x, int& y)
{
    if (x < 0) x = 0;
    if (x >= nScreenWidth) x = nScreenWidth;
    if (y < 0) y = 0;
    if (y >= nScreenHeight) y = nScreenHeight;
}

void GameEngine::DrawLine(int x1, int y1, int x2, int y2, short c, short col)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;
    dx1 = abs(dx); dy1 = abs(dy);
    px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1; y = y1; xe = x2;
        }
        else
        {
            x = x2; y = y2; xe = x1;
        }

        Draw(x, y, c, col);

        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
                px = px + 2 * dy1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
            Draw(x, y, c, col);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1; y = y1; ye = y2;
        }
        else
        {
            x = x2; y = y2; ye = y1;
        }

        Draw(x, y, c, col);

        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
            Draw(x, y, c, col);
        }
    }
}

void GameEngine::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col)
{
    DrawLine(x1, y1, x2, y2, c, col);
    DrawLine(x2, y2, x3, y3, c, col);
    DrawLine(x3, y3, x1, y1, c, col);
}

void GameEngine::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col)
{
    auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
    auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, c, col); };

    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;   // Starting points
    dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2) goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        if (t1x < t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i < dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) t1xp = signx1;//t1x += signx1;
                else          goto next1;
            }
            if (changed1) break;
            else t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;//t2x += signx2;
                else          goto next2;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next2:
        if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
        if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
        drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                     // Now increase y
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2) break;

    }
next:
    // Second half
    dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x < t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i < dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else   	   	  t1x += signx1;
            if (i < dx1) i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next4:

        if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
        if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
        drawline(minx, maxx, y);
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y > y3) return;
    }
}

void GameEngine::DrawCircle(int xc, int yc, int r, short c, short col)
{
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;
    if (!r) return;

    while (y >= x) // only formulate 1/8 of circle
    {
        Draw(xc - x, yc - y, c, col);//upper left left
        Draw(xc - y, yc - x, c, col);//upper upper left
        Draw(xc + y, yc - x, c, col);//upper upper right
        Draw(xc + x, yc - y, c, col);//upper right right
        Draw(xc - x, yc + y, c, col);//lower left left
        Draw(xc - y, yc + x, c, col);//lower lower left
        Draw(xc + y, yc + x, c, col);//lower lower right
        Draw(xc + x, yc + y, c, col);//lower right right
        if (p < 0) p += 4 * x++ + 6;
        else p += 4 * (x++ - y--) + 10;
    }
}

void GameEngine::FillCircle(int xc, int yc, int r, short c, short col)
{
    // Taken from wikipedia
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;
    if (!r) return;

    auto drawline = [&](int sx, int ex, int ny)
    {
        for (int i = sx; i <= ex; i++)
            Draw(i, ny, c, col);
    };

    while (y >= x)
    {
        // Modified to draw scan-lines instead of edges
        drawline(xc - x, xc + x, yc - y);
        drawline(xc - y, xc + y, yc - x);
        drawline(xc - x, xc + x, yc + y);
        drawline(xc - y, xc + y, yc + x);
        if (p < 0) p += 4 * x++ + 6;
        else p += 4 * (x++ - y--) + 10;
    }
};

void GameEngine::DrawSprite(int x, int y, Sprite* sprite)
{
    if (sprite == nullptr)
        return;

    for (int i = 0; i < sprite->nWidth; i++)
    {
        for (int j = 0; j < sprite->nHeight; j++)
        {
            if (sprite->GetGlyph(i, j) != L' ')
                Draw(x + i, y + j, sprite->GetGlyph(i, j), sprite->GetColour(i, j));
        }
    }
}

void GameEngine::DrawPartialSprite(int x, int y, Sprite* sprite, int ox, int oy, int w, int h)
{
    if (sprite == nullptr)
        return;

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (sprite->GetGlyph(i + ox, j + oy) != L' ')
                Draw(x + i, y + j, sprite->GetGlyph(i + ox, j + oy), sprite->GetColour(i + ox, j + oy));
        }
    }
}

void GameEngine::DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r, float s, short col, short c)
{
    // pair.first = x coordinate
    // pair.second = y coordinate

    // Create translated model vector of coordinate pairs
    std::vector<std::pair<float, float>> vecTransformedCoordinates;
    int verts = vecModelCoordinates.size();
    vecTransformedCoordinates.resize(verts);

    // Rotate
    for (int i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
        vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
    }

    // Scale
    for (int i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
        vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
    }

    // Translate
    for (int i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
        vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
    }

    // Draw Closed Polygon
    for (int i = 0; i < verts + 1; i++)
    {
        int j = (i + 1);
        DrawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
            (int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, c, col);
    }
}

GameEngine::~GameEngine()
{
    delete[]screen;
}