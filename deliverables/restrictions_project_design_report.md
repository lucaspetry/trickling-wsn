# [EPOSMote III](http://epos.lisha.ufsc.br/EPOSMote+III) Restrictions and Project Design (Task 2)

## Project Design
Our goal is to implement a **dynamic** linear regression predictor with gradient descent and a **static** Elman neural network predictor. When a gateway has interest on the data coming from a sensor and data is available, the `notify()` method on the sensor-side will cause the data to be broadcasted to the network and reach the gateway. Therefore, we propose some modifications to the [SmartData](http://epos.lisha.ufsc.br/EPOS+2+User+Guide#Smart_Data) class and the design of a subclass __PredictiveSmartData__. [SmartData](http://epos.lisha.ufsc.br/EPOS+2+User+Guide#Smart_Data) will implement the `broadcast()` method below, which will replace all the calls to `notify()` in the [SmartData](http://epos.lisha.ufsc.br/EPOS+2+User+Guide#Smart_Data) class.

`void Smart_Data::broadcast() { notify(); }`

The **PredictiveSmartData** will overwrite this method, verifying with the predictors if observers should be notified (and the data transmitted). The simplified class diagram below summarizes the new classes and modifications to [SmartData](http://epos.lisha.ufsc.br/EPOS+2+User+Guide#Smart_Data) that are being proposed.
![Class Diagram](https://github.com/lucaspetry/trickling-wsn/tree/master/deliverables/d2/class_diagram.png)
Whenever a **PredictiveSmartData** is instantiated, the set operation mode will always be `ADVERTISED`, given that we're only interested in sensors being monitored by the gateway. The *errorMargin* is a user-defined parameter in **PredictiveSmartData**, meaning how acceptable a predicted value *y* is, given the value *x* read from the sensor. *y* is acceptable if it lies between *x - errorMargin* and *x + errorMargin*. The predictor-specific parameters are described as follows.

### Linear Regression Predictor Parameters
- *m* and *b*: coefficients of the linear function *y = mx + b*;
- *N*: number of data points that will be considered to update the coefficients *m* and *b*;
- *learningRate*: learning rate of the gradient descent algorithm;
- *gradientIterations*: stop criteria for the gradient descent algorithm.
These parameters have default values, which are used if no values are fed by the user.

### Elman Neural Network Predictor Parameters
- *wInputLayer*: input layer weights of the neural network;
- *wHiddenLayer*: hidden layer weights of the neural network;
- *wInputLayer*: output layer weight of the neural network.
These parameters must be obtained from a previously trained neural network, using an existing series of historic data.

## Hardware Restrictions
The most concerning hardware configuration for our project is ROM and RAM, and CPU clock speed. [EPOSMote III](http://epos.lisha.ufsc.br/EPOSMote+III) provides the following configuration:
- 512Kbyte, 256Kbyte or 128Kbyte flash memory (ROM);
- 32Kbyte RAM;
- Up to 32-MHz clock speed.
Due to the fact that the linear predictor is dynamic, we'll need to store some parameters and a limited number of data values in RAM. On the other hand, the parameters used by the Elman predictor can be stored in ROM. Lastly, the *lastValue* attribute in the **PredictiveSmartData** needs to sit in RAM.

## Challenges Beyond the Scope of the Project
During our analysis, we identified some challenges regarding communication in a WSN that could affect our implementation. Bearing in mind that the gateway assumes that the predicted value is correct whenever it doesn't receive data within `period` time, if the network fails to deliver the messages from the sensor to the gateway, a desynchronization of the predictors running on the sensor and on the gateway may occur.

A solution to this problem would be to allow the gateway to somehow know when a failure in the WSN happens. A possible solution we have discussed is to have a user-defined parameter defining a period of time in which the sensor **must** transmit data to the gateway at least once. Hence, the gateway has means to know if a transmission problem happened within a period of time, being able to take any necessary actions.
