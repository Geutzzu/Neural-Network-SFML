#pragma once

#include "NeuralNetwork.h"
#include "LayerVisualizer.h"
#include "NetworkVisualizer.h"
#include "Neuron.h"
#include <set>
#include "Slider.h"
#include "Dropdown.h"
#include <map>
#include "Graph.h"
#include "StandardizationCalculator.h"
#include "NeuronPlot.h"
#include <variant>
#include <any>


class MainLoop {
private:

	RenderWindow window;
	NeuralNetwork network;
	variant<NetworkVisualizer<Neuron>, NetworkVisualizer<NeuronPlot>> networkVisualizer;
	TextureManager* textureManager;
	GameState gameState;
	set<DataPoint> dataPoints;
	map<DataPoint, CircleShape> circles;
	Graph costGraph;
	StandardizationCalculator statsX; /// not used now - no need for standardization
	StandardizationCalculator statsY; /// not used now

	/// hyperparameters
	int epoch;
	double learningRate;
	double momentum;
	ActivationType activationType;
	CostType costType;


	/// colors
	Color backgroundColor;
	Color buttonColor;
	Color hoverColor;
	Color displayColor;

	/// sizes
	int normalButtonWidth;
	int normalButtonHeight;

	/// buttons and sliders
	Button resetInputsButton;
	Button resetTrainingButton;
	Slider learningRateSlider;
	Slider momentumSlider;
	Dropdown classDropdown;
	Dropdown costDropdown;
	Dropdown activationDropdown;
	Button previousPointButton;
	Button nextPointButton;
	Button toggleHighlightMode;
	Button toggleVisualizeMode;

	/// Clocks
	Clock dataPointClock;

	/// misc
	bool dataSetEmpty;
	bool highlightMode;
	bool graphDisplayed;
	Color dataColor;
	vector<Color> classColors;
	int numberClasses;
	set<DataPoint>::iterator currentDataPoint;
	double maxCost;
	bool isAddingDataPoints;


	/// plot visualization
	bool visualizeMode;
	VertexArray pixels;
	int pixelSize;

	/// highlighting
	CircleShape highlight;
	Clock clock;

	/// views
	View plotView;
	View networkView;

	/// ctrl z / ctrl y
	stack<DataPoint> undoStack;
	stack<DataPoint> redoStack;
	CircleShape dataCircle;


	MainLoop();
	~MainLoop() = default;


public:

	MainLoop(const MainLoop& m) = delete;
	MainLoop& operator=(const MainLoop& m) = delete;

	static MainLoop* getInstance();

	/// initializing functions
	void initializeClassDropdown();
	void initializeCostDropdown();
	void initializeActivationDropdown();
	void computePlotPositions();

	/// functions for functionality
	void holdForDataPoint();
	void addDataPoint(Event& event);
	void updateHighlight();
	void updateNumberOfClasses();
	void drawRightHalfScreen();
	void visualizePlot();
	void eventHandler(sf::Event& event, bool& space); /// to change the bool space
	void zoomHandler();
	void trainingState();
	void resetInputs();
	void resetTraining();
	void toggleHighlight();
	void toggleVisualize();
	const double& getCostPercentage();

	/// undo redo
	void undo();
	void redo();

	/// standardization
	double calculateMeanX();
	double calculateMeanY();
	double calculateStdDevX();
	double calculateStdDevY();


	/// main loop
	void run();


};
