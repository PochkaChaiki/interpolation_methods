#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Point
{
    double x; 
    double y;
};

vector <double> Newton(vector <Point> p, vector <double> newx)
{
    vector <double> result;
    result.reserve(newx.size());
    for (int l = 0; l < newx.size(); l++)
    {
        double sum;
        for(int i = 1; i < p.size(); ++i)
        {
            sum = p[0].y;
            double F = 0;
            for(int j = 0; j <= i; ++j)
            {    
                double den = 1;
                for(int k = 0; k <= i; ++k)
                    if (k != j) 
                        den *= (p[j].x - p[k].x);
                F += p[j].y/den;
            }
            for(int k = 0; k < i; ++k)
                F *= (newx[l] - p[k].x);
            sum += F;
        }
        result.push_back(sum);
    }
    return result;
}