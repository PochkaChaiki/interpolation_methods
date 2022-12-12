#ifndef METHODS_H
#define METHODS_H

#include <vector>

struct Point{
    double x;
    double y;
};

std::vector<double> Newton(std::vector <Point> p, std::vector <double> newx);

std::vector<double> Lagrange(std::vector<Point> p, std::vector <double> newx);

std::vector<double> Linear(std::vector <Point> p, std::vector<double> newx);

std::vector<double> LSM(std::vector <Point> p, std::vector <double> newx, int polinom = 1);


#endif