#include <bits\stdc++.h>
#include "Enemy.h"

using namespace std;

/**
* copy constructor for enemy
*/
Enemy::Enemy(const Enemy& src) :
    hit_points(src.hit_points),
    score(new int(*src.score))
{
}

/**
* assignment operator for enemy
*/
Enemy& Enemy::operator=(const Enemy& rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    //free old memory
    delete score;

    //copy new memory
    hit_points = rhs.hit_points;
    score = new int(*rhs.score);

    return *this;
}

/**
* set score method
*/
void ArmedEnemy::set_score(const int new_score) {
    *score = new_score;
    cout << "score is now " << *score << "\n";
}

/**
* shoot method
*/
void ArmedEnemy::shoot() {
    if (ammo_level > 0) {
        cout << "bang!\n";
        --ammo_level;
    }
    else {
        cout << "out of ammo\n";
    }
}