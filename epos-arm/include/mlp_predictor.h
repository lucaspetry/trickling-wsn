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
    MLP_Predictor() : _consecutive_reals(0) {
        for(unsigned int i = 0; i < PT::LAG_INPUTS; i++) {
            _last_values[i] = 0.0;
        }
    }
    
    Type predict_next(Type last_value, bool is_real = false) {
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
                sum += _last_values[j] * PT::HIDDEN_WEIGHTS[i * PT::LAG_INPUTS + j];
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

        if(is_real) {
            _consecutive_reals++;
        } else {
            _consecutive_reals = 0;
        }

	    return output;
    }

    // If all last values were read from sensor, then predictor is definitely reliable
    bool reliable() {
        return _consecutive_reals >= PT::LAG_INPUTS;
    }

private:
    inline void add_last_value(float last_value) {
        for(unsigned int i = 1; i < PT::LAG_INPUTS; i++) {
            _last_values[i - 1] = _last_values[i];
        }

        _last_values[PT::LAG_INPUTS - 1] = last_value;
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
    // Predictor traits
    typedef Traits<MLP_Predictor<Type>> PT;

    float _last_values[PT::LAG_INPUTS];
    unsigned int _consecutive_reals;
    
};

__END_SYS

#endif
