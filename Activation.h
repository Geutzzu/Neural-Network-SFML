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


template <ActivationType T>
class Activation;

template <>
class Activation<ActivationType::Sigmoid>
{
public:
	double Activate(const vector<double>& inputs, int index) const
	{
		return 1.0 / (1 + exp(-inputs[index]));
	}

	double Derivative(const vector<double>& inputs, int index) const
	{
		double a = Activate(inputs, index);
		return a * (1 - a);
	}
	double Activate(double input) const
	{
		return 1.0 / (1 + exp(-input));
	}

	double Derivative(double input) const
	{
		double a = Activate(input);
		return a * (1 - a);
	}
};

template <>
class Activation<ActivationType::TanH>
{
public:
	double Activate(const vector<double>& inputs, int index) const
	{
		double e2 = exp(2 * inputs[index]);
		return (e2 - 1) / (e2 + 1);
	}

	double Derivative(const vector<double>& inputs, int index) const
	{
		double e2 = exp(2 * inputs[index]);
		double t = (e2 - 1) / (e2 + 1);
		return 1 - t * t;
	}

	double Activate(double input) const
	{
		double e2 = exp(2 * input);
		return (e2 - 1) / (e2 + 1);
	}

	double Derivative(double input) const
	{
		double e2 = exp(2 * input);
		double t = (e2 - 1) / (e2 + 1);
		return 1 - t * t;
	}
};

template <>
class Activation<ActivationType::ReLU>
{
public:
	double Activate(const vector<double>& inputs, int index) const
	{
		return max(0.0, inputs[index]);
	}

	double Derivative(const vector<double>& inputs, int index) const
	{
		return (inputs[index] > 0) ? 1 : 0;
	}

	double Activate(double input) const
	{
		return max(0.0, input);
	}

	double Derivative(double input) const
	{
		return (input > 0) ? 1 : 0;
	}
};

template <>
class Activation<ActivationType::SiLU>
{
public:
	double Activate(const vector<double>& inputs, int index) const
	{
		return inputs[index] / (1 + exp(-inputs[index]));
	}

	double Derivative(const vector<double>& inputs, int index) const
	{
		double sig = 1 / (1 + exp(-inputs[index]));
		return inputs[index] * sig * (1 - sig) + sig;
	}

	double Activate(double input) const
	{
		return input / (1 + exp(-input));
	}

	double Derivative(double input) const
	{
		double sig = 1 / (1 + exp(-input));
		return input * sig * (1 - sig) + sig;
	}
};

template <>
class Activation<ActivationType::Softmax>
{
public:
	double Activate(const vector<double>& inputs, int index) const
	{
		double expSum = 0;
		for (double input : inputs)
		{
			expSum += exp(input);
		}

		return exp(inputs[index]) / expSum;
	}

	double Derivative(const vector<double>& inputs, int index) const
	{
		double expSum = 0;
		for (double input : inputs)
		{
			expSum += exp(input);
		}

		double ex = exp(inputs[index]);

		return (ex * expSum - ex * ex) / (expSum * expSum);
	}

};

template <ActivationType T>
Activation<T> GetActivationFromType()
{
	return Activation<T>(); /// return the activation function
}


