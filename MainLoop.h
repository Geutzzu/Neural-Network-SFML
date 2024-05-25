#pragma once

#include <fstream>
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
#include "ThreadPool.h"


class MainLoop {
private:

	RenderWindow window;
	NeuralNetwork network;
	variant<NetworkVisualizer<Neuron>, NetworkVisualizer<NeuronPlot>> networkVisualizer;
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
	Button resetOrStartTrainingButton;
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
	bool isVisualizing;
	bool isDiscretized;

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

	/// thread pool
	ThreadPool threadPool;

	

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
	void visualizePlotDiscretized();
	void eventHandler(sf::Event& event, bool& space); /// to change the bool space
	void zoomHandler();
	void trainingState();
	void resetInputs();
	void resetOrStartTraining();
	void startTraining();
	void stopTraining();
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

	/// rgb to hsv
	void rgbToHsv(int r, int g, int b, double* h, double* s, double* v);
	void hsvToRgb(double h, double s, double v, int* r, int* g, int* b);

	/// read and write to file ( ctrl + s / ctrl + o )
	void writeToFile(const string& filename);
	void readFromFile(const string& filename);

	/// main loop
	void run();


};
