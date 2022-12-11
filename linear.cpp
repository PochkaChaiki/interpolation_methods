#include <iostream>
#include <vector>

using namespace std;

struct Point
{
	double x;
	double y;
};

vector<double> Linear(vector <Point> train, vector<double> test)
{
	vector<double> ytest;
	ytest.reserve(test.size());
	for (int i = 0; i < train.size(); i++) {
		for (int j = 0; j < test.size(); j++) {
			if (test[j] > train[i].x) {
				ytest.push_back(train[i].y + ((train[i + 1].y - train[i].y)/(train[i+1].x-train[i].x))/(test[j]-train[i].x));
				break;
			}
		}
	};
	return ytest;
}