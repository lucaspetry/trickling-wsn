#ifndef MLP_PREDICTOR_HPP_
#define MLP_PREDICTOR_HPP_

#include "predictor.hpp"
#include <array>
#include <math.h>

template<class Type, unsigned short INPUTS, unsigned short HIDDEN_UNITS>
class MLPPredictor : public Predictor<Type> {

using arrayInput = std::array<double, INPUTS>;
using array = std::array<double, HIDDEN_UNITS>;
using matrix = std::array<arrayInput, HIDDEN_UNITS>;

private:
  arrayInput _lastInputs;
  matrix _wInput; // pesos saindo do nodo de entrada para a hidden layer
  array _biasInput; // bias entrando na hidden layer, junto com o nodo de entrada
  array _wHiddenLayer; // pesos saindo de cada item da hidden layer para o nodo de saida
  double _biasHidden; // bias entrando na output layer, junto com a hidden layer

  bool _isNormalized;
  Type _trainingMin;
  Type _trainingMax;
  unsigned int _currentIndex;

public:
  MLPPredictor(matrix wInput, array biasInput, array wHiddenLayer, double biasHidden, bool isNormalized = false, Type trainingMin = 0, Type trainingMax = 0) {
        _wInput = wInput;
        _biasInput = biasInput;
        _wHiddenLayer = wHiddenLayer;
        _biasHidden = biasHidden;

        _isNormalized = isNormalized;
        _trainingMin = trainingMin;
        _trainingMax = trainingMax;

        _currentIndex = 0;

        for(int i = 0; i < INPUTS; i++)
          _lastInputs[i] = 0.2578; // TODO(LUCAS) - Only for test purposes
    }

  ~MLPPredictor() { }

  void addLastValue(Type lastValue) {
    if(_currentIndex >= INPUTS) {
      for(unsigned int i = 1; i < INPUTS; i++)
        _lastInputs[i - 1] = _lastInputs[i];
      
      _currentIndex = INPUTS - 1;
    }
   
    _lastInputs[_currentIndex++] = lastValue;
  }

  inline Type normalize(Type nonNormalizedValue) {
    return (nonNormalizedValue - _trainingMin) / (_trainingMax - _trainingMin);
  }

  inline Type denormalize(Type normalizedValue) {
    return normalizedValue * (_trainingMax - _trainingMin) + _trainingMin;
  }

  Type predictNext(Type lastValue) {
    if(_isNormalized) lastValue = normalize(lastValue);
    addLastValue(lastValue);

  	// multiplica a entrada pelos pesos de entrada e soma com o bias
  	// para cada um dos N neuronios intermediarios
  	// e passa pela função de ativação
    double outputHiddenLayer[HIDDEN_UNITS];
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      double sum = 0;
      for(int j = 0; j < INPUTS; j++) {
          sum += _lastInputs[j] * _wInput[i][j];
      }

   		sum += _biasInput[i];
     	outputHiddenLayer[i] = activationFunction(sum);
    }

    //multiplica e soma os valores no nodo de saida
    double sumOutputLayer = _biasHidden;
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      sumOutputLayer += outputHiddenLayer[i] * _wHiddenLayer[i];
    }

    Type output = outputActivationFunction(sumOutputLayer);
    if(_isNormalized) output = denormalize(output);
    return output;
  }

  double activationFunction(double summation) {
    return 1/(1+exp(-summation));
  }

  inline double outputActivationFunction(double summation) {
    return summation;
  }
};

#endif // MLP_PREDICTOR_HPP_