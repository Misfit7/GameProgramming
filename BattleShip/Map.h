#pragma once
#include <bits/stdc++.h>
#include "Battleship.h"

using namespace std;


class Map
{
public:
    Map(int size);
    void M_print();
    void M_tprint();
    void M_rebuild();
    bool M_judgeHit(int x, int y);
    bool M_judgeWon();
    int get_Msize();
    ~Map();

private:
    int M_size;
    int B_xPosition, B_yPosition;
    vector<vector<char>> M_map;
    shared_ptr<BattleShip> ship;
};

Map::Map(int size) :B_xPosition(0), B_yPosition(0)
{
    ship = make_shared<BattleShip>(BattleShip(2));
    M_size = size;
    for (int i = 0; i < size; i++) {
        vector<char> m(size, '-');
        M_map.push_back(m);
    }
}

void Map::M_print()
{
    for (int x = 0; x < M_size; x++) {
        for (int y = 0; y < M_size; y++) {
            cout << '-' << ' ';
        }
        cout << endl;
    }
    cout << B_xPosition << ' ' << B_yPosition << endl;
}

void Map::M_tprint()
{
    ship->B_printScore();
    for (auto y : M_map) {
        for (auto x : y) {
            cout << x << ' ';
        }
        cout << endl;
    }
}

void Map::M_rebuild()
{
    M_map[B_xPosition][B_yPosition] = '-';
    int x = rand() % M_size, y = rand() % M_size;
    B_xPosition = x;
    B_yPosition = y;
    M_map[x][y] = '*';
}

bool Map::M_judgeHit(int x, int y)
{
    if (x == B_xPosition && y == B_yPosition)
    {
        ship->Hit_success();
        return true;
    }
    return false;
}

bool Map::M_judgeWon()
{
    if (ship->get_Score() >= ship->get_hps())
    {
        return true;
    }
    return false;
}
int Map::get_Msize() {
    return M_size;
}

Map::~Map()
{
}