#pragma once

#include <vector>

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



class LayerData {
private:
	vector<double> inputs;
	vector<double> activations;
	vector<double> weightedInputs;
public:

	LayerData() = default;

	LayerData(int numberInputs, int numberOutputs);

	LayerData(const vector<double>& inputs, const vector<double>& activations, const vector<double>& weightedInputs);

	const vector<double>& getInputs() const { return this->inputs; }
	const vector<double>& getActivations() const { return this->activations; }
	const vector<double>& getWeightedInputs() const { return this->weightedInputs; }

	void setInputs(const vector<double>& inputs) { this->inputs = inputs; }
	void setActivations(const vector<double>& activations) { this->activations = activations; }
	void setWeightedInputs(const vector<double>& weightedInputs) { this->weightedInputs = weightedInputs; }

	void setInput(int index, double value) { this->inputs[index] = value; }
	void setActivation(int index, double value) { this->activations[index] = value; }
	void setWeightedInput(int index, double value) { this->weightedInputs[index] = value; }

	const double& getInput(int index) const { return this->inputs[index]; }
	const double& getActivation (int index) const { return this->activations[index]; }
	const double& getWeightedInput(int index) const { return this->weightedInputs[index]; }

	int getNumberInputs() const { return this->inputs.size(); }
	int getNumberOutputs() const { return this->activations.size(); }
};



