// EPOS Predictive Smart Data Declarations

#ifndef __predictive_smart_data_h
#define __predictive_smart_data_h

#include <smart_data.h>
#include <predictor.h>
#include <linear_predictor.h>

#include <utility/ostream.h>

using namespace EPOS;

__BEGIN_SYS

// Predictive Smart Data is a Smart Data that makes use of a predictor to do trickling.
template<typename Transducer>
class Predictive_Smart_Data: public Smart_Data<Transducer>
{
// TODO(LUCAS) - Copiei isso mas não deveria
public:
    static const unsigned int UNIT = Transducer::UNIT;
    static const unsigned int NUM = Transducer::NUM;
    static const unsigned int ERROR = Transducer::ERROR;
    typedef typename TSTP::Unit::Get<NUM>::Type Value;

    typedef RTC::Microsecond Microsecond;

    typedef TSTP::Unit Unit;
    typedef TSTP::Error Error;
    typedef TSTP::Coordinates Coordinates;
    typedef TSTP::Region Region;
    typedef TSTP::Time Time;
    typedef TSTP::Time_Offset Time_Offset;
    
public:
    Predictive_Smart_Data(unsigned int dev, const Microsecond & expiry)
    : Smart_Data<Transducer>(dev, expiry, Smart_Data<Transducer>::ADVERTISED), _last_value(0) {
        cout << "Built Predictive_Smart_Data: Const 1.\n"; // TODO(LUCAS)
        if(Traits<Predictive_Smart_Data<Transducer>>::PREDICTOR == Traits<Predictive_Smart_Data<Transducer>>::LINEAR_REGRESSION)
            _predictor = new Linear_Predictor<Value>();
             
        if(Traits<Predictive_Smart_Data<Transducer>>::ACC_MARGIN > 1)
            _acc_margin = 1;
        else if(Traits<Predictive_Smart_Data<Transducer>>::ACC_MARGIN < 0)
            _acc_margin = 0;
        else
            _acc_margin = Traits<Predictive_Smart_Data<Transducer>>::ACC_MARGIN;
    }
    
    Predictive_Smart_Data(const Region & region, const Microsecond & expiry, const Microsecond & period = 0)
    : Smart_Data<Transducer>(region, expiry, period, Smart_Data<Transducer>::ADVERTISED), _last_value(0) {
        cout << "Built Predictive_Smart_Data: Const 2.\n"; // TODO(LUCAS)
        if(Traits<Predictive_Smart_Data<Transducer>>::PREDICTOR == Traits<Predictive_Smart_Data<Transducer>>::LINEAR_REGRESSION)
            _predictor = new Linear_Predictor<Value>();
             
        if(Traits<Predictive_Smart_Data<Transducer>>::ACC_MARGIN > 1)
            _acc_margin = 1;
        else if(Traits<Predictive_Smart_Data<Transducer>>::ACC_MARGIN < 0)
            _acc_margin = 0;
        else
            _acc_margin = Traits<Predictive_Smart_Data<Transducer>>::ACC_MARGIN;
    }
    
protected:
    void broadcast() {
        Value predicted = _predictor->predict_next(_last_value);
        Value real = Smart_Data<Transducer>::_value;
        
        cout << "Real:      " << real << "\n"; // TODO(LUCAS)
        cout << "Predicted: " << predicted << "\n"; // TODO(LUCAS)
            
        // If predicted data is acceptable do not notify observers
        if(predicted >= real * (1 - _acc_margin) && predicted <= real * (1 + _acc_margin)) {
            _last_value = predicted;
            cout << ":: Observers won't be notified!\n"; // TODO(LUCAS)
        } else {
            _last_value = real;
            cout << ":: Observers will be notified!\n"; // TODO(LUCAS)
            Smart_Data<Transducer>::notify();
        }
    }
    
private:
    float _acc_margin;
    Predictor<Value> * _predictor;
    Value _last_value;
    OStream cout;
  
};

__END_SYS

#endif

#include <transducer.h>
