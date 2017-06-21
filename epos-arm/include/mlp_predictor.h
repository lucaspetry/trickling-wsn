// EPOS Multilayer Perceptron Predictor Declarations

#ifndef __mlp_predictor_h
#define __mlp_predictor_h

#include <predictor.h>
#include <utility/math.h>

__BEGIN_SYS

template<typename Type>
class MLP_Predictor: public Predictor<Type>
{
public:
    MLP_Predictor() {}
    
    Type predict_next(Type last_value) {
        db<MLP_Predictor>(TRC) << "MLP_Predictor::predict_next(last_value=" << last_value << ")" << endl;
    	// Compute hidden units
	    float outputHiddenLayer[HIDDEN_UNITS];
	    for(unsigned int i = 0; i < HIDDEN_UNITS; i++) {
	   		float sum = last_value * HIDDEN_WEIGHTS[i] + HIDDEN_BIASES[i];
	     	outputHiddenLayer[i] = hiddenActivationFunction(sum);
	    }

	    // Compute output unit
	    float sumOutputLayer = OUTPUT_BIAS;
	    for(unsigned int i = 0; i < HIDDEN_UNITS; i++) {
	      sumOutputLayer += outputHiddenLayer[i] * OUTPUT_WEIGHTS[i];
	    }
	    return outputActivationFunction(sumOutputLayer);
    }

private:
	float hiddenActivationFunction(float sum) { // Sigmoidal function
		return 1 / (1 + exp(-sum));
	}

	inline float outputActivationFunction(float sum) { // Identity function
		return sum;
	}

private:
    typedef Traits<MLP_Predictor<Type>> P_Traits;

    static const unsigned int HIDDEN_UNITS = P_Traits::HIDDEN_UNITS;
    static const float * HIDDEN_WEIGHTS;
    static const float * HIDDEN_BIASES;
    static const float * OUTPUT_WEIGHTS;
    static const float OUTPUT_BIAS;
    
};

template<typename Type>
const float * MLP_Predictor<Type>::HIDDEN_WEIGHTS = MLP_Predictor<Type>::P_Traits::HIDDEN_WEIGHTS;
template<typename Type>
const float * MLP_Predictor<Type>::HIDDEN_BIASES = MLP_Predictor<Type>::P_Traits::HIDDEN_BIASES;
template<typename Type>
const float * MLP_Predictor<Type>::OUTPUT_WEIGHTS = MLP_Predictor<Type>::P_Traits::OUTPUT_WEIGHTS;
template<typename Type>
const float MLP_Predictor<Type>::OUTPUT_BIAS = MLP_Predictor<Type>::P_Traits::OUTPUT_BIAS;

__END_SYS

#endif
