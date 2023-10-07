//#define _USE_MATH_DEFINES
//
//#include <bits/stdc++.h>
//#include <chrono>
//
//using namespace std;
//
////屏幕大小
//int nScreenWidth = 120;
//int nScreenHeight = 40;
//
////玩家位置X/Y，角度A
//float fPlayerX = 14.7f;
//float fPlayerY = 5.09f;
//float fPlayerA = 0.0f;
//
////小地图尺寸
//int nMapHeight = 16;
//int nMapWidth = 16;
//
////视野范围
//float fFOV = M_PI / 4.0f; //角度
//float fDepth = 16.0f; //最远视野
//
////行进速度
//float fSpeed = 5.0f;
//
//int main()
//{
//    //创建屏幕输出流
//    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
//    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//    SetConsoleActiveScreenBuffer(hConsole);
//    DWORD dwBytesWritten = 0;
//
//    //地图
//    wstring map;
//    map += L"################";
//    map += L"#..............#";
//    map += L"#.......########";
//    map += L"#..............#";
//    map += L"#......##......#";
//    map += L"#......##......#";
//    map += L"#..............#";
//    map += L"###............#";
//    map += L"##.............#";
//    map += L"#......####..###";
//    map += L"#......#.......#";
//    map += L"#......#.......#";
//    map += L"#..............#";
//    map += L"#......#########";
//    map += L"#..............#";
//    map += L"################";
//
//    //计算持续时间差
//    auto tp1 = chrono::system_clock::now(); //start
//    auto tp2 = chrono::system_clock::now(); //end
//
//    //游戏循环刷新
//    while (1)
//    {
//        tp2 = chrono::system_clock::now(); //时间end
//        chrono::duration<float> elapsedTime = tp2 - tp1; //时间差
//        tp1 = tp2; //更新旧时间起始
//        float fElapsedTime = elapsedTime.count(); //转为浮点
//
//        //玩家转向,GetAsyncKeyState返回32768或0
//        if (GetAsyncKeyState(unsigned short('A')) & 0x8000) //(16)0x8000=(10)32768=(2)1000 0000 0000 0000 
//            fPlayerA -= (0.75f * fSpeed) * fElapsedTime; //转向速度*帧时间
//        if (GetAsyncKeyState(unsigned short('D')) & 0x8000)
//            fPlayerA += (0.75f * fSpeed) * fElapsedTime;
//        if (GetAsyncKeyState(unsigned short('W')) & 0x8000)
//        {
//            fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime; //投影到前进角度的X*行走速度*帧时间
//            fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime; //投影到前进角度的Y*行走速度*帧时间
//            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //前进之后碰到墙壁
//            {
//                fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
//                fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
//            }
//        }
//        if (GetAsyncKeyState(unsigned short('S')) & 0x8000)
//        {
//            fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
//            fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
//            if (map.c_str()[int(fPlayerX) * nMapWidth + int(fPlayerY)] == '#') //前进之后碰到墙壁
//            {
//                fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
//                fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime; //退回原位
//            }
//        }
//
//        for (int x = 0; x < nScreenWidth; x++)
//        {
//            //依次计算每条光(x条)照到空间物体上的角度
//            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
//            //该角度下玩家到空间物体的距离
//            float fDistanceToWall = 0.0f;
//            //光线射出的增量大小
//            float fStepSize = 0.1f;
//            //循环增量确定是否到达物体范围内，即撞墙
//            bool bHitWall = false;
//            //是否出界
//            bool bBoundary = false;
//            //玩家正在看向地方的向量
//            float fEyeX = sinf(fRayAngle);
//            float fEyeY = cosf(fRayAngle);
//
//            while (!bHitWall && fDistanceToWall < fDepth)
//            {
//                fDistanceToWall += fStepSize; //增量检测碰墙距离
//                int nTestX = int(fPlayerX + fEyeX * fDistanceToWall); //检测地点X
//                int nTestY = int(fPlayerY + fEyeY * fDistanceToWall); //检测地点Y
//
//                //如果光线超出边界
//                if ((nTestX < 0) || (nTestX >= nMapWidth) || (nTestY < 0) || (nTestY >= nMapHeight))
//                {
//                    bHitWall = true; //跳出循环
//                    fDistanceToWall = fDepth; //设置为最大视野距离
//                }
//                else //在地图内
//                {
//                    if (map.c_str()[nTestX * nMapWidth + nTestY] == '#') //碰到墙壁
//                    {
//                        bHitWall = true; //碰到墙壁,跳出循环
//                        //对每个墙壁单元的四个角检测
//                        vector<pair<float, float>> p; //<到完美角的距离d,点积(向量间角度)dot>
//                        for (int tx = 0; tx < 2; tx++)
//                        {
//                            for (int ty = 0; ty < 2; ty++)
//                            {
//                                float vy = float(nTestY) + ty - fPlayerY; //预计完美角点与玩家位置的向量
//                                float vx = float(nTestX) + tx - fPlayerX;
//                                float d = sqrt(vx * vx + vy * vy); //完美角点与玩家的距离
//                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d); //视线落点单位向量*完美角点单位向量
//                                p.push_back(make_pair(d, dot));
//                            }
//                        }
//                        sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });
//
//                        float fBound = 0.01; //指定最小角度
//                        if (acos(p.at(0).second) < fBound) bBoundary = true; //反余弦得到角度
//                        if (acos(p.at(1).second) < fBound) bBoundary = true;
//                        if (acos(p.at(2).second) < fBound) bBoundary = true;
//                    }
//                }
//            }
//            //到天花板和地板的距离,距离越远减去的高度越小(比例关系)
//            int nCeiling = float(nScreenHeight / 2.0) - nScreenHeight / (float(fDistanceToWall)); //屏幕中点-相对于到墙壁距离的屏幕高度
//            int nFloor = nScreenHeight - nCeiling;
//
//            short nShade = ' '; //阴影
//            if (fDistanceToWall <= fDepth / 4.0f) nShade = 0x2588; //full balck
//            else if (fDistanceToWall <= fDepth / 3.0f) nShade = 0x2593; //dark shade
//            else if (fDistanceToWall <= fDepth / 2.0f) nShade = 0x2592; //medium shade
//            else if (fDistanceToWall <= fDepth)        nShade = 0x2591; //light shade
//            else                                       nShade = ' '; //远处空白
//
//            if (bBoundary)    nShade = ' '; //墙壁边界
//
//            //设置画面
//            for (int y = 0; y < nScreenHeight; y++)
//            {
//                if (y <= nCeiling)
//                    screen[y * nScreenWidth + x] = ' '; //天空
//                else if (y > nCeiling && y <= nFloor)
//                    screen[y * nScreenWidth + x] = nShade; //墙
//                else
//                {
//                    //对地板阴影处理,越往下(y越大)着色越空(比例)
//                    float b = 1.0f - ((float(y) - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
//                    if (b < 0.25)      nShade = '#';
//                    else if (b < 0.5)  nShade = 'x';
//                    else if (b < 0.75) nShade = '.';
//                    else if (b < 0.9)  nShade = '-';
//                    else               nShade = ' ';
//                    screen[y * nScreenWidth + x] = nShade; //地板
//                }
//            }
//        }
//        //显示状态信息(错位)
//        //swprintf_s(screen, nScreenWidth * nScreenHeight, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime); //最后为帧数(一秒钟处理多少次循环)
//        //显示地图
//        for (int nx = 0; nx < nMapWidth; nx++)
//        {
//            for (int ny = 0; ny < nMapHeight; ny++)
//            {
//                screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
//            }
//        }
//        //显示玩家位置
//        screen[(int(fPlayerX) + 1) * nScreenWidth + int(fPlayerY)] = 'P';
//
//        //输出画面
//        screen[(nScreenWidth * nScreenHeight) - 1] = '\0';
//        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
//    }
//
//    return 0;
//}