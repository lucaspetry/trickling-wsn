#include <fstream>
#include <iostream>
#include <iomanip>
#include "mlp_predictor.hpp"

#define HIDDEN_UNITS 5
#define INPUTS 4

using arrayInput = std::array<float, INPUTS>;
using array = std::array<float, HIDDEN_UNITS>;
using matrix = std::array<arrayInput, HIDDEN_UNITS>;

void printUsage() {
  std::cout << "  Usage is:" << std::endl;
  std::cout << "   -in <datafile>        : Single-value series input data (time should not be provided)" << std::endl;
  std::cout << "   -out <statsfile>      : Output statistics file" << std::endl;
  std::cout << "   -margin <margin>      : Percentual acceptance margin from 0 to 1 (default: 0.05) " << std::endl;
}

int main(int argc, char* argv[]) {
	matrix wInput;
	wInput[0] = {1.856189880744175, -2.364044192323763, -2.015193948157803, -2.111389598306678};
	wInput[1] = {2.452078535933905, -1.502047403453988, 2.858228661817955, -0.592074412290217};
    wInput[2] = {-2.379480482473748, 0.050063969277013, 2.423120236133121, 2.233053371105561};
    wInput[3] = {2.504129446209458, 2.777068627114355, -0.117111318654031, 1.770901264697802};
    wInput[4] = {0.774307665838050, 2.677979856190503, 1.735792413082430, 2.647371994499392};
	array biasInput = {-4.214431915868052, -2.173575154565063, -0.638939802192748, 2.059575481573106, 4.160726537029708};
	array wHiddenLayer = {-0.500381577509181, 0.505849042103031, 1.239191435875788, 0.230485145952991, 0.062830637852658};
	float biasHidden = -0.742589287808183f;

	float minValue = 0.0;
	float maxValue = 2025.0;

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

	auto predictor = new MLPPredictor<float, INPUTS, HIDDEN_UNITS>(wInput, biasInput, wHiddenLayer,
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
