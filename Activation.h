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

/// Activation functions
enum class ActivationType
{
	Sigmoid,
	TanH,
	ReLU,
	SiLU,
	Softmax
};


class Activation {
private:
    ActivationType activationType;

public:
    Activation(ActivationType activationType) : activationType(activationType) {}

    double Activate(double value) const {
        switch (activationType) {
        case ActivationType::Sigmoid:
            return 1.0 / (1 + exp(-value));
        case ActivationType::TanH: {
            double e2 = exp(2 * value);
            return (e2 - 1) / (e2 + 1);
        }
        case ActivationType::ReLU:
            return max(0.0, value);
        case ActivationType::SiLU:
            return value / (1 + exp(-value));
        default:
            // Handle unsupported activation type
            return 0.0;
        }
    }


    double Derivative(double value) const {
        switch (activationType) {
        case ActivationType::Sigmoid: {
            double a = Activate(value);
            return a * (1 - a);
        }
        case ActivationType::TanH: {
            double e2 = exp(2 * value);
            double t = (e2 - 1) / (e2 + 1);
            return 1 - t * t;
        }
        case ActivationType::ReLU:
            return (value > 0) ? 1 : 0;
        case ActivationType::SiLU: {
            double sig = 1 / (1 + exp(-value));
            return value * sig * (1 - sig) + sig;
        }
        default:
            // Handle unsupported activation type
            return 0.0;
        }
    }

    double Activate(const vector<double>& inputs, int index) const {
        double input = inputs[index];
        switch (activationType) {
        case ActivationType::Sigmoid:
            return 1.0 / (1 + exp(-input));
        case ActivationType::TanH: {
            double e2 = exp(2 * input);
            return (e2 - 1) / (e2 + 1);
        }
        case ActivationType::ReLU:
            return max(0.0, input);
        case ActivationType::SiLU:
            return input / (1 + exp(-input));
        case ActivationType::Softmax: {
            double expSum = 0;
            for (double input : inputs) {
                expSum += exp(input);
            }
            double expInput = exp(inputs[index]);
            return expInput / expSum;
        }
        default:
            // Handle unsupported activation type
            return 0.0;
        }
    }

    double Derivative(const vector<double>& inputs, int index) const {
        double input = inputs[index];
        switch (activationType) {
        case ActivationType::Sigmoid: {
            double a = Activate(inputs, index);
            return a * (1 - a);
        }
        case ActivationType::TanH: {
            double e2 = exp(2 * input);
            double t = (e2 - 1) / (e2 + 1);
            return 1 - t * t;
        }
        case ActivationType::ReLU:
            return (input > 0) ? 1 : 0;
        case ActivationType::SiLU: {
            double sig = 1 / (1 + exp(-input));
            return input * sig * (1 - sig) + sig;
        }
        case ActivationType::Softmax: {
            double expSum = 0;
            for (double input : inputs) {
                expSum += exp(input);
            }
            double expInput = exp(inputs[index]);
            return (expInput * expSum - expInput * expInput) / (expSum * expSum);
        }
        default:
            // Handle unsupported activation type
            return 0.0;
        }
    }
};



