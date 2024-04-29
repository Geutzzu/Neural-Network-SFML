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

	const vector<double>& GetInputs() const { return this->inputs; }
	const vector<double>& GetActivations() const { return this->activations; }
	const vector<double>& GetWeightedInputs() const { return this->weightedInputs; }

	void SetInputs(const vector<double>& inputs) { this->inputs = inputs; }
	void SetActivations(const vector<double>& activations) { this->activations = activations; }
	void SetWeightedInputs(const vector<double>& weightedInputs) { this->weightedInputs = weightedInputs; }

	void SetInput(int index, double value) { this->inputs[index] = value; }
	void SetActivation(int index, double value) { this->activations[index] = value; }
	void SetWeightedInput(int index, double value) { this->weightedInputs[index] = value; }

	double GetInput(int index) const { return this->inputs[index]; }
	double GetActivation(int index) const { return this->activations[index]; }
	double GetWeightedInput(int index) const { return this->weightedInputs[index]; }

	int GetNumberInputs() const { return this->inputs.size(); }
	int GetNumberOutputs() const { return this->activations.size(); }
};



