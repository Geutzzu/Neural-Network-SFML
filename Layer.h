


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

class Layer {
private:
	vector<double> weights;
	vector<double> biases;
	int numberInputs;
	int numberOutputs;

	vector <double> costGradientWeights; //// gradient for the weights - each weight will have a gradient pointing towards a local minimum
	vector <double> costGradientBiases; //// gradient for the biases - each bias will have a gradient pointing towards a local minimum

	LayerData layerData;

public:

	Layer(int numberInputs, int numberOutputs);

	void InitializeWeightsAndBiases(); /// initialize the weights and biases of the layer

	vector<double> CalculateOutputs(const vector<double>& inputs);
	void UpdateGradients(const vector<double>& specificValues);
	void ApplyGradients(double learningRate);
	/// vector<double> CalculateOutputs(const vector<double>& inputs);

	double GetWeight(int inputIndex, int outputIndex) const { return this->weights[inputIndex + outputIndex * this->numberInputs]; }
	double GetBias(int outputIndex) const { return this->biases[outputIndex]; }

	void SetWeight(int inputIndex, int outputIndex, double value) { this->weights[inputIndex + outputIndex * this->numberInputs] = value; }
	void SetBias(int outputIndex, double value) { this->biases[outputIndex] = value; }

	/// geter for reference (ruins encapsulation)
	double& GetWeightRef(int inputIndex, int outputIndex) { return this->weights[inputIndex + outputIndex * this->numberInputs]; }
	double& GetBiasRef(int outputIndex) { return this->biases[outputIndex]; }

	const LayerData& GetLayerData() const { return this->layerData; }

};

