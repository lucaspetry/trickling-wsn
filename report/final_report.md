# Predictors for Trickling in WSN

## Authors
- Douglas Marcelino Beppler Martins
- Lucas May Petry
- Maike de Paula Santos

## Motivation 
Nowadays, the Internet of Things (IoT) is present in numerous kinds of devices, from residential automation to smart cities (INTERNET... 2017). In an IoT environment, the more devices there are, the bigger is the number of messages exchanged among them. This increase in communication can lead to several problems. One of these issues is the device’s power consumption. Since many sensors make use of a battery as their power source, the excessive use of their wireless may drain a large portion of it (SOMASUNDARA et al., 2006). Moreover, a large amount of communication between devices implies a large spending in transmission infrastructure. Hence, tools and techniques that allow companies to reduce the previously mentioned factors become of great value.

## Goals
In order to diminish the communication between a sensor and a gateway, we propose the implementation of [SmartData](http://epos.lisha.ufsc.br/EPOS+2+User+Guide#Smart_Data) predictors. The goal is to run the designed predictor on both the sensor and the gateway, so they talk less without losing information. Every time data becomes available on a sensor, the data will be sent to the gateway only if it differs from the predicted value, given a user-defined threshold. If that’s not the case, the gateway will know the value beforehand via the predictor.

## Methodology
After studying the existing algorithms, we chose to program a linear regression with gradient descent (NEDRICH, 2014) and Elman Neural Networks (KOSKELA et al., 1997) as our predictors. We intend to prototype these algorithms in the C++ language for a general purpose operating system, so that we are able to test them with manual inputs and confirm the desired behavior. Subsequently, the algorithms will be ported to run on [EPOSMote III](http://epos.lisha.ufsc.br/EPOSMote+III), using real sensors to simulate a gateway-sensor environment and validate the efficiency of this approach. Initially, we will run tests in a controlled environment. Afterwards, data from the [UFSC IoT Gateway](http://iot.ufsc.br/HomePage) will be used to demonstrate the proposed algorithms.

## Tasks
1. Make project plan.
2. Study system restrictions and limitations of [EPOSMote III](http://epos.lisha.ufsc.br/EPOSMote+III) and elaborate implementation strategies, such as design patterns.
3. Code and test the chosen prediction algorithms in C++ for a general purpose operating system.
4. Code and test the chosen prediction algorithm with basic statistics on [EPOS](http://epos.lisha.ufsc.br/EPOS+2+User+Guide).
5. Code and test the chosen prediction algorithm with neural networks on [EPOS](http://epos.lisha.ufsc.br/EPOS+2+User+Guide).
6. Deploy the final product using the [UFSC IoT Gateway](http://iot.ufsc.br/HomePage).
7. Present the final project and results.

## Deliverables
1. Project plan.
2. Report on restrictions and limitations of embedded systems and the strategy used to tackle the problems.
3. Code and test results of the prediction algorithms on a general purpose operating system.
4. Code and test results of the basic statistics predictor on [EPOS](http://epos.lisha.ufsc.br/EPOS+2+User+Guide).
5. Code and test results of the neural network predictor on [EPOS](http://epos.lisha.ufsc.br/EPOS+2+User+Guide).
6. Deployed algorithm on [UFSC IoT Gateway](http://iot.ufsc.br/HomePage).
7. Project demonstration.

## Schedule
- First week (W1): 4/26/2017. 
- Last week (W10): 7/5/2017.

| Task                                                                                                                          | W1 | W2 | W3 | W4 | W5 | W6 | W7 | W8 | W9 | W10 |
|:-------------------------------------------------------------------------------------------------------------------------------|:--:|:--:|:--:|:--:|:--:|:--:|----|----|----|:---:|
| 1. Make project plan                                                                                                          |  X | D1 |    |    |    |    |    |    |    |     |
| 2. Study system restrictions and limitations of EPOSMote III and elaborate implementation strategies, such as design patterns |    |  X | D2 |    |    |    |    |    |    |     |
| 3. Code and test the chosen prediction algorithms in C++ for a general purpose operating system                               |    |    |  X | D3 |    |    |    |    |    |     |
| 4. Code and test the linear regression with gradient descent predictor on EPOS                                                |    |    |    |  X |  X | D4 |    |    |    |     |
| 5. Code and test the Elman neural network predictor on EPOS                                                                   |    |    |    |    |    |  X | X  | D5 |    |     |
| 6. Deploy the final product using the UFSC IoT Gateway                                                                        |    |    |    |    |    |    |    | X  | X  |  D6 |
| 7. Present the final project and results                                                                                      |    |    |    |    |    |    |    |    |    |  D7 |
