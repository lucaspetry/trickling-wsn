#ifndef LINEAR_PREDICTOR_HPP_
#define LINEAR_PREDICTOR_HPP_

#include "Predictor.hpp"
#include <iostream>

template<class Type, unsigned short DATA_POINTS>
class LinearPredictor : public Predictor<Type> {

private:
  Type _dataWindow[DATA_POINTS];
  unsigned int _currentIndex;
  float _learningRate;
  short _gradientIterations;
  float _m;
  float _b;

public:
  LinearPredictor(float learningRate = 0.001, short gradientIterations = 1000, float m = 0, float b = 0) {
    _currentIndex = 0;
    _learningRate = learningRate;
    _gradientIterations = gradientIterations;
    _m = m;
    _b = b;
  }
  
  ~LinearPredictor() { }
  
  Type predictNext(Type lastValue) {
    Type nextValue = _m * lastValue + _b;

    addLastValue(lastValue);    
    updateCoefficients();
    
    return nextValue;    
  }
  
protected:
  void addLastValue(Type lastValue) {
    if(_currentIndex >= DATA_POINTS) {
      for(unsigned int i = 1; i < DATA_POINTS; i++)
        _dataWindow[i - 1] = _dataWindow[i];
      
      _currentIndex = DATA_POINTS - 1;
    }
   
    _dataWindow[_currentIndex++] = lastValue;
  }
  
  void updateCoefficients() {
    float b = _b;
    float m = _m;

    int N = _currentIndex;

    for(int i = 0; i < _gradientIterations; i++) {
      float b_gradient = 0;
      float m_gradient = 0;
     
      for(int j = 0; j < _currentIndex; j++) {
        int x = j+1;
        float y = _dataWindow[j];

        b_gradient += -(2.0/N) * (y - ((m * x) + b));
        m_gradient += -(2.0/N) * x * (y - ((m * x) + b));
      }

      b = b - (_learningRate * b_gradient);
      m = m - (_learningRate * m_gradient);
    }

    _b = b;
    _m = m;

    std::cout << "New function:\ny = " << _m << "x + " << _b << std::endl;
  }  
};

#endif // LINEAR_PREDICTOR_HPP_