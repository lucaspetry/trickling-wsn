// EPOS Multilayer Perceptron Predictor Declarations

#ifndef __mlp_predictor_h
#define __mlp_predictor_h

#include <predictor.h>
#include <utility/math.h>
#include <utility/ostream.h>

__BEGIN_SYS

template<typename Type>
class MLP_Predictor: public Predictor<Type>
{
public:
    MLP_Predictor() {
        for(unsigned int i = 0; i < PT::LAG_INPUTS; i++)
            _lag_inputs[i] = 0.0;
    }
    
    Type predict_next(Type last_value) {
        db<MLP_Predictor>(TRC) << "MLP_Predictor::predict_next(last_value=" << last_value << ")" << endl;
        
        // Add value and normalize if necessary
        float input = last_value;
        if(PT::NORMALIZATION) input = normalize(input);
        add_last_value(input);

    	// Compute hidden units
	    float hidden_output[PT::HIDDEN_UNITS];
	    for(unsigned int i = 0; i < PT::HIDDEN_UNITS; i++) {
            float sum = PT::HIDDEN_BIASES[i];
            for(int j = 0; j < PT::LAG_INPUTS; j++) {
                sum += _lag_inputs[j] * PT::HIDDEN_WEIGHTS[i * PT::LAG_INPUTS + j];
            }
	     	hidden_output[i] = hidden_activation_function(sum);
	    }

	    // Compute output unit
	    float output = PT::OUTPUT_BIAS;
	    for(unsigned int i = 0; i < PT::HIDDEN_UNITS; i++) {
	      output += hidden_output[i] * PT::OUTPUT_WEIGHTS[i];
	    }
        output = output_activation_function(output);

        // Denormalize, if necessary, and return prediction
        if(PT::NORMALIZATION) output = denormalize(output);
	    return output;
    }

private:
    inline void add_last_value(Type last_value) {
        for(unsigned int i = 1; i < PT::LAG_INPUTS; i++)
            _lag_inputs[i - 1] = _lag_inputs[i];

        _lag_inputs[PT::LAG_INPUTS - 1] = last_value;
    }

    inline float normalize(float value) {
        return (value - PT::NORMALIZATION_MIN) / (PT::NORMALIZATION_MAX - PT::NORMALIZATION_MIN);
    }

    inline float denormalize(float value) {
        return value * (PT::NORMALIZATION_MAX - PT::NORMALIZATION_MIN) + PT::NORMALIZATION_MIN;
    }

	float hidden_activation_function(float sum) { // Sigmoidal function
		return 1 / (1 + exp(-sum));
	}

	inline float output_activation_function(float sum) { // Identity function
		return sum;
	}

private:
    typedef Traits<MLP_Predictor<Type>> PT;
    Type _lag_inputs[PT::LAG_INPUTS];
    
};

__END_SYS

#endif
