#include <iostream>
#include "mlp_predictor.hpp"

#define HIDDEN_UNITS 5
#define INPUTS 3

using arrayInput = std::array<double, INPUTS>;
using array = std::array<double, HIDDEN_UNITS>;
using matrix = std::array<arrayInput, HIDDEN_UNITS>;

int main(int argc, char* argv[]) {
	matrix wInput;
	wInput[0] = {-0.618286781629468, -0.724056702800380, 4.66223842383100};
	wInput[1] = {-2.25629386684104, 4.16138800108699, 0.968371515398330};
	wInput[2] = {-1.14929932529931, 1.42080487252203, 4.37687735275928};
	wInput[3] = {-2.22303524180525, -0.864304934954279, -4.06333035928021};
	wInput[4] = {3.03422058199185, 3.26105843183883, -1.58422262880005};
	array biasInput = {4.83983182011767, 2.25770237433247, -0.267896160836834, -2.39151299799297, 4.83391715868682};
	array wHiddenLayer = {-0.412420967808636, 0.896218844051993, 0.664567894352862, -0.189001779575074, -0.521224580849148};
	double biasHidden = -0.144331309491435;

	double minValue = 1.7;
	double maxValue = 33.9;

 	auto predictor = new MLPPredictor<double, INPUTS, HIDDEN_UNITS>(wInput, biasInput, wHiddenLayer,
 																	biasHidden, false, minValue, maxValue);

 	double inputs[] = {0,257763975155280, 0,170807453416149, 0,170807453416149, 0,170807453416149, 0,136645962732919, 0,155279503105590, 0,0838509316770187, 0,257763975155280, 0,239130434782609, 0,239130434782609};
 	double expect[] = {0,170807453416149, 0,170807453416149, 0,170807453416149, 0,136645962732919, 0,155279503105590, 0,0838509316770187, 0,257763975155280, 0,239130434782609, 0,239130434782609, 0,239130434782609};
	
	std::cout << "Matlab\t\tOutput\n" << std::endl;
 	for(int i = 0; i < 11; i++) {
 		double lastValue = predictor->predictNext(inputs[i]);
 		std::cout << expect[i] << "\t\t" << lastValue << std::endl;
 	}

  	return 0;
}
