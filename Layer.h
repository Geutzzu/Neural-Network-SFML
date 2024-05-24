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
#include <sstream>

#include "NeuralNetwork.h"
#include "DataPoint.h"
#include "LayerData.h"
#include "Cost.h"
#include "Activation.h"



using namespace std;
using namespace sf;


class Layer {
private:
	vector<double> weights;
	vector<double> biases;
	int numberInputs;
	int numberOutputs;

	vector <double> costGradientWeights; //// gradient for the weights - each weight will have a gradient pointing towards a local minimum
	vector <double> costGradientBiases; //// gradient for the biases - each bias will have a gradient pointing towards a local minimum

	vector<double> prevDeltaWeights; /// previous delta weights
	vector<double> prevDeltaBiases; /// we use them so we can store the previous values so we can calculate the momentum

	LayerData layerData; /// data for the layer - inputs, weighted inputs, activations

public:

	Layer(int numberInputs, int numberOutputs);

	void initializeWeightsAndBiases(); /// initialize the weights and biases of the layer

	vector<double> calculateOutputs(const vector<double>& inputs, const ActivationType& activationType);
	void updateGradients(const vector<double>& specificValues);
	void applyGradients(double learningRate, double momentum);
	/// vector<double> CalculateOutputs(const vector<double>& inputs);

	double getWeight(int inputIndex, int outputIndex) const { return this->weights[inputIndex + outputIndex * this->numberInputs]; }
	double getBias(int outputIndex) const { return this->biases[outputIndex]; }

	void setWeight(int inputIndex, int outputIndex, double value) { this->weights[inputIndex + outputIndex * this->numberInputs] = value; }
	void setBias(int outputIndex, double value) { this->biases[outputIndex] = value; }

	/// geter for reference (ruins encapsulation)
	double& getWeightRef(int inputIndex, int outputIndex) { return this->weights[inputIndex + outputIndex * this->numberInputs]; }
	double& getBiasRef(int outputIndex) { return this->biases[outputIndex]; }

	const vector <double>& getWeights() const { return this->weights; }
	const vector <double>& getBiases() const { return this->biases; }

	const LayerData& getLayerData() const { return this->layerData; }
	const int getNumberInputs() const { return this->numberInputs; }
	const int getNumberOutputs() const { return this->numberOutputs; }
	const double getOutput(int index) const { return this->layerData.getActivation(index); }
};