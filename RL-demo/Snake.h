#pragma once
#include <utility>
#include <iostream>

using namespace std;
class Snake
{

public:

	float state_table[2][101][101] = {0};
	float state_value[101] = {0};
	int reward_table[101];

	pair<int, int>* ladders;
	int act_num;
	int ladder_num;
	int* dice_ranges;
	int pos;
	bool isFinish;
	const float lossrate = 0.8;
	float value_state;
	float value_strategy;	

	Snake();
	Snake(int ladder_num);
	~Snake();

	int action(int act);
	int ladderMove(int pos_t);
	void getStateTable();
	void policyEvaluation();

};

