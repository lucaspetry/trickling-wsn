#ifndef LINEAR_PREDICTOR_HPP_
#define LINEAR_PREDICTOR_HPP_

#include "predictor.hpp"

template<class Type>
class LinearPredictor : public Predictor<Type> {

private:
  Type *_dataWindow;
  unsigned int _dataPoints;
  unsigned int _currentIndex;
  double _learningRate;
  short _gradientIterations;
  double _m;
  double _b;
  unsigned long _t;

public:
  LinearPredictor(unsigned int dataPoints = 50, double learningRate = 0.001, short gradientIterations = 1000, double m = 0, double b = 0) {
    _dataPoints = dataPoints;
    _dataWindow = new Type[dataPoints];
    _currentIndex = 0;
    _learningRate = learningRate;
    _gradientIterations = gradientIterations;
    _m = m;
    _b = b;
    _t = 0;
  }
  
  ~LinearPredictor() { }
  
  Type predictNext(Type lastValue) {
    Type nextValue = _m * _t + _b;

    addLastValue(lastValue);    
    updateCoefficients();
    _t++;
        
    return nextValue;
  }
  
protected:
  void addLastValue(Type lastValue) {
    if(_currentIndex >= _dataPoints) {
      for(unsigned int i = 1; i < _dataPoints; i++)
        _dataWindow[i - 1] = _dataWindow[i];
      
      _currentIndex = _dataPoints - 1;
    }
   
    _dataWindow[_currentIndex++] = lastValue;
  }
  
  void updateCoefficients() {
    double m = _m;
    double b = _b;

    double N = _currentIndex;
    for(int i = 0; i < _gradientIterations; i++) {
      double m_gradient = 0;
      double b_gradient = 0;
     
      for(int j = 0; j < _currentIndex; j++) {
        int x = _t - _currentIndex + j;
        double y = _dataWindow[j];

        m_gradient += -(2.0/N) * x * (y - ((m * x) + b));
        b_gradient += -(2.0/N) * (y - ((m * x) + b));
      }

      m = m - (_learningRate * m_gradient);
      b = b - (_learningRate * b_gradient);
    }

    _m = m;
    _b = b;
  }  
};

#endif // LINEAR_PREDICTOR_HPP_