#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>

#include "NeuralNetwork.h"
#include "DataPoint.h"
#include "Layer.h"
#include "LayerData.h"
#include "Cost.h"
#include "Activation.h"


using namespace std;

enum class CostType {
	MeanSquareError,
	CrossEntropy
};

class Cost {
private:
    CostType costType;

public:
    Cost(CostType costType) : costType(costType) {}

    double Function(const std::vector<double>& predictedOutputs, const std::vector<double>& expectedOutputs) const {
        double cost = 0;
        if (costType == CostType::MeanSquareError) {
            for (size_t i = 0; i < predictedOutputs.size(); i++) {
                double error = predictedOutputs[i] - expectedOutputs[i];
                cost += error * error;
            }
            return 0.5 * cost;
        }
        else if (costType == CostType::CrossEntropy) {
            for (size_t i = 0; i < predictedOutputs.size(); i++) {
                double x = predictedOutputs[i];
                double y = expectedOutputs[i];
                double v = (y == 1) ? -std::log(x) : -std::log(1 - x);
                cost += std::isnan(v) ? 0 : v;
            }
            return cost;
        }
        else {
            // Handle unsupported cost type
            return 0.0;
        }
    }

    double Derivative(double predictedOutput, double expectedOutput) const {
        if (costType == CostType::MeanSquareError) {
            return predictedOutput - expectedOutput;
        }
        else if (costType == CostType::CrossEntropy) {
            double x = predictedOutput;
            double y = expectedOutput;
            if (x == 0 || x == 1) {
                return 0;
            }
            return (-x + y) / (x * (x - 1));
        }
        else {
            // Handle unsupported cost type
            return 0.0;
        }
    }
};

