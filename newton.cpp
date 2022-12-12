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
    result.reserve(newx.size()); // подготавливаем место для последующего заполнения
    for (int l = 0; l < newx.size(); l++) 
    {
		double sum = p[0].y; // вводим значение у(0)
        for(int i = 1; i < p.size(); ++i)
        {
            double F = 0;
            for(int j = 0; j <= i; ++j) //Начало вычисления разделенной разности порядка j
            {    
                double den = 1;
                for(int k = 0; k <= i; ++k)
                    if (k != j) 
                        den *= (p[j].x - p[k].x);
                F += p[j].y/den; // Нашли разделенную разность порядка j
            }
            for(int k = 0; k < i; ++k) 
                F *= (newx[l] - p[k].x); 
            sum += F; // находим искомое значение функции в точке y(i)
        }
        result.push_back(sum);
    }
    return result;
}