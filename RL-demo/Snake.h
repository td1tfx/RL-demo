#pragma once
#include <utility>
#include <iostream>

using namespace std;
class Snake
{

public:

	pair<int, int>* ladders;
	int act_num;
	int ladder_num;
	int* dice_ranges;
	int pos;
	bool isFinish;

	Snake();
	Snake(int ladder_num);
	~Snake();

	int action(int act);

};

