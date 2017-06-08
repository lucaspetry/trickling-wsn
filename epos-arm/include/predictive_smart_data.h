// EPOS Predictive Smart Data Declarations

#ifndef __predictive_smart_data_h
#define __predictive_smart_data_h

#include <smart_data.h>
#include <predictor.h>
#include <linear_predictor.h>

#include <utility/ostream.h>

__BEGIN_SYS

// Predictive Smart Data is a Smart Data that makes use of a predictor to do trickling.
template<typename Transducer>
class Predictive_Smart_Data: public Smart_Data<Transducer>
{
public:
    typedef typename TSTP::Unit::Get<Transducer::NUM>::Type Value;
    
    typedef RTC::Microsecond Microsecond;

    typedef TSTP::Region Region;
    typedef TSTP::Time Time;
    typedef TSTP::Time_Offset Time_Offset;
    typedef typename Smart_Data<Transducer>::Mode Mode;
    
public:
    Predictive_Smart_Data(unsigned int dev, const Microsecond & expiry)
    : Smart_Data<Transducer>(dev, expiry, Smart_Data<Transducer>::ADVERTISED),
    _predictor(new P_Type()), _acc_margin(P_Traits::ACC_MARGIN / (float) 100), 
    _last_value(0), _sync_interval(P_Traits::SYNC_INTERVAL), _trusty(true) {
        assert(P_Traits::ACC_MARGIN >= 0 && P_Traits::ACC_MARGIN <= 100);
    }
    
    Predictive_Smart_Data(const Region & region, const Microsecond & period = 0, const Mode & mode = Mode::PRIVATE)
    : Smart_Data<Transducer>(region, period, period, mode),
    _predictor(new P_Type()), _acc_margin(P_Traits::ACC_MARGIN / (float) 100),
    _last_value(0), _sync_interval(P_Traits::SYNC_INTERVAL), _trusty(true) {
        assert(P_Traits::ACC_MARGIN >= 0 && P_Traits::ACC_MARGIN <= 100);
    }
        
    operator Value() {
        Value predicted;
        if(Smart_Data<Transducer>::_mode != Smart_Data<Transducer>::ADVERTISED) { // TODO(LUCAS) - Verificar com Cesar
            predicted = _predictor->predict_next(_last_value);
            cout << "Predicted: " << predicted << "\n"; // TODO(LUCAS)
        }
        
        if(Smart_Data<Transducer>::expired()) {
            if((Smart_Data<Transducer>::_device != Smart_Data<Transducer>::REMOTE) && (Transducer::POLLING)) { // Local data source
                Transducer::sense(Smart_Data<Transducer>::_device, this); // read sensor
                Smart_Data<Transducer>::_time = TSTP::now();
            } else {
                // TODO(LUCAS) - PREDICT
                // Assumes predicted value is good
                _last_value = predicted;
                
                cout << "Predictive_Smart_Data::Value()\n"; // TODO(LUCAS)
                if(P_Traits::SYNC_INTERVAL != 0 && _sync_interval == 0) {
                    _trusty = false;
                }
                _sync_interval--;
            }
        } else {
            _sync_interval = P_Traits::SYNC_INTERVAL;
            _last_value = Smart_Data<Transducer>::_value;
        }

        Value ret = _last_value;
        if(Smart_Data<Transducer>::_mode & Smart_Data<Transducer>::CUMULATIVE) {
            Smart_Data<Transducer>::_value = 0; 
            cout << "Value set to ZERO!\n";
        }
        return ret;
    }

    bool trusty() { return _trusty; }
    
protected:    
    void send(const Time t, Time_Offset expiry) {
        Value predicted = _predictor->predict_next(_last_value);
        Value real = Smart_Data<Transducer>::_value;
        
        cout << "Predictive_Smart_Data::should_send()\n"; // TODO(LUCAS)
        cout << "Real:      " << real << "\n"; // TODO(LUCAS)
        cout << "Predicted: " << predicted << "\n"; // TODO(LUCAS)
        
        bool acc_dec_margin = predicted >= real * (1 - _acc_margin);
        bool acc_add_margin = predicted <= real * (1 + _acc_margin);
        bool check_sync = _sync_interval > 0 || P_Traits::SYNC_INTERVAL == 0;

        // If predicted data is acceptable do not notify observers
        if( acc_dec_margin && acc_add_margin && check_sync) {
            _last_value = predicted;
            _sync_interval--;
            cout << ":: Observers won't be notified!\n"; // TODO(LUCAS)
        } else {
            _last_value = real;
            cout << ":: Observers will be notified!\n"; // TODO(LUCAS)
            Smart_Data<Transducer>::send(t, expiry);
            _sync_interval = P_Traits::SYNC_INTERVAL;
        }
    }
    
private:
    typedef Traits<Predictive_Smart_Data<Transducer>> P_Traits;
    typedef typename IF<(P_Traits::PREDICTOR == P_Traits::LINEAR), Linear_Predictor<Value>, Linear_Predictor<Value>>::Result P_Type;

    Predictor<Value> * _predictor;
    float _acc_margin;
    Value _last_value;
    unsigned int _sync_interval;
    bool _trusty;
    EPOS::OStream cout;
  
};

__END_SYS

#endif

#include <transducer.h>
