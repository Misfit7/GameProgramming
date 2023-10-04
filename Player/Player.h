#pragma once
#include <iostream>

using namespace std;

class Player {
public:
	Player(int hps);
	~Player();
	int get_hit_points() const;
	int get_score() const;
	string get_name() const;
	void set_hit_points(int new_hit_points);
	void set_score(int new_score);
	void set_name(string new_name);
private:
	int hit_points;
	int* score;
	string name;
};

Player::Player(int hps) :
	hit_points(hps) {
	score = new int(hit_points);
}

Player::~Player() {
	delete score;
}

int Player::get_hit_points() const {
	return hit_points;
}

int Player::get_score() const {
	return *score;
}

string Player::get_name() const{
	return name;
}

void Player::set_hit_points(const int new_hit_points) {
	hit_points = new_hit_points;
}

void Player::set_score(const int new_score) {
	*score = new_score;
}

void Player::set_name(string new_name){
	name = new_name;
}
