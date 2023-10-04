#pragma once
#include <bits/stdc++.h>

using namespace std;

class BattleShip
{
public:
    BattleShip();
    BattleShip(int hps);
    void Hit_success();
    void B_printScore();
    void set_Score(int s);
    int get_Score();
    int get_hps();
    ~BattleShip();

private:
    int hit_points;
    shared_ptr<int> score;
};

BattleShip::BattleShip() :hit_points(2)
{
    score = make_shared<int>(0);
}

BattleShip::BattleShip(int hps) : hit_points(hps)
{
    score = make_shared<int>(0);
}

void BattleShip::Hit_success()
{
    (*score)++;
}

inline void BattleShip::B_printScore()
{
    cout << "your score is " << *score << endl;
}

void BattleShip::set_Score(int s)
{
    *score = s;
}

int BattleShip::get_Score()
{
    return *score;
}

int BattleShip::get_hps()
{
    return hit_points;
}

BattleShip::~BattleShip()
{
}