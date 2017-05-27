// EPOS Predictive Smart Data Declarations

#ifndef __predictive_smart_data_h
#define __predictive_smart_data_h

#include <smart_data.h>
#include <predictor.h>

__BEGIN_SYS

// Predictive Smart Data is a Smart Data that makes use of a predictor to do trickling.
template<typename Transducer>
class Predictive_Smart_Data: public Smart_Data<Transducer>
{

public:    
    enum Predictor_Type {
        LINEAR_REGRESSION = 0,
        ELMAN_NEURAL_NETWORK = 1,
    };
    
public:
    Predictive_Smart_Data(const Smart_Data<Transducer>::Region & region, const Smart_Data<Transducer>::Microsecond & expiry, const Smart_Data<Transducer>::Microsecond & period = 0, const float acc_margin = 0, const Predictor_Type & type = LINEAR_REGRESSION)
    : Smart_Data(region, expiry, period, Smart_Data<Transducer>::ADVERTISED), _last_value(0), _acc_margin(acc_margin > 1 ? 1 : (acc_margin < 0 ? 0 : acc_margin)), _predictor(type & LINEAR_REGRESSION ? 0 : 0) {
    }
    
protected:
    void broadcast() {
        Smart_Data<Transducer>::Value predicted = _predictor->predict_next(_last_value);
        Smart_Data<Transducer>::Value real = Smart_Data<Transducer>::_value;
        
        // If predicted data is acceptable do not notify observers
        if(predicted >= real * (1 - _acc_margin) && predicted <= real * (1 + _acc_margin)) {
            _last_value = predicted;
        } else {
            _last_value = real;
            notify();
        }
    }
    
private:
    Smart_Data<Transducer>::Value _last_value;
    float _acc_margin;
    Predictor<Smart_Data<Transducer>::Value> * _predictor;
  
};

__END_SYS

#endif

#include <transducer.h>
