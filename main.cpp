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
using namespace sf;

class Slider {
private:
    sf::RectangleShape track;
    sf::RectangleShape handle;
    sf::Font font;
    sf::Text text;
	string name;
    double& value; // reference to a float
    bool isDragging;

public:
    Slider(float x, float y, float width, float height, double& value, string name)
        : value(value) // initialize the reference
    {

		this->name = name;

        track.setPosition(x, y);
        track.setSize(sf::Vector2f(width, height));
        track.setFillColor(sf::Color::White);

        handle.setSize(sf::Vector2f(20, height));
        handle.setFillColor(sf::Color::Red);
        handle.setPosition(x, y);

        if (!font.loadFromFile("Resources/Roboto-Black.ttf")) { // replace with path to your font file
            cout << "Error loading font\n";
        }
        cout << "Font loaded\n";
        text.setFont(font);
        text.setCharacterSize(24); // in pixels
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y - text.getCharacterSize() - 5);

        isDragging = false;
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (handle.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                isDragging = true;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            isDragging = false;
        }
        else if (event.type == sf::Event::MouseMoved) {
            if (isDragging) {
                float newX = event.mouseMove.x - handle.getSize().x / 2;
                newX = std::max(newX, track.getPosition().x);
                newX = std::min(newX, track.getPosition().x + track.getSize().x - handle.getSize().x);
                handle.setPosition(newX, handle.getPosition().y);

				value = ((newX - track.getPosition().x) / (track.getSize().x - handle.getSize().x)) * 6 - 3;
				text.setString(name + ":" + to_string(value));
            }
        }
    }

	void setText(string str) {
		this->text.setString(str);
	}

    void draw(sf::RenderWindow& window) {
        window.draw(track);
        window.draw(handle);
        window.draw(text);
    }
};


/*
/// Weight values connecting each input to the first output
double weight_1_1, weight_2_1;

/// Weight values connecting each input to the second output
double weight_1_2, weight_2_2;

/// Bias values for the first and second outputs
double bias_1, bias_2;

/// Classifies the given input as either 0 or 1
int Classify(double input_1, double input_2)
{
	/// Calculate the first output
	double output_1 = input_1 * weight_1_1 + input_2 * weight_2_1 + bias_1;

	/// Calculate the second output
	double output_2 = input_1 * weight_1_2 + input_2 * weight_2_2 + bias_2;

	/// Print the outputs
	///cout << "Output 1: " << output_1 << endl;
	///cout << "Output 2: " << output_2 << endl;

	/// Classify the input
	if (output_1 > output_2)
	{
		///cout << "Classified as 0" << endl;
		return 0;
	}
	else
	{
		///cout << "Classified as 1" << endl;
		return 1;
	}
}
*/




enum class GameState {
    InputingData,
    Training
};


void DrawRightHalfScreen(sf::RenderWindow& window, sf::Color color) {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(window.getSize().x / 2, window.getSize().y));
    rectangle.setPosition(window.getSize().x / 2, 0);
    rectangle.setFillColor(color);

    window.draw(rectangle);
}

void Vizualize(RenderWindow& window, NeuralNetwork& network)
{

    VertexArray container(Points, 200 * 200);
    for (int i = 0; i < 200 * 200; i++) {
        container[i].position = Vector2f(i % 200 * 2, i / 200 * 2) + Vector2f(window.getSize().x / 2, 0);
        container[i].color = Color::White;
    }
    window.clear();
    double input_1, input_2;
    int classification;

    for (int x = 0; x < 200; x++) {
        for (int y = 0; y < 200; y++) {
            input_1 = x / 200.0;
            input_2 = y / 200.0;
            classification = network.Classify({ input_1, input_2 });
            Vertex& pixel = container[x + y * 200];
            pixel.color = classification == 0 ? Color::Red : Color::Blue;
        }
    }

}


void Train(NeuralNetwork& network, const vector<DataPoint>& dataPoints, int epochs, double learningRate, RenderWindow& window)
{

    for (int epoch = 0; epoch < epochs; epoch++) {
        network.Learn(dataPoints, learningRate);
        Vizualize(window, network);
    }
}




void InitializeTrainingData(NeuralNetwork& network, vector<DataPoint>& dataPoints, RenderWindow& window, vector<CircleShape> circles, GameState& gameState)
{
	gameState = GameState::InputingData;

	network.InitializeWeightsAndBiases(); /// initialize the weights and biases of the neural network

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.x > window.getSize().x / 2 && gameState == GameState::InputingData) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    circles.push_back(CircleShape(10));
                    circles.back().setFillColor(Color::Blue);
                    circles.back().setPosition(event.mouseButton.x, event.mouseButton.y);
                    dataPoints.push_back(DataPoint({ (event.mouseButton.x - window.getSize().x / 2) / 1.0, event.mouseButton.y / 1.0 }, { 1, 0 }));
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    circles.push_back(CircleShape(10));
                    circles.back().setFillColor(Color::Red);
                    circles.back().setPosition(event.mouseButton.x, event.mouseButton.y);
                    dataPoints.push_back(DataPoint({ (event.mouseButton.x - window.getSize().x / 2) / 1.0, event.mouseButton.y / 1.0 }, { 0, 1 }));
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                gameState = GameState::Training;
            }
        }

        window.clear();


        DrawRightHalfScreen(window, Color::White);

        for (int i = 0; i < circles.size(); i++) {
            window.draw(circles[i]);
        }


		if (gameState == GameState::Training)
        {
            for (int i = 0; i < dataPoints.size(); i++) { /// for each data point
               network.UpdateGradientsForDataPoint(dataPoints[i]); /// update the gradients for the data point
            }
            network.ApplyAllGradients(0.01 / dataPoints.size()); /// apply the gradients to the weights and biases

            Vizualize(window, network);
		}

       

        window.display();

    }



}




int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Slider Example");
	/// using sliders to modify the weights and biases

	///Visualize(window);


	/// Seting the data points
    DataPoint point;
    NeuralNetwork network({ 2, 4, 2 });
	vector<CircleShape> circles;
    GameState gameState = GameState::InputingData;

	/// InitializeTrainingData(network, dataPoints, window, circles, gameState);
    
	vector<DataPoint> dataPoints = point.GenerateDataPoints(1000);


	InitializeTrainingData(network, dataPoints, window, circles, gameState);


	/// data sample 
	/*

	network.InitializeWeightsAndBiases(); /// initialize the weights and biases of the neural network

    for (int epoch = 0; epoch < 2000; epoch++)
    {
        for (int i = 0; i < dataPoints.size(); i++) { /// for each data point
            network.UpdateGradientsForDataPoint(dataPoints[i]); /// update the gradients for the data point
        }
        network.ApplyAllGradients(0.001); /// apply the gradients to the weights and biases


    }
  
    vector <DataPoint> results;

	results = network.ClassifyAll(dataPoints);

	cout << "Data points:" << endl;

	for (int i = 0; i < dataPoints.size(); i++) {

		double first = results[i].GetOutputs()[0] > 0.5 ? 1 : 0;
		double second = results[i].GetOutputs()[1] > 0.5 ? 1 : 0;

		results[i].SetOutputs({ first, second });

		cout << "Point: " << i << " Expected: " << dataPoints[i].GetOutputs()[0] << " " << dataPoints[i].GetOutputs()[1] << " Classified: " << results[i].GetOutputs()[0] << " " << results[i].GetOutputs()[1] << endl;
	}

	point = DataPoint({ 0.5, 0.5 }, { 1, 0 });

 

	/// for each circle that we place on the screen we create a data point

	
    */

	return 0;
}