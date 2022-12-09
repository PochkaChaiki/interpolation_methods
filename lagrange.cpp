#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct Point {
    double x, y; 
};

vector<double> Lagrange(vector<Point>p, vector <double> newx) {
    vector <double> ys;
    ys.reserve(newx.size());
    for (int k = 0; k < newx.size(); k++) {
        double result = 0;
        for (int i = 0; i < p.size(); i++) {
            double y = p[i].y;
            for (int j = 0; j < p.size(); j++) {
                if (j != i) y *= (newx[k] - p[j].x) / (p[i].x - p[j].x);
            }
            result += y;
        }
        ys.push_back(result);
    }
    return ys;
}