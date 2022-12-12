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
	for (int i = 0; i < test.size(); ++i) {
		for (int j = 0; j < train.size(); ++j) {
			if (test[i] > train[j].x) {
				ytest.push_back(train[j].y + (test[i]-train[j].x)*(train[j+1].y-train[j].y)/(train[j+1].x-train[j].x));
				break;
			}
		}
	};
	return ytest;
}