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


void printValues(std::vector<double> values){
    for(auto i: values)
        std::cout<<i<<"\n";
}

void getValuesFromFile(std::string input_file, std::vector<Point>& data, std::vector<double>& testx, std::vector<double>& testy){
    data.clear();
    testx.clear();
    testy.clear();
    std::ifstream fin;
    fin.open(input_file);
    Point p;
    int amount(0);
    fin >> amount;
    data.reserve(amount);
    for (int i(0); i < amount; ++i){
        fin >> p.x >> p.y;
        data.push_back(p);
    }
    fin >> amount;
    testx.reserve(amount);
    testy.reserve(amount);
    for (int i(0); i< amount; ++i){
        fin >> p.x >> p.y;
        testx.push_back(p.x);
        testy.push_back(p.y);
    }

    fin.close();
}

void passValuesToFile(std::string output, std::vector<double>& result, double mae, double smape, double r2_score, int64_t time, int amount){
    
    std::ofstream fout;
    fout.open(output, std::ios::app);
    for (auto& i: result)
        fout << i << " ";
    fout << mae << ' ' << smape << ' ' << r2_score << ' ' << time << ' ' << amount << '\n';
    fout.close();
}

int selectPolinomialDegree(std::vector<Point>& data, std::vector<double>& testx, std::vector<double>& testy){
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
    return --polinomDegree;
}

std::vector<Point> selectBetterPoints(Method method, const std::vector<Point>& data, std::vector<double>& testy, std::vector<double>& testx){
    std::vector<Point> dataSelect = data;
    std::vector<Point> copyDataSelect;
    double error = -__DBL_MAX__;
    std::vector<double> result = method(dataSelect, testx);
    int counter(2);
    while((error <= r2_score(testy, result))&& (counter < data.size())){
        error = r2_score(testy, result);
        copyDataSelect.clear();
        copyDataSelect = dataSelect;
        dataSelect.clear();
        dataSelect.reserve(data.size()/counter);
        for (int i(0); i < data.size(); ++i){
            if (i % counter == 1) {
                dataSelect.push_back(data[i]);
            }
        };
        counter++;
        result.clear();
        result = method(dataSelect, testx);
    }
    result.clear();
    return copyDataSelect;
}

int main(){
    std::unordered_map<std::string, Method> interpolationMethods { {"Newton", Newton}, {"Lagrange", Lagrange}}; // 
    std::vector<Point> data;
    std::vector<double> testx;
    std::vector<double> testy;
    
    std::vector<std::string> files{"data_1.txt", "data_2.txt", "data_3.txt"};
    for (auto file: files){
        getValuesFromFile(file, data, testx, testy);

        // std::cout<<"Methods"<<std::endl;
        std::ofstream fout(file.substr(0, file.length()-4) + "_result.txt", std::ios::out);
        for (auto& i: testx)
            fout << i <<' ';
        fout << ' ' << ' ' << " \n";
        for (auto& i: testy)
            fout << i <<' ';
        fout << ' ' << ' ' << " \n";
        fout.close();

        std::string output = file.substr(0, file.length() - 4) + "_result.txt";

        auto start = std::chrono::system_clock::now();
        std::vector<double> result = Linear(data, testx);
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        passValuesToFile(output, result, meanAbsoluteError(testy, result), 
                            SMAPE(testy, result), r2_score(testy, result), 
                            duration, data.size());

        for (auto [name, method]: interpolationMethods){
            std::vector<Point> goodPoints = selectBetterPoints(method, data, testy, testx);
            start = std::chrono::system_clock::now();
            result = method(goodPoints, testx);
            end = std::chrono::system_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            passValuesToFile(output, result, meanAbsoluteError(testy, result), 
            SMAPE(testy, result), r2_score(testy, result), duration, 
            goodPoints.size());
        }

        int polinomDegree = selectPolinomialDegree(data, testx, testy);
        start = std::chrono::system_clock::now();
        result = LSM(data, testx, polinomDegree);
        end = std::chrono::system_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        passValuesToFile(output, result, meanAbsoluteError(testy, result), 
                            SMAPE(testy, result), r2_score(testy, result), 
                            duration, data.size());
    }
    return 0;
}