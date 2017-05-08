# Predictors for Trickling in WSN

## Authors
- Douglas Marcelino Beppler Martins
- Lucas May Petry
- Maike de Paula Santos

## Motivation 
Nowadays, the Internet of Things (IoT) is present in numerous kinds of devices, from residential automation to smart cities (INTERNET... 2017). In an IoT environment, the more devices there are, the bigger is the number of messages exchanged among them. This increase in communication can lead to several problems. One of these issues is the device’s power consumption. Since many sensors make use of a battery as their power source, the excessive use of their wireless may drain a large portion of it (SOMASUNDARA et al., 2006). Moreover, a large amount of communication between devices implies in a large spending with transmission infrastructure. Hence, tools and techniques that allow companies to reduce the previously mentioned factors become of great value.

## Goals
In order to diminish the communication between a sensor and a gateway, we propose the implementation of [SmartData](http://epos.lisha.ufsc.br/EPOS+2+User+Guide#Smart_Data) predictors. The goal is to run the designed predictor on both the sensor and the gateway, so they talk less without losing information. Every Time data becomes available on a sensor, the data will be sent to the gateway only if it differs from the predicted value, given a user-defined threshold. If that’s not the case, the gateway will know the value beforehand via the predictor.

## Methodology
After studying the existing algorithms, we intend to prototype the chosen ones in the C++ language for a general purpose operating system, so that we are able to test them with manual inputs and confirm the desired behavior. Subsequently, the algorithms will be ported to run on [EPOSMote III](http://epos.lisha.ufsc.br/EPOSMote+III), using real sensors to simulate a gateway-sensor environment and validate the efficiency of this approach. Initially, we will run tests in a controlled environment. Afterwards, data from the [UFSC IoT Gateway](http://iot.ufsc.br/HomePage) will be used to demonstrate the proposed algorithms.

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
