#pragma once
#include <iomanip>

namespace lab4
{
    double computeFunction(double x);
    double integrateLeft(int divisions, double start, double end);
    double integrateLeftParallel(int divisions, double start, double end);
    double integrateRight(int divisions, double start, double end);
    double integrateRight(int divisions, double start, double end);
    double integrateRightParallel(int divisions, double start, double end);
    double integrateMid(int divisions, double start, double end);
    double integrateMidParallel(int divisions, double start, double end);
    double integrateTrapezoid(int divisions, double start, double end);
    double integrateTrapezoidParallel(int divisions, double start, double end);
    double integrateSimpson(int divisions, double start, double end);
    double integrateSimpsonParallel(int divisions, double start, double end);
    double calculateIntegral(int mode, int divisions, double start, double end);
    double calculateIntegralParallel(int mode, int divisions, double start, double end);
    void start();
}