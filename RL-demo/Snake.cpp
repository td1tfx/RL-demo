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
	//memset(state_value, 10, 101);
	memset(reward_table, -1, 101); 
	reward_table[100] = 100;
	getStateTable();
	policyEvaluation();
}


Snake::~Snake()
{
}

int Snake::action(int act) {
	int step = rand() % dice_ranges[act]+1;
	pos += step;
	if (pos == 100) {
		isFinish = true;
		return 100;
	}
	else if (pos > 100) {
		pos = 200 - pos;
		cout << "over 100!" << endl;
	}
	for (int i = 0; i < ladder_num; i++) {
		if (pos == ladders[i].first) {
			pos = ladders[i].second;
			cout << ladders[i].first << " ladder to " << ladders[i].second << endl;
			break;
		}
		else if (pos == ladders[i].second) {
			pos = ladders[i].first;
			cout << ladders[i].second << " ladder to " << ladders[i].first << endl;
			break;
		}
	}
	return -1;
}

int Snake::ladderMove(int pos_t) {
	for (int i = 0; i < ladder_num; i++) {
		if (pos_t == ladders[i].first) {
			pos_t = ladders[i].second;
			//cout << ladders[i].first << " ladder to " << ladders[i].second << endl;
			break;
		}
		else if (pos_t == ladders[i].second) {
			pos_t = ladders[i].first;
			//cout << ladders[i].second << " ladder to " << ladders[i].first << endl;
			break;
		}
	}
	return pos_t;
}

void Snake::getStateTable() {
	for (int i = 0; i < 2; i++) {
		float prob = 1.0 / dice_ranges[i];
		for (int s = 0; s < 100; s++) {
			for (int step = 0; step < dice_ranges[i]; step++) {
				step += s;
				step = ladderMove(step);
				state_table[i][s][step] += prob;
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
				for (int j = 0; j < 2; j++) {
					value_sa += state_table[j][i][h] * (lossrate* state_value[i] + reward_table[h]);
				}
				value_sa = value_sa / 2;
				value_sas += value_sa;
			}
			new_state_value[i] = value_sas;
			diff = new_state_value[i] - state_value[i];
			diff = diff*diff;
			tal_diff += diff;
		}
		tal_diff = sqrt(tal_diff);
		if (tal_diff < 0.00001) {
			cout << "iteration times:" << iteration_count << endl;
			break;
		}
		else {
			for (int i = 0; i < 101; i++) {
				state_value[i] = new_state_value[i];
			}
		}
	}
	
}
