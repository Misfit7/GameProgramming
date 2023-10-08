#define _USE_MATH_DEFINES

#include <bits/stdc++.h>
#include <chrono>

//#include "olcConsoleGameEngine.h"
#include "GameEngine.h"

using namespace std;

//class FPS :public olcConsoleGameEngine
class FPS :public GameEngine
{
public:
    virtual bool OnUserCreate() {
        map += L"#########.......#########.......";
        map += L"#...............#...............";
        map += L"#.......#########.......########";
        map += L"#..............##..............#";
        map += L"#......##......##......##......#";
        map += L"#......##..............##......#";
        map += L"#..............##..............#";
        map += L"###............####............#";
        map += L"##.............###.............#";
        map += L"#............####............###";
        map += L"#..............................#";
        map += L"#..............##..............#";
        map += L"#..............##..............#";
        map += L"#...........#####...........####";
        map += L"#..............................#";
        map += L"###..####....########....#######";
        map += L"####.####.......######..........";
        map += L"#...............#...............";
        map += L"#.......#########.......##..####";
        map += L"#..............##..............#";
        map += L"#......##......##.......#......#";
        map += L"#......##......##......##......#";
        map += L"#..............##..............#";
        map += L"###............####............#";
        map += L"##.............###.............#";
        map += L"#............####............###";
        map += L"#..............................#";
        map += L"#..............................#";
        map += L"#..............##..............#";
        map += L"#...........##..............####";
        map += L"#..............##..............#";
        map += L"################################";
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {
        if (m_keys[L'Q'].bHeld) //(16)0x8000=(10)32768=(2)1000 0000 0000 0000 
            fPlayerA -= (0.75f * fSpeed) * fElapsedTime; //转向速度*帧时间
        if (m_keys[L'E'].bHeld)
            fPlayerA += (0.75f * fSpeed) * fElapsedTime;
        if (m_keys[L'W'].bHeld)
        {
            fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime; //投影到前进角度的X*行走速度*帧时间
            fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime; //投影到前进角度的Y*行走速度*帧时间
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //前进之后碰到墙壁
            {
                fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
                fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
            }
        }
        if (m_keys[L'S'].bHeld)
        {
            fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //前进之后碰到墙壁
            {
                fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
                fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
            }
        }
        if (m_keys[L'A'].bHeld)
        {
            fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //前进之后碰到墙壁
            {
                fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
                fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
            }
        }
        if (m_keys[L'D'].bHeld)
        {
            fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //前进之后碰到墙壁
            {
                fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
                fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
            }
        }

        for (int x = 0; x < ScreenWidth(); x++)
        {
            //依次计算每条光(x条)照到空间物体上的角度
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;
            //该角度下玩家到空间物体的距离
            float fDistanceToWall = 0.0f;
            //光线射出的增量大小
            float fStepSize = 0.1f;
            //循环增量确定是否到达物体范围内，即撞墙
            bool bHitWall = false;
            //是否出界
            bool bBoundary = false;
            //玩家正在看向地方的向量
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += fStepSize; //增量检测碰墙距离
                int nTestX = int(fPlayerX + fEyeX * fDistanceToWall); //检测地点X
                int nTestY = int(fPlayerY + fEyeY * fDistanceToWall); //检测地点Y

                //如果光线超出边界
                if ((nTestX < 0) || (nTestX >= nMapWidth) || (nTestY < 0) || (nTestY >= nMapHeight))
                {
                    bHitWall = true; //跳出循环
                    fDistanceToWall = fDepth; //设置为最大视野距离
                }
                else //在地图内
                {
                    if (map.c_str()[nTestX * nMapWidth + nTestY] == '#') //碰到墙壁
                    {
                        bHitWall = true; //碰到墙壁,跳出循环
                        //对每个墙壁单元的四个角检测
                        vector<pair<float, float>> p; //<到完美角的距离d,点积(向量间角度)dot>
                        for (int tx = 0; tx < 2; tx++)
                        {
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = float(nTestY) + ty - fPlayerY; //预计完美角点与玩家位置的向量
                                float vx = float(nTestX) + tx - fPlayerX;
                                float d = sqrt(vx * vx + vy * vy); //完美角点与玩家的距离
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d); //视线落点单位向量*完美角点单位向量
                                p.push_back(make_pair(d, dot));
                            }
                        }
                        sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

                        float fBound = 0.01; //指定最小角度
                        if (acos(p.at(0).second) < fBound) bBoundary = true; //反余弦得到角度
                        if (acos(p.at(1).second) < fBound) bBoundary = true;
                        //if (acos(p.at(2).second) < fBound) bBoundary = true;
                    }
                }
            }
            //到天花板和地板的距离,距离越远减去的高度越小(比例关系)
            int nCeiling = float(ScreenHeight() / 2.0) - ScreenHeight() / (float(fDistanceToWall)); //屏幕中点-相对于到墙壁距离的屏幕高度
            int nFloor = ScreenHeight() - nCeiling;

            short nShade = ' '; //阴影
            if (fDistanceToWall <= fDepth / 4.0f) nShade = 0x2588; //full balck
            else if (fDistanceToWall <= fDepth / 3.0f) nShade = 0x2593; //dark shade
            else if (fDistanceToWall <= fDepth / 2.0f) nShade = 0x2592; //medium shade
            else if (fDistanceToWall <= fDepth)        nShade = 0x2591; //light shade
            else                                       nShade = ' '; //远处空白

            if (bBoundary)    nShade = ' '; //墙壁边界

            //设置画面
            for (int y = 0; y < ScreenHeight(); y++)
            {
                if (y <= nCeiling)
                    Draw(x, y, L' '); //天空
                else if (y > nCeiling && y <= nFloor)
                    Draw(x, y, nShade); //墙
                else
                {
                    //对地板阴影处理,越往下(y越大)着色越空(比例)
                    float b = 1.0f - ((float(y) - ScreenHeight() / 2.0f) / ((float)ScreenHeight() / 2.0f));
                    if (b < 0.25)      nShade = L'#';
                    else if (b < 0.5)  nShade = L'x';
                    else if (b < 0.75) nShade = L'.';
                    else if (b < 0.9)  nShade = L'-';
                    else               nShade = L' ';
                    Draw(x, y, nShade); //地板
                }
            }

        }
        //显示地图
        for (int nx = 0; nx < nMapWidth; nx++)
        {
            for (int ny = 0; ny < nMapHeight; ny++)
            {
                Draw(nx, ny + 1, map[ny * nMapWidth + nx]);
            }
        }
        //显示玩家位置
        Draw(int(fPlayerY), int(fPlayerX) + 1, L'P');

        return true;
    }

private:
    //玩家位置X/Y，角度A
    float fPlayerX = 14.7f;
    float fPlayerY = 5.09f;
    float fPlayerA = 0.0f;
    //地图
    wstring map;
    //小地图尺寸
    int nMapHeight = 32;
    int nMapWidth = 32;
    //视野范围
    float fFOV = M_PI / 4.0f; //角度
    float fDepth = 16.0f; //最远视野
    //行进速度
    float fSpeed = 5.0f;
};


int main()
{
    FPS game;
    //game.ConstructConsole(320, 240, 4, 4);
    game.Build(360, 120);
    game.Start();

    return 0;
}