#include <fstream>
#include <iostream>
#include <iomanip>
#include "mlp_predictor.hpp"

#define HIDDEN_UNITS 5
#define INPUTS 3

using arrayInput = std::array<double, INPUTS>;
using array = std::array<double, HIDDEN_UNITS>;
using matrix = std::array<arrayInput, HIDDEN_UNITS>;

void printUsage() {
  std::cout << "  Usage is:" << std::endl;
  std::cout << "   -in <datafile>        : Single-value series input data (time should not be provided)" << std::endl;
  std::cout << "   -out <statsfile>      : Output statistics file" << std::endl;
  std::cout << "   -margin <margin>      : Percentual acceptance margin from 0 to 1 (default: 0.05) " << std::endl;
}

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

	if (argc < 5) {
	  printUsage();
	  return 0;
	}

	std::string inputFile = "";
	std::string outputFile = "";
	float accMargin = 0.05;

	for(int i = 1; i < argc; i++) {
		if (i + 1 != argc)
			if(std::string(argv[i]) == "-in") {
				inputFile = std::string(argv[i+1]);
			} else if(std::string(argv[i]) == "-out") {
				outputFile = std::string(argv[i+1]);
			} else if(std::string(argv[i]) == "-margin") {
				accMargin = strtof(argv[i+1], 0);
			}
	}

	if(inputFile == "") {
		std::cout << "  ERROR: NO INPUT FILE PROVIDED!\n" << std::endl;
		printUsage();
		return 0;
	} else if(outputFile == "") {
		std::cout << "  ERROR: NO OUTPUT FILE PROVIDED!\n" << std::endl;
		printUsage();
		return 0;
	}

	std::cout << "\nTest Parameters:" << std::endl;
	std::cout << "> Input File:        " << inputFile << std::endl;
	std::cout << "> Output File:       " << outputFile << std::endl;
	std::cout << "> Acceptance Margin: " << accMargin << std::endl;

	auto predictor = new MLPPredictor<double, INPUTS, HIDDEN_UNITS>(wInput, biasInput, wHiddenLayer,
																		biasHidden, true, minValue, maxValue);

	std::ifstream input(inputFile.c_str());

	if(!input.is_open()) {
		std::cout << "  ERROR: COULD NOT OPEN INPUT FILE!\n" << std::endl;    
	}

	std::ofstream output;
	output.open(outputFile.c_str());

	if(!output.is_open()) {
		std::cout << "  ERROR: COULD NOT WRITE TO OUTPUT FILE!\n" << std::endl;    
	}

	std::cout << "\nRunning predictor... ";
	std::cout.flush();

	output << "data_point,predicted,difference,hit," << std::endl;  
	std::string line = "";
	float lastValue = 0;
	unsigned int points = 0;
	unsigned int hits = 0;
	float mean = 0;
	unsigned int firstHit = 0;

	while(getline(input, line)) {
	float dataPoint = strtof(line.c_str(), 0);
	float predicted = predictor->predictNext(lastValue);
	float difference = fabs(dataPoint - predicted);
	mean += difference;
	float margin = dataPoint * accMargin;
	unsigned int hit = (predicted >= dataPoint - margin && predicted <= dataPoint + margin) ? 1 : 0;

	points++;
	hits += hit;
	output << dataPoint << "," << predicted << "," << difference << "," << hit << "," << std::endl;

	if(hit == 1)
	  lastValue = predicted;
	else
	  lastValue = dataPoint;

	if(firstHit == 0 && hit == 1)
	  firstHit = points;
	}

	mean = mean/points;

	std::cout << "DONE!\n" << std::endl;
	float hitsPerc = ((float) hits / (float) points) * 100;

	std::cout << "Predictor Hits: " << hitsPerc << "% (" << hits << "/" << points << ")" << std::endl;
	std::cout << "Mean of Error:  " << mean << std::endl;
	std::cout << "First Hit:      Element #" << firstHit << "\n" << std::endl;

	// Clean-up
	input.close();
	output.close();
	delete predictor;

	return 0;
}
