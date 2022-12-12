#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <unordered_map>
#include <fstream>
#include <chrono>

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
    // auto f = [] (double x) { return std::sin(std::pow(x, 3/2)) / std::log(x);};
    // auto f = [] (double x) {return std::pow(x, 3);};
    auto f = [] (double x) {return x;};
    int pointsAmount = 100;
    double lb = 1.5;
    double rb = 10;
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

void getValuesFromFile(std::vector<Point>& data, std::vector<double>& testx, std::vector<double>& testy){
    std::ifstream fin;
    fin.open("values.txt");
    int counter = 1;
    Point p;
    p.x = 0;
    while(!fin.eof()){
        fin >> p.y;
        if (counter % 5 == 0){
            testx.push_back(p.x);
            testy.push_back(p.y);
        } else 
            data.push_back(p);
        
        p.x++;
        counter++;
        // std::cout<<counter<<std::endl;
    }

    fin.close();
}

int main(){
    std::unordered_map<std::string, Method> interpolationMethods {{"Newton",Newton}, {"Linear", Linear}, {"Lagrange", Lagrange}};
    std::vector<Point> data;
    std::vector<double> testx;
    std::vector<double> testy;
    
    gen(data, testx, testy);
    
    // getValuesFromFile(data, testx, testy);
    printValues(testy);
    
    std::cout<<"Methods"<<std::endl;
    for (auto [name, method]: interpolationMethods){
        auto start = std::chrono::system_clock::now();
        std::vector<double> result = method(data, testx);
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout<<name<<" duration is: " << (int)duration << std::endl;
        double error = meanAbsoluteError(testy, result);
        std::cout << "Mean Absolute Error for " << name << " is " << error << std::endl;
        error = SMAPE(testy, result);
        std::cout << "SMAPE for " << name << " is " << error << std::endl;
        error = r2_score(testy, result);
        std::cout << "R2 score for " << name << " is " << error << std::endl << std::endl;
        printValues(result);
    }

    int polinomDegree = 1;
    double error = -__DBL_MAX__;
    std::vector<double> result = LSM(data, testx, polinomDegree);
    while(error <= r2_score(testy, result)){
        error = r2_score(testy, result);
        polinomDegree++;
        result.clear();
        result = LSM(data, testx, polinomDegree);
    }

    result.clear();
    polinomDegree--;
    auto start = std::chrono::system_clock::now();
    result = LSM(data, testx, polinomDegree);
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout<<"LSM duration is: " << (int)duration << std::endl;
    error = meanAbsoluteError(testy, result);
    std::cout << "Polinom degree is: " <<polinomDegree<<std::endl;
    std::cout << "Mean Absolute Error for LSM is " << error << std::endl;
    error = SMAPE(testy, result);
    std::cout << "SMAPE for LSM is " << error << std::endl;
    error = r2_score(testy, result);
    std::cout << "R2 score for LSM is " << error << std::endl << std::endl; 
    printValues(result);
    return 0;
}