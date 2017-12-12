#include "stdafx.h"
#include "Snake.h"
#include <time.h>


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
	reward_table[100] = 100;
	getStateTable();
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

int Snake::ladder_move(int pos_t) {
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
				step = ladder_move(step);
				state_table[i][s][step] += prob;
			}
		}
		state_table[i][100][100] = 1;
	}
}
