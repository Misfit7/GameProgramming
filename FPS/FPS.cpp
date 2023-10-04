#include <bits/stdc++.h>

using namespace std;

//屏幕大小
int nScreenWidth = 120;
int nScreenHeight = 40;

//玩家位置X/Y，角度A
float fPlayerX = 0.0f;
float fPlayerY = 0.0f;
float fPlayerA = 0.0f;

//小地图尺寸
int nMapHeight = 16;
int nMapWidth = 16;

int main()
{
    //创建屏幕输出流
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    //地图
    wstring map;


    //游戏循环刷新
    while (1)
    {

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    cout << "Hello World!" << endl;
}