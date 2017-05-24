#ifndef ELMAN_PREDICTOR_HPP_
#define ELMAN_PREDICTOR_HPP_

#include "predictor.hpp"
#include <array>
#include <math.h>



// https://www.mathworks.com/help/nnet/ref/separatewb.html
template<class Type, unsigned short HIDDEN_UNITS>
class ElmanPredictor : public Predictor<Type> {

using array = std::array<float, HIDDEN_UNITS>;
using matrix = std::array<std::array<float, HIDDEN_UNITS>, HIDDEN_UNITS>;

private:
  array _wInputLayer; // IW
  array _biasHiddenLayer; // Bias inputting in hidden layer
  matrix _wHiddenLayer; // LW
  array _wOutputLayer; // pesos que vao pro ultimo neuronio(o de saida)
  float _biasOutputLayer; // bias que vai pro ultimo neuronio(o de saida)

  array _fHiddenLayer; // Output value from the hidden layer

public:
  ElmanPredictor(array wInputLayer, array biasHiddenLayer, matrix wHiddenLayer, array wOutputLayer, float biasOutputLayer) {
        _wInputLayer = wInputLayer;
        _biasHiddenLayer = biasHiddenLayer;
        _wHiddenLayer = wHiddenLayer;
        _wOutputLayer = wOutputLayer;
        _biasOutputLayer = biasOutputLayer;

        for(int i = 0; i < HIDDEN_UNITS; i++) {
          _fHiddenLayer[i] = 1;
        }
      }

  ~ElmanPredictor() { }

  Type predictNext(Type lastValue) {

    //para cada um dos N neuronios input
    // multiplica pelos pesos de entrada(_wInputLayer)
    float vInputLayer[HIDDEN_UNITS];
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      vInputLayer[i] = lastValue * _wInputLayer[i];
    }


    //multiplica o valor da saida de cada neuronio
    //pelo peso da aresta respectiva de entrada de cada
    //neuronio na retroalimentacao
    float vHiddenLayer[HIDDEN_UNITS][HIDDEN_UNITS];
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      for(int j = 0; j < HIDDEN_UNITS; j++) {
        vHiddenLayer[i][j] = _fHiddenLayer[i]*_wHiddenLayer[i][j]; // assume o risco
      }
    }

    //faz o somatorio e coloca em sumHiddenLayer
    float sumHiddenLayer[HIDDEN_UNITS];
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      sumHiddenLayer[i] = _biasHiddenLayer[i] + vInputLayer[i];
      for(int j = 0; j < HIDDEN_UNITS; j++) {
        sumHiddenLayer[i]+= vHiddenLayer[i][j];
      }
    }

    //O valor retroalimentado(_fHiddenLayer) eh a saida da
    // funcao de ativacao ou o resultado do somatorio? é o valor
    // da saida da funcao de ativacao que vai na rretroalimentacao
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      _fHiddenLayer[i] = activationFunction(sumHiddenLayer[i]);
    }

    //multiplica e soma os valores no nodo de saida
    float sumOutputLayer = _biasOutputLayer;
    for(int i = 0; i < HIDDEN_UNITS; i++) {
      sumOutputLayer+= _fHiddenLayer[i]*_wOutputLayer[i];
    }
    return outputActivationFunction(sumOutputLayer);
  }

  float activationFunction(float summation) {
    return 2/(1+exp(-2*summation))-1;
  }

  float outputActivationFunction(float summation) {
    return summation;
  }
};

#endif // ELMAN_PREDICTOR_HPP_