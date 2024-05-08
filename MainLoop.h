#pragma once

#include "NeuralNetwork.h"
#include "LayerVisualizer.h"
#include "NetworkVisualizer.h"
#include "Neuron.h"
#include <set>
#include "Slider.h"
#include "Dropdown.h"
#include <map>

class MainLoop {
private:

	RenderWindow window;
	NeuralNetwork network;
	NetworkVisualizer<Neuron> networkVisualizer;
	TextureManager* textureManager;
	GameState gameState;
	set<DataPoint> dataPoints;
	map<DataPoint, sf::CircleShape> circles;

	/// buttons and sliders
	Button resetInputsButton;
	Button resetTrainingButton;
	Slider learningRateSlider;
	Dropdown classDropdown;
	Button previousPointButton;
	Button nextPointButton;

	/// parameters
	int epoch;
	double learningRate;

	/// misc
	bool dataSetEmpty;
	Color dataColor;
	vector<Color> classColors;
	int numberClasses;
	set<DataPoint>::iterator currentDataPoint;


	/// highlighting
	CircleShape highlight;
	Clock clock;

	/// views
	sf::View plotView;
	sf::View networkView;


	MainLoop();

public:

	MainLoop(const MainLoop& m) = delete;
	MainLoop& operator=(const MainLoop& m) = delete;

	static MainLoop* getInstance();

	/// initializing functions
	void initializeClassDropdown();

	/// functions for functionality
	void updateHighlight();
	void updateNumberOfClasses();
	void drawRightHalfScreen();
	void visualizePlot();
	void eventHandler(sf::Event& event, bool& space); /// to change the bool space
	void zoomHandler();
	void trainingState();
	void resetInputs();
	void resetTraining();


	/// main loop
	void run();


};
