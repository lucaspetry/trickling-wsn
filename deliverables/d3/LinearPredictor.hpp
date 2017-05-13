#ifndef LINEAR_PREDICTOR_HPP_
#define LINEAR_PREDICTOR_HPP_

#include "Predictor.hpp"

template<class Type>
class LinearPredictor : public Predictor<Type> {

public:
  LinearPredictor() { }
  
  ~LinearPredictor() { }
  
  Type predictNext(Type lastValue) {
    return 0; // TODO
  }
  
};

#endif // LINEAR_PREDICTOR_HPP_