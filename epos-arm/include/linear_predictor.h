// EPOS Linear Regression Predictor Declarations

#ifndef __linear_predictor_h
#define __linear_predictor_h

#include <predictor.h>

__BEGIN_SYS

template<typename Type>
class Linear_Predictor: public Predictor<Type>
{
public:
    Linear_Predictor() {
        _current_idx = 0;
        _t = 0;
        _m = Traits<Linear_Predictor<Type>>::M;
        _b = Traits<Linear_Predictor<Type>>::B;
    }
    
    Type predict_next(Type last_value) {
        db<Linear_Predictor>(TRC) << "Linear_Predictor::predict_next(last_value=" << last_value << ")" << endl;
        Type next_value = _m * _t + _b;

        add_last_value(last_value);    
        update_coefficients();
        _t++;

        return next_value;    
    }
    
private:
    void add_last_value(Type last_value) {
        if(_current_idx >= WINDOW_SIZE) {
            for(unsigned int i = 1; i < WINDOW_SIZE; i++)
                _data_window[i - 1] = _data_window[i];

            _current_idx = WINDOW_SIZE - 1;
        }

        _data_window[_current_idx++] = last_value;
    }
  
    void update_coefficients() {
        float b = _b;
        float m = _m;

        for(int i = 0; i < GD_ITERATIONS; i++) {
            float b_gradient = 0;
            float m_gradient = 0;

            for(int j = 0; j < _current_idx; j++) {
                int x = _t - _current_idx + j;
                float y = _data_window[j];

                m_gradient += -(2.0/_current_idx) * x * (y - ((m * x) + b));
                b_gradient += -(2.0/_current_idx) * (y - ((m * x) + b));

            }

            m = m - (LRATE * m_gradient);
            b = b - (LRATE * b_gradient);
        }

        _m = m;
        _b = b;
    }
  
private:
    static const unsigned int WINDOW_SIZE = Traits<Linear_Predictor<Type>>::WINDOW_SIZE;
    static const float LRATE = Traits<Linear_Predictor<Type>>::LRATE;
    static const unsigned short GD_ITERATIONS = Traits<Linear_Predictor<Type>>::GD_ITERATIONS;
    Type _data_window[WINDOW_SIZE];
    unsigned int _current_idx;
    unsigned long _t;
    float _m;
    float _b;
    
};

__END_SYS

#endif
