# Artificial Neural Network with SFML

## Description
Simple artificial neural network made in C++ that uses the SFML library for visualizing both the training process and the network itself. It was inspired by TensorFlow's neural network demo for binary classification. You can check it out here: https://playground.tensorflow.org. The project was motivated by the desire to better understand how a neural network works by building it from scratch without using any libraries. I also consider it a great learning tool for anybody that wants to pick up machine learning and wants a more visual introduction on how an artificial neural network operates.

## Demonstration

- ### Example for binary classification

![2024-05-26 19-13-30](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/e0afcc43-4b44-4a9d-be71-6483402a8c86)

- ### Example for multi-class classification using Sigmoid

![2024-05-28 01-26-09](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/7900f2c9-5590-4887-bc07-60f299d80472)

- ### Example for multi-class classification using TanH

![2024-05-28 02-08-01 (1)](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/5ec1e1cf-4c37-4926-8569-fe2fbdff8031)

## Installation
The repository contains all the files necessary to run the project, but it requires Visual Studio Community with the C++ add-ons to work. After cloning, it is as simple as opening the solution file and pressing run (version used: 2.6.1, 64-bit with C++ 17).

## What it does
We will train a neural network to learn multi-class classification. You will be able to input data points in the 2D plot represented by the right half of the screen. Based on the number of output neurons you will be able to choose between the colors corresponding to each output. In the inputting data state you will be able to add as many data points as you like with the label indicated by the color you have selected. After you are done inputting data and you are pleased with the network architecture and hyperparameters, you can then start the training process. Inside the training state you can fiddle with different visualizations (such as neuron views, data point highlighting, interpolating between colors etc.).

## What everything is
### Base neuron visualization
- you can see the activation value of each neuron (the number inside the neuron)
- the lines connecting the neurons represent the weights (if the line is thicker the value is higher, blue representing positive values and red representing negative ones)
- the values you see on the screen correspond to the data point that is referred by the "highlight mode" at any given time (at the top left corner otherwise)
- the first layer's values are the coordinates of the data points normalized between 0 and 1

![image](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/976076f6-1363-41c0-958b-5dd3279e0c8e)

### Neuron plot view
- you can see what each neuron picks up by rendering a plot of the output if only that specific neuron is active (i.e. you set everything else from its layer to 0 the exception being the output layer)
- gives interesting insights on how the network actually learns step-by-step

![image](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/c1b9a0b0-cf1c-4092-ae4b-a1b68221df0b)

### Loss and epochs
- the text on the top left corner represents the loss (error) of the model or "accuracy" (0 meaning it has no loss and it has fitted to the data properly, 1 meaning it got everything wrong and it doesn't know what it's doing)
- it has a little graph that changes based on the value of the loss
- the epochs text shows how many times the network has went through the dataset at a specific time
- in ML terms it indicates that the model has taken a gradient step to the minimum of the function for each data point (the code performs full batch gradient descent)

### Hyperparameters
- you can change the learning rate and the momentum via sliders
- via the dropdowns on the left you can change the loss function or the activation function (from experience it yields the best results with cross entropy + sigmoid)

![image](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/96395262-fa88-4ab9-841f-c4146498e359)

### Class dropdown
- from this dropdown you can change the label of each data point that you place on the plot

![image](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/0f60aa00-c4b8-428c-ad7e-02f173b3baaf)

### Inputting data
- after you have picked a label you can either click or hold to add data points to the plot
- you can add as many as you like but keep in mind that there is no data preprocessing so what you input will be the training data
- you can also Ctrl z / Ctrl y (undo / redo) the data points that you input and you can also save to a file and load from it the dataset (Ctrl s / Ctrl o)

<p align="center">
  <img src="https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/1d497b2f-5223-4c95-8116-ddbb2cabc0ae" />
</p>

## How it works

### The plot visualizations (both for the neurons and the main one)
- the plots are made by predicting every n-th pixel using the network (value dictated by how detailed you want the visualization to be) and drawing on the screen the respective color
- for each pixel, in order to reduce the number of draw calls per frame, I store each output in a vertex array of triangles and each "rectangle" on the screen that is predicted consists of two triangles placed one on top of another
- using this method I am able to reduce the number of draw calls to one per frame
```cpp
this->pixels = VertexArray(Triangles, width * height * 6); // 6 vertices for each pixel (2 triangles)
```
- the interpolation is done by calculating the weighted sum of the labels in linear RGB (normal rgb creates abnormal colors when interpolating)

### The network itself
- in short, it uses mathematics
- forward propagation works by going layer to layer and calculating the outputs of each one (given an activation and some inputs, nothing too complicated here)
- backpropagation is done by calculating the gradient for each weight and bias in respect to the cost function, and changing their respective values by a certain amount towards the correct value (the one dictated by minimizing the cost function)
- there are a lot of resources going in depth into the math and formulas of this process and I will not do it here.
- my network uses the chain rule to calculate the partial derivative of each value by going from the output layer to the input layer
- one important thing to note is that a significant performance increase was achieved by implementing momentum in the network
- momentum works by making previous gradients relevant for the current calculations and implementing this sped up the process of training immensely
- I have not implemented regularization due to the scope of this project

## Important keybinds and input

- g: resets the cost graph
- c: hides the cost graph
- Enter: starts training
- Space: toggles the main plot visualization
- d: toggle between view interpolation
- Ctrl + z: undo
- Ctrl + y: redo
- Ctrl + s: save the dataset in file
- Ctrl + o: loads the dataset from the same file
- network view panning: you can scroll to zoom in and out, and you can also hold right-click to pan the camera around the network visualization

![2024-05-27 23-00-18](https://github.com/Geutzzu/Neural-Network-SFML/assets/80857756/7dc9ac57-5997-4b35-a20a-68aa5059ce99)

## Notes
- The app runs by default in 1920x1080 fullscreen
- I might expand this readme to dig deeper into some features such as the math behind the network
