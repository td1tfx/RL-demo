#include "stdafx.h"
#include "Snake.h"
#include <time.h>
#include <vector>


Snake::Snake()
{
	Snake(10);
}

Snake::Snake(int ladder_num_t) {
	srand((unsigned)time(NULL));
	pos = 0;
	act_num = 2;
	dice_ranges = new int[act_num];
	dice_ranges[0] = 3;
	dice_ranges[1] = 6;
	ladder_num = ladder_num_t;
	ladders = new pair<int, int>[ladder_num];
	for (int i = 0; i < ladder_num;i++) {
		ladders[i].first = rand() % 100;
		for (int j = 0; j < i; j++) {
			while (ladders[i].first == ladders[j].first) {
				ladders[i].first = rand() % 100;
			}
		}
	}
	for (int i = 0; i < ladder_num; i++) {
		ladders[i].second = rand() % 100;
		while (ladders[i].second == ladders[i].first) {
			ladders[i].second = rand() % 100;
		}
	}
	isFinish = false;
	memset(reward_table, -1, 101*sizeof(int)); 
// 	for (int h = 0; h < 101; h++) {
// 		for (int j = 0; j < 2; j++) {
// 			policy_table[j][h] = 0.5;
// 		}
// 	}
	//memset(policy_table, 0.500, 202 * sizeof(float));
	reward_table[100] = 100;
	getStateTable();
	policyIteration();
}


Snake::~Snake()
{
}

int Snake::action(int act) {
	int step = rand() % dice_ranges[act]+1;
	pos += step;
	if (pos == 100) {
		isFinish = true;
		pos = -1;
		return 100;
	}
	else if (pos > 100) {
		pos = 200 - pos;
		//cout << "over 100!" << endl;
	}
	for (int i = 0; i < ladder_num; i++) {
		if (pos == ladders[i].first) {
			pos = ladders[i].second;
			//cout << ladders[i].first << " ladder to " << ladders[i].second << endl;
			break;
		}
		else if (pos == ladders[i].second) {
			pos = ladders[i].first;
			//cout << ladders[i].second << " ladder to " << ladders[i].first << endl;
			break;
		}
	}
	return -1;
}

int Snake::ladderMove(int pos_t) {
	for (int i = 0; i < ladder_num; i++) {
		if (pos_t == ladders[i].first) {
			pos_t = ladders[i].second;
			break;
		}
		if (pos_t == ladders[i].second) {
			pos_t = ladders[i].first;
			break;
		}
	}
	return pos_t;
}

void Snake::getStateTable() {
	for (int i = 0; i < 2; i++) {
		float prob = 1.0 / dice_ranges[i];
		for (int s = 0; s < 100; s++) {
			for (int step = 1; step < dice_ranges[i]+1; step++) {
				int step1 = step + s;
				step1 = ladderMove(step1);
				state_table[i][s][step1] += prob;
			}
		}
		state_table[i][100][100] = 1;
	}
}

void Snake::policyEvaluation() {
	int iteration_count = 0;
	while(true) {
		iteration_count++;
		float new_state_value[101];
		for (int i = 0; i < 101; i++) {
			new_state_value[i] = state_value[i];
		}
		float tal_diff = 0;
		float diff = 0;
		for (int i = 0; i < 101; i++) {
			float value_sas = 0;
			for (int h = 0; h < 101; h++) {
				float value_sa = 0;
				value_sa += state_table[policy_table[i]][i][h] * (lossrate* state_value[h] + reward_table[h]);
				value_sas += value_sa;
			}
			new_state_value[i] = value_sas;
			diff = new_state_value[i] - state_value[i];
			diff = diff*diff;
			tal_diff += diff;
		}
		tal_diff = sqrt(tal_diff);
		if (tal_diff < 0.00001) {
			//cout << "iteration times:" << iteration_count << endl;
			break;
		}
		else {
			for (int i = 0; i < 101; i++) {
				state_value[i] = new_state_value[i];
			}
		}
	}
	
}

void Snake::policyImprovement() {
	for (int i = 0; i < 101; i++) {
		old_policy_table[i] = policy_table[i];
	}
	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 2; j++) {
			for (int h = 0; h < 101; h++) {
				policy_value[i][j] += state_table[j][i][h] * (lossrate* state_value[h] + reward_table[h]);
			}
		}
		if (policy_value[i][0] >= policy_value[i][1]) {
			policy_table[i] = 0;
		}
		else {
			policy_table[i] = 1;
		}
	}
}

void Snake::policyIteration() {
	bool isContinue = true;
	int iteration_count = 0;
	while (isContinue) {
		iteration_count++;
		policyEvaluation();
		policyImprovement();	
		for (int i = 0; i < 101; i++) {
			if (old_policy_table[i] != policy_table[i]) {
			break;
			}
			if (i == 100) {
				isContinue = false;
				cout << "iteration times:" << iteration_count << endl;
			}
		}
	}	
}

void Snake::mentecarloEvaluation() {
	int prev_state;
	prev_state = pos;
	while(true) {
		element element_t;
		element_t[1] = pos;
		element_t[0] = action(policy_table[pos]);
		episode.push(element_t);
		if (pos == -1) {
			break;
		}
	}
	int return_value;
	while (!episode.empty())
	{

		return_value = return_value * lossrate + episode.top()[0];
		element e_t = { return_value, episode.top()[1]};
		e_value.push(e_t);
		episode.pop();
	}
	while (!e_value.empty()) {
		int act = policy_table[e_value.top()[1]];
		value_count[e_value.top()[1]][act]++;
		policy_value[e_value.top()[1]][act] = (e_value.top()[0] - policy_value[e_value.top()[1]][act]) / value_count[e_value.top()[1]][act];
		e_value.pop();
	}
}

void Snake::mentecarloOptimize() {
	bool isContinue = true;
	int iteration_count = 0;
	while (isContinue) {
		iteration_count++;
		mentecarloEvaluation();
		policyImprovement();
		for (int i = 0; i < 101; i++) {
			if (old_policy_table[i] != policy_table[i]) {
				break;
			}
			if (i == 100) {
				isContinue = false;
				cout << "iteration times:" << iteration_count << endl;
			}
		}
	}
}
