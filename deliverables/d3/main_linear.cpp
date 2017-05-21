#include "linear_predictor.hpp"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>

void printUsage() {
  std::cout << "  Usage is:" << std::endl;
  std::cout << "   -in <datafile>        : Single-value series input data (time should not be provided)" << std::endl;
  std::cout << "   -out <statsfile>      : Output statistics file" << std::endl;
  std::cout << "   -margin <margin>      : Percentual acceptance margin from 0 to 1 (default: 0.05) " << std::endl;
  std::cout << "   -window <datawindow>  : Number of data points to be considered (default: 50)" << std::endl;
  std::cout << "   -lrate <learningrate> : Linear regression learning rate (default: 0.001)" << std::endl;
  std::cout << "   -i <iterations>       : Number of gradient descent iterations (default: 1000)" << std::endl;
  std::cout << "   -m <initialm>         : Initial m coefficient (default: 0)" << std::endl;
  std::cout << "   -b <initialb>         : Initial b coefficient (default: 0)" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 5) {
      printUsage();
      return 0;
  }
  
  std::string inputFile = "";
  std::string outputFile = "";
  float accMargin = 0.05;
  unsigned int window = 50;
  float learningRate = 0.001;
  unsigned int iterations = 1000;
  float initM = 0;
  float initB = 0;
  
  for(int i = 1; i < argc; i++) {
    if (i + 1 != argc)
      if(std::string(argv[i]) == "-in") {
        inputFile = std::string(argv[i+1]);
      } else if(std::string(argv[i]) == "-out") {
        outputFile = std::string(argv[i+1]);
      } else if(std::string(argv[i]) == "-margin") {
        accMargin = strtof(argv[i+1], 0);
      } else if(std::string(argv[i]) == "-window") {
        window = atoi(argv[i+1]);
      } else if(std::string(argv[i]) == "-lrate") {
        learningRate = strtof(argv[i+1], 0);
      } else if(std::string(argv[i]) == "-i") {
        iterations = atoi(argv[i+1]);
      } else if(std::string(argv[i]) == "-m") {
        initM = strtof(argv[i+1], 0);
      } else if(std::string(argv[i]) == "-b") {
        initB = strtof(argv[i+1], 0);
      }
  }
  
  if(inputFile == "") {
    std::cout << "  ERROR: NO INPUT FILE PROVIDED!\n" << std::endl;
    printUsage();
    return 0;
  }
  else if(outputFile == "") {
    std::cout << "  ERROR: NO OUTPUT FILE PROVIDED!\n" << std::endl;
    printUsage();
    return 0;
  }
  
  std::cout << "\nTest Parameters:" << std::endl;
  std::cout << "> Input File:        " << inputFile << std::endl;
  std::cout << "> Output File:       " << outputFile << std::endl;
  std::cout << "> Acceptance Margin: " << accMargin << std::endl;
  std::cout << "> Window Size:       " << window << std::endl;
  std::cout << "> Learning Rate:     " << learningRate << std::endl;
  std::cout << "> GD Iterations:     " << iterations << std::endl;
  std::cout << "> Initial m:         " << initM << std::endl;
  std::cout << "> Initial b:         " << initB << std::endl;
  
  Predictor<double>* predictor = new LinearPredictor<double>(window, learningRate, iterations, initM, initB);
  
  
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
  
  output << "data_point,predicted,difference,hit" << std::endl;  
  std::string line = "";
  float lastValue = 0;
  unsigned int points = 0;
  unsigned int hits = 0;
  
  while(getline(input, line)) {
    float dataPoint = strtof(line.c_str(), 0);
    float predicted = predictor->predictNext(lastValue);
    double difference = abs(dataPoint - predicted);
    float margin = dataPoint * accMargin;
    unsigned int hit = (predicted >= dataPoint - margin && predicted <= dataPoint + margin) ? 1 : 0;
    
    points++;
    hits += hit;
    output << dataPoint << "," << predicted << "," << difference << "," << hit << "," << std::endl;
    
    if(hit == 1)
      lastValue = predicted;
    else
      lastValue = dataPoint;
  }
  
  std::cout << "DONE!\n" << std::endl;
  float hitsPerc = ((float) hits / (float) points) * 100;
  
  std::cout << "Predictor Hits: " << hitsPerc << "% (" << hits << "/" << points << ")\n" << std::endl;

  // Clean-up
  input.close();
  output.close();
  delete predictor;
  
  return 0;
}