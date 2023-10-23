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
        //��ͼ
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

        //��ʼ����Ļ���ÿһ�еľ���
        fDepthBuffer = new float[ScreenWidth()];

        //ͼ��
        spriteWall = new Sprite(L"fps_wall.spr");
        spriteLamp = new Sprite(L"fps_lamp.spr");
        spriteFireBall = new Sprite(L"fps_fireball.spr");

        //��Ʒ�����б�
        listObjects = { { 8.5f, 8.5f, 0.0f, 0.0f, false, spriteLamp },
            { 7.5f, 7.5f, 0.0f, 0.0f, false, spriteLamp },
            { 10.5f, 3.5f, 0.0f, 0.0f, false, spriteLamp } };
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {
        //����ӳ��((16)0x8000=(10)32768=(2)1000 0000 0000 0000 )
        if (m_keys[L'W'].bHeld)
        {
            fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime; //ͶӰ��ǰ���Ƕȵ�X*�����ٶ�*֡ʱ��
            fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime; //ͶӰ��ǰ���Ƕȵ�Y*�����ٶ�*֡ʱ��
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //ǰ��֮������ǽ��
            {
                fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime; //�˻�ԭλ
                fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime; //�˻�ԭλ
            }
        }
        if (m_keys[L'S'].bHeld)
        {
            fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //ǰ��֮������ǽ��
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
        //���ӳ��
        if (m_mouse[0].bPressed)
        {
            if (m_bConsoleInFocus) {
                sObject Bullet;
                Bullet.x = fPlayerX;
                Bullet.y = fPlayerY;
                float fOffset = (float(rand()) / float(RAND_MAX) - 0.5f) * 0.1f; //�ӵ�ƫ��ɢ��
                Bullet.vx = sinf(fPlayerA + fOffset) * 8.0f; //�ٶ�
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
        //    fPlayerA -= (0.75f * fSpeed) * fElapsedTime; //ת���ٶ�*֡ʱ��
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
            //���μ���ÿ����(x��)�յ��ռ������ϵĽǶ�
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;
            //�ýǶ�����ҵ��ռ�����ľ���
            float fDistanceToWall = 0.0f;
            //���������������С
            float fStepSize = 0.01f;
            //ѭ������ȷ���Ƿ񵽴����巶Χ�ڣ���ײǽ
            bool bHitWall = false;
            //�Ƿ����
            bool bBoundary = false;
            //������ڿ���ط�������
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);
            //ȡ����
            float fSampleX = 0.0f;

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += fStepSize; //���������ǽ����
                int nTestX = int(fPlayerX + fEyeX * fDistanceToWall); //���ص�X
                int nTestY = int(fPlayerY + fEyeY * fDistanceToWall); //���ص�Y

                //������߳����߽�
                if ((nTestX < 0) || (nTestX >= nMapWidth) || (nTestY < 0) || (nTestY >= nMapHeight))
                {
                    bHitWall = true; //����ѭ��
                    fDistanceToWall = fDepth; //����Ϊ�����Ұ����
                }
                else //�ڵ�ͼ��
                {
                    if (map.c_str()[nTestX * nMapWidth + nTestY] == '#') //����ǽ��
                    {
                        bHitWall = true; //����ǽ��,����ѭ��
                        //��ÿ��ǽ�ڵ�Ԫ�ı߽���(������)
                        float fBlockMidX = float(nTestX) + 0.5f; //��Ԫ���е�
                        float fBlockMidY = float(nTestY) + 0.5f;

                        float fTestPointX = fPlayerX + fEyeX * fDistanceToWall; //��ײ��
                        float fTestPointY = fPlayerY + fEyeY * fDistanceToWall;

                        float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX)); //�����к����ó��Ƕ�
                        //����ȡ����X(���ݲ�ͬ����)
                        if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f) //��1����
                            fSampleX = fTestPointY - float(nTestY); //���������ϵĲ�����
                        if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f) //��2����
                            fSampleX = fTestPointX - float(nTestX);
                        if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f) //��3����
                            fSampleX = fTestPointY - float(nTestY);
                        if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f) //��4����
                            fSampleX = fTestPointX - float(nTestX);
                    }
                }
            }
            //���컨��͵ذ�ľ���,����ԽԶ��ȥ�ĸ߶�ԽС(������ϵ)
            int nCeiling = float(ScreenHeight() / 2.0) - ScreenHeight() / (float(fDistanceToWall)); //��Ļ�е�-����ڵ�ǽ�ھ������Ļ�߶�
            int nFloor = ScreenHeight() - nCeiling;

            //���»�������������
            fDepthBuffer[x] = fDistanceToWall;

            //���û���
            for (int y = 0; y < ScreenHeight(); y++)
            {
                if (y <= nCeiling)
                    Draw(x, y, L' '); //���
                else if (y > nCeiling && y <= nFloor)
                {
                    if (fDistanceToWall < fDepth) //��Ұ�ڻ�������
                    {
                        float fSampleY = (float(y) - float(nCeiling)) / (float(nFloor) - float(nCeiling)); //ȡ����Y(��������ذ�ı���)
                        Draw(x, y, spriteWall->SampleGlyph(fSampleX, fSampleY), spriteWall->SampleColour(fSampleX, fSampleY)); //ǽ
                    }
                    else Draw(x, y, PIXEL_SOLID, 0); //��Ұ�ⲻ��ɫ
                }
                else
                {
                    Draw(x, y, PIXEL_SOLID, FG_DARK_GREEN); //�ذ�
                }
            }

        }
        //����װ����Ʒ
        for (auto& object : listObjects)
        {
            //��������״̬
            object.x += object.vx * fElapsedTime; //�ٶ�*ÿ֡ʱ��
            object.y += object.vy * fElapsedTime;
            //ײǽ��ɾ��
            if (map.c_str()[int(object.x) * nMapWidth + int(object.y)] == '#')
                object.bRemove = true;
            //�Ƿ�ɼ�
            float fVectorX = object.x - fPlayerX; //����X����
            float fVectorY = object.y - fPlayerY; //����Y����
            //����ҵľ����ж�
            float fObjectDistanceFromPlayer = sqrtf(fVectorX * fVectorX + fVectorY * fVectorY);
            //�Ƿ��������Ұ��Χ�н���
            float fEyeX = sinf(fPlayerA);
            float fEyeY = cosf(fPlayerA);
            float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVectorY, fVectorX); //��Ʒ�ĽǶ�(atan(��Ұ�������-��Ʒ��������))
            //�������ʹ��λ��0,2PI֮��
            if (fObjectAngle < -M_PI)
                fObjectAngle += 2.0f * M_PI;
            if (fObjectAngle > M_PI)
                fObjectAngle -= 2.0f * M_PI;
            bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f; //����ľ���ֵ����Ұ�н���
            //�ж��Ƿ����(����Ұ�н��ڣ��������һ�����룬�����������Ұ��Χ)
            if (bInPlayerFOV && fObjectDistanceFromPlayer >= 0.5f && fObjectDistanceFromPlayer < fDepth)
            {
                float fObjectCeiling = float(ScreenHeight()) / 2.0f - float(ScreenHeight()) / fObjectDistanceFromPlayer; //�����컨��λ��
                float fObjectFloor = ScreenHeight() - fObjectCeiling; //����ذ�λ��
                float fObjectHeight = fObjectFloor - fObjectCeiling; //����ĸ߶�
                float fObjectAspectRatio = float(object.sprite->nHeight) / float(object.sprite->nWidth); //�زĳ����
                float fObjectWidth = fObjectHeight / fObjectAspectRatio; //�����زĳ���ȵõ�����Ŀ��
                float fMiddleOfObject = (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * float(ScreenWidth()); //����X�е�

                for (float lx = 0; lx < fObjectWidth; lx++)
                {
                    for (float ly = 0; ly < fObjectHeight; ly++)
                    {
                        float fSampleX = lx / fObjectWidth; //X����
                        float fSampleY = ly / fObjectHeight; //X����
                        wchar_t c = object.sprite->SampleGlyph(fSampleX, fSampleY); //����ͼ��
                        int nObjectColumn = int(fMiddleOfObject + lx - (fObjectWidth / 2.0f)); //��ĻXλ��,��һ����Ʒ��ȷ�����ַ�Χ
                        if (nObjectColumn >= 0 && nObjectColumn < ScreenWidth())
                        {
                            if (c != L' ' && fObjectDistanceFromPlayer <= fDepthBuffer[nObjectColumn]) //ͼ�񲻿հײ���û�б��ڵ������
                            {
                                Draw(nObjectColumn, fObjectCeiling + ly, c, object.sprite->SampleColour(fSampleX, fSampleY));
                                fDepthBuffer[nObjectColumn] = fObjectDistanceFromPlayer; //���»��������ľ���
                            }
                        }
                    }
                }
            }

        }
        //����ɾ��������
        listObjects.remove_if([](sObject& o) {return o.bRemove; });

        //��ʾ��ͼ
        for (int nx = 0; nx < nMapWidth; nx++)
        {
            for (int ny = 0; ny < nMapHeight; ny++)
            {
                Draw(nx, ny + 1, map[ny * nMapWidth + nx]);
            }
        }
        //��ʾ���λ��
        Draw(int(fPlayerY), int(fPlayerX) + 1, L'P');
        return true;
    }

private:
    //���λ��X/Y���Ƕ�A
    float fPlayerX = 14.7f;
    float fPlayerY = 5.09f;
    float fPlayerA = 0.0f;
    //��ͼ
    wstring map;
    //С��ͼ�ߴ�
    int nMapHeight = 32;
    int nMapWidth = 32;
    //��Ұ��Χ
    float fFOV = M_PI / 4.0f; //�Ƕ�
    float fDepth = 16.0f; //��Զ��Ұ
    //�н��ٶ�
    float fSpeed = 5.0f;

    //��Ȼ�����(��¼����)
    float* fDepthBuffer = nullptr;

    //ͼ��
    Sprite* spriteWall;
    Sprite* spriteLamp;
    Sprite* spriteFireBall;

    //װ����Ʒ
    struct sObject
    {
        float x, y; //XY����
        float vx, vy; //XY�ٶ�
        bool bRemove; //�Ƿ�ɾ��
        Sprite* sprite;
    };
    //װ����Ʒ�Ķ����б�
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