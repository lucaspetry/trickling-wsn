#ifndef MLP_PREDICTOR_HPP_
#define MLP_PREDICTOR_HPP_

#include "predictor.hpp"
#include <array>
#include <math.h>


template<class Type, unsigned short HIDDEN_UNITS>
class MLPPredictor : public Predictor<Type> {

using array = std::array<float, HIDDEN_UNITS>;

private:
  array _wInput; // pesos saindo do nodo de entrada para a hidden layer
  array _biasInput; // bias entrando na hidden layer, junto com o nodo de entrada
  array _wHiddenLayer; // pesos saindo de cada item da hidden layer para o nodo de saida
  float _biasHidden; // bias entrando na output layer, junto com a hidden layer
public:
  MLPPredictor(array wInput, array biasInput, array wHiddenLayer, float biasHidden) {
        _wInput = wInput;
        _biasInput = biasInput;
        _wHiddenLayer = wHiddenLayer;
        _biasHidden = biasHidden;
    }

  ~MLPPredictor() { }

  Type predictNext(Type lastValue) {

  	// multiplica a entrada pelos pesos de entrada e soma com o bias
  	// para cada um dos N neuronios intermediarios
  	// e passa pela função de ativação
    float outputHiddenLayer[HIDDEN_UNITS];
    float summation = 0;
    for(int i = 0; i < HIDDEN_UNITS; i++) {
   		summation = lastValue * _wInput[i] + _biasInput[i];
     	outputHiddenLayer[i] = activationFunction(summation);
    }

    //multiplica e soma os valores no nodo de saida
    float sumOutputLayer = _biasHidden;
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      sumOutputLayer += outputHiddenLayer[i] * _wHiddenLayer[i];
    }

    return outputActivationFunction(sumOutputLayer);
  }

  float activationFunction(float summation) {
    return 1/(1+exp(-summation));
  }

  float outputActivationFunction(float summation) {
    return summation;
  }
};

#endif // MLP_PREDICTOR_HPP_