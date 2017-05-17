#include "elman_predictor.hpp"
#include "linear_predictor.hpp"
#include <iostream>

double linearFunction(double m, double b, int t) {
  return m*t + b;
}

double triangularFunction(double a, double b, double c, int t) {
  double newT = t % ((int) b);
  
  if(newT <= c)
    return (2*(newT - a)) / ((b - a)*(c - a));
  
  return (2*(b - newT)) / ((b - a)*(b - c));
}

void testLinearFunction(Predictor<double>* predictor) {
  // Linear f(t) = mt + b
  double m = 3, b = 9;
  
  std::cout << "f as a Linear Function" << std::endl;
  std::cout << "f(t)\t\t\tP(f(t-1))" << std::endl;
  for(int t = 0; t < 150; t++) {
    std::cout << "f(" << t << ") = " << linearFunction(m, b, t) <<
      "\t\tP(" << linearFunction(m, b, t - 1) << ") = " << predictor->predictNext(linearFunction(m, b, t - 1)) << std::endl;
  }
  std::cout << "\n" << std::endl;
}

void testTriangularFunction(Predictor<double>* predictor) {
  // Triangular      c
  //              a     b
  double a = 2, b = 2342, c = 233;
  
  std::cout << "f as a Triangular Function" << std::endl;
  std::cout << "f(t)\t\t\tP(f(t-1))" << std::endl;
  for(int t = 0; t < 70; t++) {
    std::cout << "f(" << t << ") = " << triangularFunction(a, b, c, t) <<
      "\t\tP(" << triangularFunction(a, b, c, t - 1) << ") = " << predictor->predictNext(triangularFunction(a, b, c, t - 1)) << std::endl;
  }
  std::cout << "\n" << std::endl;
}

int main() {
  Predictor<double>* predictor = new LinearPredictor<double, 50>(0.00001, 3000, 0, 0);
  testLinearFunction(predictor);
  delete predictor;
  
  // Daily mean temperatures in Davis, CA from 1/1/2014 to 7/30/2014
  float temp[] = {45.4,47.8,47.2,48.8,53.2,46.6,47.7,46.2,46.9,46.5,48.9,48.6,52.2,48.4,48.3,47.5,48.7,48.1,48.4,49.9,49.3,48.9,50.9,56.4,56.1,52.0,49.0,55.4,57.9,55.5,51.9,48.1,44.6,44.8,43.5,41.7,47.4,48.2,51.0,55.9,56.8,53.0,52.9,56.4,59.4,56.6,55.8,50.9,52.1,57.4,55.2,55.7,57.0,54.2,55.8,56.9,55.3,55.7,54.9,55.7,53.4,54.7,57.3,59.6,56.4,55.8,60.1,60.6,57.8,59.7,60.4,56.3,62.8,61.0,59.7,59.8,58.1,59.2,59.5,58.4,56.8,59.8,56.5,54.0,53.7,56.8,58.0,51.7,49.7,46.5,48.0,52.2,53.0,52.2,60.4,64.4,66.2,67.0,64.9,64.3,59.1,60.3,62.7,62.4,65.9,66.4,68.1,62.6,62.8,63.3,60.4,57.8,61.5,58.3,50.4,55.3,58.9,69.3,73.4,72.9,82.8,65.4,61.7,61.9,61.5,62.5,61.8,62.4,62.4,65.0,69.4,74.0,75.3,76.2,82.1,68.7,68.5,63.9,61.9,65.6,67.8,69.6,75.1,78.5,75.1,73.5,70.9,73.0,66.9,62.5,69.4,69.3,62.5,70.0,73.9,72.1,71.7,93.9,93.2,87.7,78.4,66.7,67.2,74.9,67.5,66.7,65.5,73.9,71.2,72.8,73.3,68.5,65.7,72.0,72.6,70.2,69.9,71.2,74.7,83.1,77.8,71.2,71.9,72.7,71.0,75.5,77.7,77.6,74.2,72.6,71.5,68.7,72.4,82.9,80.5,78.3,72.7,70.7,72.7,72.7,73.0,71.5,71.5,74.8,79.4,80.8,80.4,77.5,76.4,77.5};
  predictor = new LinearPredictor<double, 50>(0.00001, 3000, 0, 0);
  
  std::cout << "f as Temperatures in Davis, CA from 1/1/2014 to 7/30/2014" << std::endl;
  std::cout << "f(t)\t\t\tP(f(t-1))" << std::endl;
  for(int t = 1; t < 150; t++) {
    std::cout << "f(" << t << ") = " << temp[t] <<
      "\t\tP(" << temp[t-1] << ") = " << predictor->predictNext(temp[t-1]) << std::endl;
  }
  std::cout << "\n" << std::endl;
  delete predictor;

  predictor = new LinearPredictor<double, 30>(0.0001, 2000, 0, 0);
  testTriangularFunction(predictor);
  delete predictor;
  
  return 0;
}