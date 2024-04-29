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
#include <random>

#include "NeuralNetwork.h"
#include "DataPoint.h"
#include "Layer.h"
#include "LayerData.h"
#include "Cost.h"
#include "Activation.h"


using namespace std;

class DataPoint //// class for the data points
{
private:
	vector<double> inputs; /// input values
	vector<double> outputs; /// output values

public:

	DataPoint() = default; /// default constructor


	DataPoint(const vector<double>& inputs, const vector<double>& outputs) /// constructor
	{
		this->inputs = inputs; /// set the inputs
		this->outputs = outputs; /// set the output
	}

	std::vector<DataPoint> GenerateDataPoints(int numPoints) {
		std::vector<DataPoint> dataPoints;
		dataPoints.reserve(numPoints);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0, 1.0);

		for (int i = 0; i < numPoints; ++i) {
			double x = dis(gen);
			double y = dis(gen);
			DataPoint dp;
			dp.inputs = { x, y };
			// Label points above the line y = x as {1, 0}, and points below the line as {0, 1}
			dp.outputs = (y > x) ? std::vector<double>{1, 0} : std::vector<double>{ 0, 1 };
			dataPoints.push_back(dp);
		}

		return dataPoints;
	}


	const vector<double>& GetInputs() const /// geter for the inputs
	{
		return this->inputs;
	}

	const vector<double>& GetOutputs() const /// geter for the outputs
	{
		return this->outputs;
	}

	void SetInputs(const vector<double>& inputs) /// setter for the inputs
	{
		this->inputs = inputs;
	}

	void SetOutputs(const vector<double>& outputs) /// setter for the outputs
	{
		this->outputs = outputs;
	}
};
