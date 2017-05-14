#include "ElmanPredictor.hpp"
#include "LinearPredictor.hpp"
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
  
  std::cout << "f(t)\t\t\tP(f(t-1))" << std::endl;
  for(int t = 0; t < 30; t++) {
    std::cout << "f(" << t << ") = " << linearFunction(m, b, t) <<
      "\t\tP(" << linearFunction(m, b, t - 1) << ") = " << predictor->predictNext(linearFunction(m, b, t - 1)) << std::endl;
  }
  std::cout << "\n" << std::endl;
}

void testTriangularFunction(Predictor<double>* predictor) {
  // Triangular      c
  //              a     b
  double a = 0.345254634, b = 7, c = 5;
  
  std::cout << "f(t)\t\tP(f(t-1))" << std::endl;
  for(int t = 0; t < 30; t++) {
    std::cout << "f(" << t << ") = " << triangularFunction(a, b, c, t) <<
      "\t\tP(" << triangularFunction(a, b, c, t - 1) << ") = " << predictor->predictNext(triangularFunction(a, b, c, t - 1)) << std::endl;
  }
  std::cout << "\n" << std::endl;
}

int main() {
  Predictor<double>* predictor = new LinearPredictor<double, 20>();
  testLinearFunction(predictor);
  
  delete predictor;
  predictor = new LinearPredictor<double, 20>();
  testTriangularFunction(predictor);
  
  return 0;
}