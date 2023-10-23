#define _USE_MATH_DEFINES

#include <bits/stdc++.h>
#include <chrono>

//#include "olcConsoleGameEngine.h"
#include "BufferOutput.h"
#include "Mesh.h"

using namespace std;

//class FPS :public olcConsoleGameEngine
class FPS :public GameEngine
{
public:
    FPS() { setGameName(L"FPSgame"); };

protected:
    virtual bool OnUserCreate() {
        //地图
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

        //初始化屏幕宽度每一列的距离
        fDepthBuffer = new float[ScreenWidth()];

        //图像
        spriteWall = new Sprite(L"fps_wall.spr");
        spriteLamp = new Sprite(L"fps_lamp.spr");
        spriteFireBall = new Sprite(L"fps_fireball.spr");

        //物品对象列表
        listObjects = { { 8.5f, 8.5f, 0.0f, 0.0f, false, spriteLamp },
            { 7.5f, 7.5f, 0.0f, 0.0f, false, spriteLamp },
            { 10.5f, 3.5f, 0.0f, 0.0f, false, spriteLamp } };
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {
        //键盘映射((16)0x8000=(10)32768=(2)1000 0000 0000 0000 )
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
                fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }
        if (m_keys[L'A'].bHeld)
        {
            fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#')
            {
                fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }
        if (m_keys[L'D'].bHeld)
        {
            fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#')
            {
                fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }
        if (m_keys[VK_ESCAPE].bPressed)
        {
            ClipCursor(nullptr);
            m_bConsoleInFocus = false;
            showCursor();
        }
        //鼠标映射
        if (m_mouse[0].bPressed)
        {
            if (m_bConsoleInFocus) {
                sObject Bullet;
                Bullet.x = fPlayerX;
                Bullet.y = fPlayerY;
                float fOffset = (float(rand()) / float(RAND_MAX) - 0.5f) * 0.1f; //子弹偏移散射
                Bullet.vx = sinf(fPlayerA + fOffset) * 8.0f; //速度
                Bullet.vy = cosf(fPlayerA + fOffset) * 8.0f;
                Bullet.sprite = spriteFireBall;
                Bullet.bRemove = false;
                listObjects.push_back(Bullet);
            }
            else {
                m_bConsoleInFocus = true;
                setLimitCursor();
                hideCursor();
            }
        }
        //if (m_keys[L'Q'].bHeld)
        //    fPlayerA -= (0.75f * fSpeed) * fElapsedTime; //转向速度*帧时间
        //if (m_keys[L'E'].bHeld)
        //    fPlayerA += (0.75f * fSpeed) * fElapsedTime;
        while (turnR.size())
        {
            int v = turnR.front();
            turnR.pop();
            switch (v)
            {
            case -1:
                fPlayerA -= (0.35f * fSpeed) * fElapsedTime;
                break;
            case 1:
                fPlayerA += (0.35f * fSpeed) * fElapsedTime;
                break;
            default:
                break;
            }
        }

        for (int x = 0; x < ScreenWidth(); x++)
        {
            //依次计算每条光(x条)照到空间物体上的角度
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;
            //该角度下玩家到空间物体的距离
            float fDistanceToWall = 0.0f;
            //光线射出的增量大小
            float fStepSize = 0.01f;
            //循环增量确定是否到达物体范围内，即撞墙
            bool bHitWall = false;
            //是否出界
            bool bBoundary = false;
            //玩家正在看向地方的向量
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);
            //取样点
            float fSampleX = 0.0f;

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
                        //对每个墙壁单元的边界检测(四条线)
                        float fBlockMidX = float(nTestX) + 0.5f; //单元格中点
                        float fBlockMidY = float(nTestY) + 0.5f;

                        float fTestPointX = fPlayerX + fEyeX * fDistanceToWall; //碰撞点
                        float fTestPointY = fPlayerY + fEyeY * fDistanceToWall;

                        float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX)); //反正切函数得出角度
                        //计算取样点X(根据不同象限)
                        if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f) //第1象限
                            fSampleX = fTestPointY - float(nTestY); //计算纹理上的采样点
                        if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f) //第2象限
                            fSampleX = fTestPointX - float(nTestX);
                        if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f) //第3象限
                            fSampleX = fTestPointY - float(nTestY);
                        if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f) //第4象限
                            fSampleX = fTestPointX - float(nTestX);
                    }
                }
            }
            //到天花板和地板的距离,距离越远减去的高度越小(比例关系)
            int nCeiling = float(ScreenHeight() / 2.0) - ScreenHeight() / (float(fDistanceToWall)); //屏幕中点-相对于到墙壁距离的屏幕高度
            int nFloor = ScreenHeight() - nCeiling;

            //更新缓冲区距离数组
            fDepthBuffer[x] = fDistanceToWall;

            //设置画面
            for (int y = 0; y < ScreenHeight(); y++)
            {
                if (y <= nCeiling)
                    Draw(x, y, L' '); //天空
                else if (y > nCeiling && y <= nFloor)
                {
                    if (fDistanceToWall < fDepth) //视野内绘制纹理
                    {
                        float fSampleY = (float(y) - float(nCeiling)) / (float(nFloor) - float(nCeiling)); //取样点Y(所在行与地板的比例)
                        Draw(x, y, spriteWall->SampleGlyph(fSampleX, fSampleY), spriteWall->SampleColour(fSampleX, fSampleY)); //墙
                    }
                    else Draw(x, y, PIXEL_SOLID, 0); //视野外不着色
                }
                else
                {
                    Draw(x, y, PIXEL_SOLID, FG_DARK_GREEN); //地板
                }
            }

        }
        //绘制装饰物品
        for (auto& object : listObjects)
        {
            //更新物体状态
            object.x += object.vx * fElapsedTime; //速度*每帧时间
            object.y += object.vy * fElapsedTime;
            //撞墙则删除
            if (map.c_str()[int(object.x) * nMapWidth + int(object.y)] == '#')
                object.bRemove = true;
            //是否可见
            float fVectorX = object.x - fPlayerX; //物体X向量
            float fVectorY = object.y - fPlayerY; //物体Y向量
            //距玩家的距离判断
            float fObjectDistanceFromPlayer = sqrtf(fVectorX * fVectorX + fVectorY * fVectorY);
            //是否在玩家视野范围夹角内
            float fEyeX = sinf(fPlayerA);
            float fEyeY = cosf(fPlayerA);
            float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVectorY, fVectorX); //物品的角度(atan(视野落点向量-物品所在向量))
            //调整结果使其位于0,2PI之间
            if (fObjectAngle < -M_PI)
                fObjectAngle += 2.0f * M_PI;
            if (fObjectAngle > M_PI)
                fObjectAngle -= 2.0f * M_PI;
            bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f; //结果的绝对值在视野夹角内
            //判断是否绘制(在视野夹角内，距离玩家一定距离，不超出玩家视野范围)
            if (bInPlayerFOV && fObjectDistanceFromPlayer >= 0.5f && fObjectDistanceFromPlayer < fDepth)
            {
                float fObjectCeiling = float(ScreenHeight()) / 2.0f - float(ScreenHeight()) / fObjectDistanceFromPlayer; //物体天花板位置
                float fObjectFloor = ScreenHeight() - fObjectCeiling; //物体地板位置
                float fObjectHeight = fObjectFloor - fObjectCeiling; //物体的高度
                float fObjectAspectRatio = float(object.sprite->nHeight) / float(object.sprite->nWidth); //素材长宽比
                float fObjectWidth = fObjectHeight / fObjectAspectRatio; //根据素材长宽比得到物体的宽度
                float fMiddleOfObject = (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * float(ScreenWidth()); //物体X中点

                for (float lx = 0; lx < fObjectWidth; lx++)
                {
                    for (float ly = 0; ly < fObjectHeight; ly++)
                    {
                        float fSampleX = lx / fObjectWidth; //X采样
                        float fSampleY = ly / fObjectHeight; //X采样
                        wchar_t c = object.sprite->SampleGlyph(fSampleX, fSampleY); //设置图像
                        int nObjectColumn = int(fMiddleOfObject + lx - (fObjectWidth / 2.0f)); //屏幕X位置,减一半物品宽确保数字范围
                        if (nObjectColumn >= 0 && nObjectColumn < ScreenWidth())
                        {
                            if (c != L' ' && fObjectDistanceFromPlayer <= fDepthBuffer[nObjectColumn]) //图像不空白并且没有被遮挡则绘制
                            {
                                Draw(nObjectColumn, fObjectCeiling + ly, c, object.sprite->SampleColour(fSampleX, fSampleY));
                                fDepthBuffer[nObjectColumn] = fObjectDistanceFromPlayer; //更新绘制物体后的距离
                            }
                        }
                    }
                }
            }

        }
        //回收删除的物体
        listObjects.remove_if([](sObject& o) {return o.bRemove; });

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

    //深度缓冲区(记录距离)
    float* fDepthBuffer = nullptr;

    //图像
    Sprite* spriteWall;
    Sprite* spriteLamp;
    Sprite* spriteFireBall;

    //装饰物品
    struct sObject
    {
        float x, y; //XY坐标
        float vx, vy; //XY速度
        bool bRemove; //是否删除
        Sprite* sprite;
    };
    //装饰物品的对象列表
    list<sObject> listObjects;
};


int main()
{
    FPS game;
    //game.ConstructConsole(320, 240, 4, 4);
    game.Build(420, 140);
    game.Start();

    return 0;
}