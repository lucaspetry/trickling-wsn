#include <iostream>
#include "mlp_predictor.hpp"

#define HIDDEN_UNITS 5
#define INPUTS 3

using arrayInput = std::array<double, INPUTS>;
using array = std::array<double, HIDDEN_UNITS>;
using matrix = std::array<arrayInput, HIDDEN_UNITS>;

int main(int argc, char* argv[]) {
	matrix wInput;
	wInput[0] = {-0.6183, -0.7241, 4.6622};
	wInput[1] = {-2.2563, 4.1614, 0.9684};
	wInput[2] = {-1.1493, 1.4208, 4.3769};
	wInput[3] = {-2.2230, -0.8643, -4.0633};
	wInput[4] = {3.0342, 3.2611, -1.5842};
	array biasInput = {4.8398, 2.2577, -0.2679, -2.3915, 4.8339};
	array wHiddenLayer = {-0.4124, 0.8962, 0.6646, -0.1890, -0.5212};
	double biasHidden = -0.1443;

	double minValue = 1.7;
	double maxValue = 33.9;

 	auto predictor = new MLPPredictor<double, INPUTS, HIDDEN_UNITS>(wInput, biasInput, wHiddenLayer,
 																	biasHidden, false, minValue, maxValue);

 	double inputs[] = {0.2578, 0.1708, 0.1708, 0.1708, 0.1366, 0.1553, 0.0839, 0.2578, 0.2391, 0.2391, 0.2391};
 	double expect[] = {0.2574, 0.2227, 0.1487, 0.2038, 0.1930, 0.1728, 0.1917, 0.1962, 0.3352, 0.2335, 0.2433};

	std::cout << "Matlab\t\tOutput\n" << std::endl;
 	for(int i = 0; i < 11; i++) {
 		double lastValue = predictor->predictNext(inputs[i]);
 		std::cout << expect[i] << "\t\t" << lastValue << std::endl;
 	}

  	return 0;
}