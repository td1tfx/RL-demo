#pragma once
#include <utility>
#include <iostream>
//#include <queue>
#include <stack>

using namespace std;
class Snake
{

public:


	typedef struct element
	{
		int reward;
		int state;

	};
	stack <element> episode;
	stack <element> e_value;

	float state_table[2][101][101] = {0};	//auto add 0
	float state_value[101] = { 0 };
	float policy_value[101][2] = {0};
	int value_count[101][2] = { 0 };
	int reward_table[101];
	int policy_table[101];
	int old_policy_table[101] = { 0 };

	pair<int, int>* ladders;
	int act_num;
	int ladder_num;
	int* dice_ranges;
	int pos;
	int state;
	bool isFinish;
	const float lossrate = 0.8;
	float value_state;
	float value_strategy;	

	Snake();
	Snake(int ladder_num);
	~Snake();

	int action(int act);
	int ladderMove(int pos_t);
	void init();
	void getStateTable();
	void policyEvaluation();
	void policyImprovement();
	void policyIteration();
	void mentecarloEvaluation();
	void mentecarloOptimize();
	void mentecarloPolicyImprovement();
	void SARSAEvaluation();
	void SARSAIteration();

};

