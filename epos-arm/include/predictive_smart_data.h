// EPOS Predictive Smart Data Declarations

#ifndef __predictive_smart_data_h
#define __predictive_smart_data_h

#include <smart_data.h>
#include <predictor.h>
#include <linear_predictor.h>
#include <mlp_predictor.h>

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
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data(dev=" << dev << ",expiry=" << expiry << ")" << endl;
        assert(P_Traits::ACC_MARGIN >= 0 && P_Traits::ACC_MARGIN <= 100);
    }
    
    Predictive_Smart_Data(const Region & region, const Microsecond & period = 0, const Mode & mode = Mode::PRIVATE)
    : Smart_Data<Transducer>(region, period, period, mode),
    _predictor(new P_Type()), _acc_margin(P_Traits::ACC_MARGIN / (float) 100),
    _last_value(0), _sync_interval(P_Traits::SYNC_INTERVAL), _trusty(true) {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data(region=" << region << ",period=" << period << ",mode=" << mode << ")" << endl;
        assert(P_Traits::ACC_MARGIN >= 0 && P_Traits::ACC_MARGIN <= 100);
    }
        
    operator Value() {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data::Value()" << endl;
        Value predicted;
        if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) { // If it's not sensor-side
            predicted = _predictor->predict_next(_last_value);
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value[!ADV]:predicted=" << predicted << endl;
        }
        
        if(Smart_Data<Transducer>::expired()) {
            if((Smart_Data<Transducer>::_device != Smart_Data<Transducer>::REMOTE) && (Transducer::POLLING)) { // Local data source
                Transducer::sense(Smart_Data<Transducer>::_device, this); // read sensor
                Smart_Data<Transducer>::_time = TSTP::now();
            } else {
                _last_value = predicted;
                
                if(P_Traits::SYNC_INTERVAL != 0 && _sync_interval == 0) {
                    _trusty = false;
                }
                if(_sync_interval > 0) _sync_interval--;
            }
        } else {
            _last_value = Smart_Data<Transducer>::_value;

            if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) // If it's not sensor-side
                _sync_interval = P_Traits::SYNC_INTERVAL;
        }
        
        if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) {
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():received=" << (!Smart_Data<Transducer>::expired()) << endl;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value():_last_value=" << _last_value << endl;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value():_sync_interval=" << _sync_interval << endl;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value():_trusty=" << _trusty << endl;
        }

        Value ret = _last_value;
        if(Smart_Data<Transducer>::_mode & Smart_Data<Transducer>::CUMULATIVE) {
            Smart_Data<Transducer>::_value = 0;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value[CUM]:_value=ZERO" << endl;
        }
        return ret;
    }

    bool trusty() { return _trusty; }
    
protected:    
    void send(const Time t, Time_Offset expiry) {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data::send(t=" << t << ",expiry=" << expiry << ")" << endl;
        Value predicted = _predictor->predict_next(_last_value);
        Value real = Smart_Data<Transducer>::_value;
        
        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():real=" << real << endl;
        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():predicted=" << predicted << endl;
        
        bool acc_dec_margin = predicted >= real * (1 - _acc_margin);
        bool acc_add_margin = predicted <= real * (1 + _acc_margin);
        bool check_sync = _sync_interval > 0 || P_Traits::SYNC_INTERVAL == 0;

        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():hit=" << (acc_dec_margin && acc_add_margin) << endl;

        // If predicted data is acceptable do not notify observers
        if(acc_dec_margin && acc_add_margin && check_sync) {
            _last_value = predicted;
            _sync_interval--;
        } else {
            _last_value = real;
            Smart_Data<Transducer>::send(t, expiry);
            _sync_interval = P_Traits::SYNC_INTERVAL;
        }

        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():_last_value=" << _last_value << endl;
        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():_sync_interval=" << _sync_interval << endl;
    }
    
private:
    typedef Traits<Predictive_Smart_Data<Transducer>> P_Traits;
    typedef typename IF<(P_Traits::PREDICTOR == P_Traits::LINEAR), Linear_Predictor<Value>, MLP_Predictor<Value>>::Result P_Type;

    Predictor<Value> * _predictor;
    float _acc_margin;
    Value _last_value;
    unsigned int _sync_interval;
    bool _trusty;
  
};

__END_SYS

#endif

#include <transducer.h>
