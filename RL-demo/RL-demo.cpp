// RL-demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Snake.h"

using namespace std;
int main()
{

	auto snake = new Snake(10);
	for (int i = 0; i < snake->ladder_num; i++) {
		
		cout << snake->ladders[i].first << "->" << snake->ladders[i].second << ";";
	}
	cout << endl;
	cout << "action0, Begin!" << endl;

	int reward = 0;
	int step_count = 0;
	while (!snake->isFinish) {
		int perReward= snake->action(0);
		reward += perReward;
		//cout << perReward << ";" << snake->pos << endl;
		step_count++;
	}
	cout << "reward = " << reward << endl;
	cout << "step count= " << step_count << endl;

	cout << "action1, Begin!" << endl;

	reward = 0;
	step_count = 0;
	snake->init();
	while (!snake->isFinish) {
		int perReward = snake->action(1);
		reward += perReward;
		//cout << perReward << ";" << snake->pos << endl;
		step_count++;
	}
	cout << "reward = " << reward << endl;
	cout << "step count= " << step_count << endl;

	cout << "RL action, Begin!" << endl;

	reward = 0;
	step_count = 0;
	snake->init();
	snake->policyIteration();
	while (!snake->isFinish) {
		int perReward = snake->action(snake->policy_table[snake->pos]);
		reward += perReward;
		//cout << perReward << ";" << snake->pos << endl;
		step_count++;
	}
	cout << "reward = " << reward << endl;
	cout << "step count= " << step_count << endl;

	
	cout << "RL mentecarlo action, Begin!" << endl;

	reward = 0;
	step_count = 0;
	snake->init();
	snake->mentecarloOptimize();
	snake->isFinish = false;
	snake->pos = 0;
	while (!snake->isFinish) {
		int perReward = snake->action(snake->policy_table[snake->pos]);
		reward += perReward;
		//cout << perReward << ";" << snake->pos << endl;
		step_count++;
	}
	cout << "reward = " << reward << endl;
	cout << "step count= " << step_count << endl;

	cout << "RL SARSA action, Begin!" << endl;

	reward = 0;
	step_count = 0;
	snake->init();
	snake->SARSAIteration();
	snake->isFinish = false;
	snake->pos = 0;
	while (!snake->isFinish) {
		int perReward = snake->action(snake->policy_table[snake->pos]);
		reward += perReward;
		//cout << perReward << ";" << snake->pos << endl;
		step_count++;
	}
	cout << "reward = " << reward << endl;
	cout << "step count= " << step_count << endl;

	system("pause");

    return 0;
}

