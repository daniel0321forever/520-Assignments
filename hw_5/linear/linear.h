#ifndef LINEAR_H
#define LINEAR_H

#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct DataPoint {
    double x, y;       // Actual coordinates
    double predicted;  // Predicted y value from model

    DataPoint(double x_ = 0, double y_ = 0, double pred_ = 0)
        : x(x_), y(y_), predicted(pred_) {}
};

class LinearRegression {
    std::vector<DataPoint> data;  // Training data
    double slope;                  // m in y = mx + b
    double intercept;              // b in y = mx + b
    double rSquared;               // R² score

public:
    LinearRegression() : slope(0), intercept(0), rSquared(0) {}

    void addPoint(double x, double y) {
        data.push_back(DataPoint(x, y, 0));
    }

    void generateSyntheticData(int numPoints = 50, double trueSlope = 2.5,
                              double trueIntercept = 1.0, double noise = 0.5) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> xDis(0, 10);
        std::normal_distribution<> noiseDis(0, noise);

        for (int i = 0; i < numPoints; i++) {
            double x = xDis(gen);
            double y = trueSlope * x + trueIntercept + noiseDis(gen);
            addPoint(x, y);
        }

        std::cout << "Generated " << numPoints << " synthetic data points.\n";
        std::cout << "True model: y = " << trueSlope << "x + " << trueIntercept << "\n";
        std::cout << "Noise level: " << noise << "\n\n";
    }

    void fit();
    void calculateMetrics();
    double calculateMSE() const;
    double calculateMAE() const;
    double predict(double x) const;
    void saveAsImage(const std::string& filename) const;

    double getRSquared() const { return rSquared; }
};

#endif
