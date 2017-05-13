#ifndef ELMAN_PREDICTOR_HPP_
#define ELMAN_PREDICTOR_HPP_

#include "Predictor.hpp"

template<class Type>
class ElmanPredictor : public Predictor<Type> {

public:
  ElmanPredictor() { }
  
  ~ElmanPredictor() { }
  
  Type predictNext(Type lastValue) {
    return 0; // TODO
  }
  
};

#endif // ELMAN_PREDICTOR_HPP_