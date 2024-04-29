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




LayerData::LayerData(int numberInputs, int numberOutputs) {
	this->inputs.resize(numberInputs);
	this->activations.resize(numberOutputs);
	this->weightedInputs.resize(numberOutputs);
}

LayerData::LayerData(const vector<double>& inputs, const vector<double>& activations, const vector<double>& weightedInputs) {
	this->inputs = inputs;
	this->activations = activations;
	this->weightedInputs = weightedInputs;
}