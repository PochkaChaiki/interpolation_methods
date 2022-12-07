#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <unordered_map>

#include "methods.h"

using Method = std::vector<double> (*) (std::vector<Point>, std::vector<double>);

double meanAbsoluteError(std::vector<double> functionValues, std::vector<double> interpolatedValues){
    double error(0);
    for (int i(0); i<functionValues.size(); ++i)
        error += std::abs(functionValues[i] - interpolatedValues[i]);
    error /= functionValues.size();
    return error;
}

double SMAPE(std::vector<double> y_true, std::vector<double> y_predicted) {
    double smape = 0;

    for (int i(0); i < y_true.size(); ++i) 
        smape += std::abs(y_true[i] - y_predicted[i]) / ((std::abs(y_true[i]) + std::abs(y_predicted[i])) / 2);
    
    return smape / y_true.size() * 100;
}

long double r2_score(std::vector<double> y_true, std::vector<double> y_predicted) {
    long double mean = 0;

    for (int i(0); i < y_true.size(); ++i) 
        mean += y_true[i];

    mean = mean / y_true.size();

    long double s_res = 0;
    long double s_tot = 0;

    for (int i(0); i < y_true.size(); ++i) {
        s_res += (y_true[i] - y_predicted[i]) * (y_true[i] - y_predicted[i]);
        s_tot += (y_true[i] - mean) * (y_true[i] - mean);
    }

    return 1 - s_res / s_tot;
}

void gen(std::vector<Point>& data, std::vector<double>& testx, std::vector<double>& testy){
    auto f = [] (double x) { return std::sin(std::pow(x, 3/2)) / std::log(x);};
    // auto f = [] (double x) {return std::pow(x, 2);};
    int pointsAmount = 30;
    double lb = -2;
    double rb = 2;
    double h = std::abs(rb-lb)/pointsAmount;
    
    data.reserve(pointsAmount);
    testx.reserve((int)pointsAmount*0.2);
    testy.reserve((int)pointsAmount*0.2);
    double x(lb);
    int i(1);
    while(x < rb){
        Point p;
        p.x = x;
        p.y = f(x); 
        if (i % 5 == 0){
            testx.push_back(p.x - h/2);
            testy.push_back(f(p.x - h/2));
        } 
        data.push_back(p);
        
        i++;
        x += h;
    }
}

void printValues(std::vector<double> values){
    for(auto i: values)
        std::cout<<i<<"\n";
}

int main(){
    std::unordered_map<std::string, Method> interpolationMethods {{"Newton",Newton}, {"Lagrange", Lagrange}, {"Linear", Linear}, {"LSM", LSM}};
    std::vector<Point> data;
    std::vector<double> testx;
    std::vector<double> testy;
    
    gen(data, testx, testy);
    // printValues(testy);
    
    std::cout<<"Methods"<<std::endl;
    for (auto [name, method]: interpolationMethods){
        std::vector<double> result = method(data, testx);
        double error = meanAbsoluteError(testy, result);
        std::cout << "Mean Absolute Error for " << name << " is " << error << std::endl;
        error = SMAPE(testy, result);
        std::cout << "SMAPE for " << name << " is " << error << std::endl;
        error = r2_score(testy, result);
        std::cout << "R2 score for " << name << " is " << error << std::endl << std::endl;
        // printValues(result);
    }

    return 0;
}