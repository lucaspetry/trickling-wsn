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
    Predictive_Smart_Data(const Smart_Data<Transducer>::Region & region, const Smart_Data<Transducer>::Microsecond & expiry, const Smart_Data<Transducer>::Microsecond & period = 0)
    : Smart_Data(region, expiry, period, Smart_Data<Transducer>::ADVERTISED), _last_value(0) { }
    
protected:
    void broadcast() {
        Smart_Data<Transducer>::Value predicted = PREDICTOR->predict_next(_last_value);
        Smart_Data<Transducer>::Value real = Smart_Data<Transducer>::_value;
        
        // If predicted data is acceptable do not notify observers
        if(predicted >= real * (1 - ACC_MARGIN) && predicted <= real * (1 + ACC_MARGIN)) {
            _last_value = predicted;
        } else {
            _last_value = real;
            notify();
        }
    }
    
private:
    static const float ACC_MARGIN = Traits<Predictive_Smart_Data<T>>::ACC_MARGIN;(Traits<Predictive_Smart_Data<T>>::ACC_MARGIN > 1 ? 1 : (Traits<Predictive_Smart_Data<T>>::ACC_MARGIN < 0 ? 0 : Traits<Predictive_Smart_Data<T>>::ACC_MARGIN))
    static Predictor<Smart_Data<Transducer>::Value> * PREDICTOR = (Traits<Predictive_Smart_Data<T>>::PREDICTOR & Traits<Predictive_Smart_Data<T>>::LINEAR_REGRESSION ? new Linear_Predictor() : 0);
    Smart_Data<Transducer>::Value _last_value;
  
};

__END_SYS

#endif

#include <transducer.h>
