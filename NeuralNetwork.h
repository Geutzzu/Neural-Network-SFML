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

#include "DataPoint.h"
#include "LayerData.h"
#include "Cost.h"
#include "Activation.h"
#include "NetworkData.h"

class Layer;

#include "Layer.h"

using namespace std;

class NeuralNetwork {
private:
	std::vector<Layer> layers; /// the layers of the neural network

public:
	NeuralNetwork(const vector<int>& layerSizes);

	void initializeWeightsAndBiases(); /// initialize the weights and biases of the neural network

	vector<double> calculateOutputs(const vector<double>& inputs, const ActivationType& activation);

	vector<double> calculateNeuronOutputs(const vector<double>& inputs, int specificLayer, int activeNeuron, const ActivationType& activation); /// for neuron plots

	vector<double> calculateNeuronOutputsDiscretized(const vector<double>& inputs, int specificLayer, int activeNeuron, const ActivationType& activation);

	int classify(const vector<double>& inputs, const ActivationType& activation);

	double costFunction(const DataPoint& dataPoint, const ActivationType& activation, const CostType& costType); /// calculate the cost of the data point

	double costFunction(const set<DataPoint>& dataPoints, const ActivationType& activation, const CostType& costType); /// calculate the cost of the data points

	void updateGradientsForDataPoint(const DataPoint& dataPoint, const ActivationType& activationType, const CostType& costType);


	vector<double> calculateOutputSpecificValues(const vector<double>& expectedOutputs, const ActivationType& activationType, const CostType& costType); /// we need the expected outputs to calculate the specific values for backpropagation

	vector<double> calculateHiddenLayerOutputSpecificValues(const vector<double>& oldSpecificValues, int oldIndex, const ActivationType& activationType, const CostType& costType); /// we need the old specific values to calculate the new specific values

	void learn(vector<DataPoint> trainingBatch, double learningRate, double momentum); /// learn from a batch of data points

	void applyAllGradients(double learningRate, double momentum); /// apply the gradients to the weights and biases

	vector <DataPoint> classifyAll(const vector<DataPoint>& dataPoints, const ActivationType& activation); /// classify all the data points

	DataPoint classify(const DataPoint& dataPoint, const ActivationType& activation); /// classify a single data point



	/// geter for reference (ruins encapsulation)
	/// Layer& GetLayerRef(int index) { return this->layers[index]; }

	const LayerData& getLayerData(int index) const;

	const Layer& getLayer(int index) const;

	const vector<Layer>& getLayers() const;

	int getNumberLayers() const;

	int getLayerIndex(const Layer& layer) const;

};