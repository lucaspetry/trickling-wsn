#ifndef ELMAN_PREDICTOR_HPP_
#define ELMAN_PREDICTOR_HPP_

#include "Predictor.hpp"

template<class Type, unsigned short HIDDEN_UNITS>
class ElmanPredictor : public Predictor<Type> {

private:
  float _wInputLayer[HIDDEN_UNITS + 1];
  float _wHiddenLayer[HIDDEN_UNITS];
  float _wOutputLayer;

public:
  ElmanPredictor(float wInputLayer[HIDDEN_UNITS + 1], float wHiddenLayer[HIDDEN_UNITS], float wOutputLayer) {
    _wInputLayer = wInputLayer;
    _wHiddenLayer = wHiddenLayer;
    _wOutputLayer = wOutputLayer;
  }
  
  ~ElmanPredictor() { }
  
  Type predictNext(Type lastValue) {
    return 0; // TODO
  }
  
};

#endif // ELMAN_PREDICTOR_HPP_