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
#include "NetworkData.h"


using namespace std;

class NeuralNetwork {
private:
	vector<Layer> layers;

public:
	NeuralNetwork(const vector<int>& layerSizes);

	void InitializeWeightsAndBiases(); /// initialize the weights and biases of the neural network

	vector<double> CalculateOutputs(const vector<double>& inputs);

	int Classify(const vector<double>& inputs);

	double CostFunction(const DataPoint& dataPoint); /// calculate the cost of the data point

	double CostFunction(const vector<DataPoint>& dataPoints); /// calculate the cost of the data points

	void UpdateGradientsForDataPoint(const DataPoint& dataPoint);

	vector <double> CalculateOutputSpecificValues(const vector<double>& expectedOutputs); /// we need the expected outputs to calculate the specific values for backpropagation

	vector <double> CalculateHiddenLayerOutputSpecificValues(const vector<double>& oldSpecificValues, int oldIndex); /// we need the old specific values to calculate the new specific values

	void Learn(vector<DataPoint> trainingBatch, double learningRate); /// learn from a batch of data points

	void ApplyAllGradients(double learningRate); /// apply the gradients to the weights and biases

	vector <DataPoint> ClassifyAll(const vector<DataPoint>& dataPoints); /// classify all the data points



	/// geter for reference (ruins encapsulation)
	/// Layer& GetLayerRef(int index) { return this->layers[index]; }
	
	const LayerData& GetLayerData(int index) const { return this->layers[index].GetLayerData(); }
};
