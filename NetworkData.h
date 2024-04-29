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
#include <vector>
#include "LayerData.h"




class NetworkData {
private:
	vector<LayerData> networkData; /// data for each layer
public:
	NetworkData(const vector<int>& layerSizes) {
		for (int i = 0; i < layerSizes.size(); i++) {
			this->networkData.push_back(LayerData(layerSizes[i], layerSizes[i + 1])); /// add the layer data to the network data
		}
	}

	const LayerData& GetLayerData(int index) const { return this->networkData[index]; }
	void SetLayerData(int index, const LayerData& layerData) { this->networkData[index] = layerData; }


};