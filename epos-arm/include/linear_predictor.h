// EPOS Linear Regression Predictor Declarations

#ifndef __predictor_h
#define __predictor_h

#include <predictor.h>

__BEGIN_SYS

template<typename T>
class Linear_Predictor: public Predictor<T>
{
public:
    Linear_Predictor() {}
    
    T predict_next(T last_value) {
        T nextValue = M * T + B;

        add_last_value(last_value);    
        update_coefficients();
        T++;

        return nextValue;    
    }
    
private:
    void add_last_value(Type last_value) {
        if(CURRENT_IDX >= WINDOW_SIZE) {
            for(unsigned int i = 1; i < WINDOW_SIZE; i++)
                DATA_WINDOW[i - 1] = DATA_WINDOW[i];

            CURRENT_IDX = WINDOW_SIZE - 1;
        }

        DATA_WINDOW[CURRENT_IDX++] = last_value;
    }
  
    void update_coefficients() {
        float b = B;
        float m = M;

        for(int i = 0; i < GD_ITERATIONS; i++) {
            float b_gradient = 0;
            float m_gradient = 0;

            for(int j = 0; j < CURRENT_IDX; j++) {
                int x = T - CURRENT_IDX + j;
                float y = DATA_WINDOW[j];

                m_gradient += -(2.0/CURRENT_IDX) * x * (y - ((m * x) + b));
                b_gradient += -(2.0/CURRENT_IDX) * (y - ((m * x) + b));

            }

            m = m - (LRATE * m_gradient);
            b = b - (LRATE * b_gradient);
        }

        B = b;
        M = m;
    }
  
private:
    static const unsigned int WINDOW_SIZE = Traits<Linear_Predictor<T>>::WINDOW_SIZE;
    static const float LRATE = Traits<Linear_Predictor<T>>::LRATE;
    static const unsigned short GD_ITERATIONS = Traits<Linear_Predictor<T>>::GD_ITERATIONS;
    static T DATA_WINDOW[WINDOW_SIZE];
    static unsigned int CURRENT_IDX = 0;
    static unsigned long T = 0;
    static unsigned short M = Traits<Linear_Predictor<T>>::M;
    static unsigned short B = Traits<Linear_Predictor<T>>::B;
    
};

__END_SYS

#endif
