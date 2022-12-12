#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;
struct Point
{
    double x;
    double y;
};
vector <double> LSM(vector <Point> p, vector <double> newx, int  polinom = 1) {
    vector <vector<double>> matrix;
    for (int i = 0; i <= polinom; ++i)
    {
        vector<double> a;
        double sum_xy = 0;
        for (int j = 0; j <= polinom; ++j)
        {
            double sum_x = 0;
            for (int k = 0; k < p.size(); ++k)
                sum_x += pow(p[k].x, i + j);
            a.push_back(sum_x);
        }
        for (int j = 0; j < p.size(); ++j)
            sum_xy += p[j].y * pow(p[j].x, i);
        a.push_back(sum_xy);
        matrix.push_back(a);
    }
    for (int i = 0; i <= polinom; ++i)
    {
        if (matrix[i][i] == 0)
            matrix[i].swap(matrix[(1 + i) % polinom + 1]);
        double delitel = matrix[i][i];
        for (auto& a : matrix[i])
            a /= delitel;
        for (int j = i + 1; j < polinom + 1; j++)
        {
            double x = matrix[j][i];
            for (int k = 0; k <= polinom + 1; k++)
                matrix[j][k] -= matrix[i][k] * x;
        }
    }
    for (int i = polinom; i > 0; --i)
        for (int j = i - 1; j >= 0; --j)
        {
            double x = matrix[j][i];
            for (int k(0); k <= polinom + 1; ++k)
                matrix[j][k] -= matrix[i][k] * x;
        }
    vector<double> A;
    for (auto a : matrix)
        A.push_back(a[polinom + 1]);
    vector<double> Y;
    for (int i(0); i < newx.size(); ++i)
    {
        double sum = 0;
        for (int j(0); j < A.size(); ++j)
            sum += A[j] * pow(newx[i], j);
        Y.push_back(sum);
    }
    return Y;
}