#ifndef ELMAN_PREDICTOR_HPP_
#define ELMAN_PREDICTOR_HPP_

#include "predictor.hpp"

template<class Type, unsigned short HIDDEN_UNITS>
class ElmanPredictor : public Predictor<Type> {

private:
  float _fHiddenLayer[HIDDEN_UNITS]; // Hidden layer output
  float _wInputLayer[HIDDEN_UNITS + 1]; // Input->Hidden weigthts. Last weight is applied to the user input.
  float _wHiddenLayer[HIDDEN_UNITS + 1]; // Hidden->Output weights. Last weight is applied to the bias.
  
public:
  ElmanPredictor(float wInputLayer[HIDDEN_UNITS + 1], float wHiddenLayer[HIDDEN_UNITS + 1]) {
    _wInputLayer = wInputLayer;
    _wHiddenLayer = wHiddenLayer;
  }
  
  ~ElmanPredictor() { }
  
  Type predictNext(Type lastValue) {
    float newFHiddenLayer[HIDDEN_UNITS];
    float output = 0;
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      float summation = 0;
      for(int j = 0; j < HIDDEN_UNITS; j++) {
        summation += _fHiddenLayer[j] * _wInputLayer[j];
      }
      summation += lastValue * _wInputLayer[HIDDEN_UNITS]; // Factor in the input
      newFHiddenLayer[i] = summation; // TODO Missing activation function
    }

    for(int i = 0; i < HIDDEN_UNITS; i++) {
      output += _fHiddenLayer[i] * _wHiddenLayer[i]; // TODO Missing activation function
    }

    _fHiddenLayer = newFHiddenLayer;
    return output;
  }
  
};

#endif // ELMAN_PREDICTOR_HPP_