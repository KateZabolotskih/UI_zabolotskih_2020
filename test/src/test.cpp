//
// Created by Екатерина Заболотских on 10.10.2020.
//
#include "../include/ILogger.h"
#include "../include/IVector.h"



int main() {
    int a;
    double data[] = {1, 2, 3};
    ILogger * logger = ILogger::createLogger(&a);
    IVector * v1 = IVector::createVector(3, data, nullptr);
    return 0;
}