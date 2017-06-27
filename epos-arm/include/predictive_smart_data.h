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
    _predictor(new P_Type()), _acc_margin(PT::ACC_MARGIN / (float) 100),
    _last_value(0), _last_value_real(false), _sync_interval(PT::SYNC_INTERVAL),_trusty(true) {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data(dev=" << dev << ",expiry=" << expiry << ")" << endl;
        assert(PT::ACC_MARGIN >= 0 && PT::ACC_MARGIN <= 100);
    }
    
    Predictive_Smart_Data(const Region & region, const Microsecond & period = 0, const Mode & mode = Mode::PRIVATE)
    : Smart_Data<Transducer>(region, period, period, mode),
    _predictor(new P_Type()), _acc_margin(PT::ACC_MARGIN / (float) 100),
    _last_value(0), _last_value_real(false), _sync_interval(PT::SYNC_INTERVAL), _trusty(true) {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data(region=" << region << ",period=" << period << ",mode=" << mode << ")" << endl;
        assert(PT::ACC_MARGIN >= 0 && PT::ACC_MARGIN <= 100);
    }
        
    operator Value() {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data::Value()" << endl;
        Value predicted;
        if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) { // If it's not sensor-side
            predicted = _predictor->predict_next(_last_value, _last_value_real);
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value[!ADV]:predicted=" << predicted << endl;
        }
        
        if(Smart_Data<Transducer>::expired()) {
            if((Smart_Data<Transducer>::_device != Smart_Data<Transducer>::REMOTE) && (Transducer::POLLING)) { // Local data source
                Transducer::sense(Smart_Data<Transducer>::_device, this); // read sensor
                Smart_Data<Transducer>::_time = TSTP::now();
            } else {
                _last_value = predicted;
                _last_value_real = false;

                if(PT::SYNC_INTERVAL != 0 && _sync_interval == 0) {
                    _trusty = false;
                }

                if(_sync_interval > 0) _sync_interval--;
            }
        } else {
            _last_value = Smart_Data<Transducer>::_value;
            _last_value_real = true;
            if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) // If it's not sensor-side
                _sync_interval = PT::SYNC_INTERVAL;
        }
        
        if(!_trusty) {
            _trusty = _predictor->reliable();
        }

        if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) {
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():received=" << (!Smart_Data<Transducer>::expired()) << endl;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value():_last_value=" << _last_value << endl;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value():_sync_interval=" << _sync_interval << endl;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value():_trusty=" << trusty() << endl;
        }

        Value ret = _last_value;
        if(Smart_Data<Transducer>::_mode & Smart_Data<Transducer>::CUMULATIVE) {
            Smart_Data<Transducer>::_value = 0;
            db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::Value[CUM]:_value=ZERO" << endl;
        }
        return ret;
    }

    bool trusty() {
        if(Smart_Data<Transducer>::_device == Smart_Data<Transducer>::REMOTE) {
            return _trusty;
        } else {
            // sensor always is trustful
            return true;
        }
    }

    
protected:    
    void send(const Time t, Time_Offset expiry) {
        db<Predictive_Smart_Data>(TRC) << "Predictive_Smart_Data::send(t=" << t << ",expiry=" << expiry << ")" << endl;
        Value predicted = _predictor->predict_next(_last_value, _last_value_real);
        Value real = Smart_Data<Transducer>::_value;
        
        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():real=" << real << endl;
        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():predicted=" << predicted << endl;
        
        bool acc_dec_margin = predicted >= real * (1 - _acc_margin);
        bool acc_add_margin = predicted <= real * (1 + _acc_margin);
        bool check_sync = _sync_interval > 0 || PT::SYNC_INTERVAL == 0;

        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():hit=" << (acc_dec_margin && acc_add_margin) << endl;

        // If predicted data is acceptable do not notify observers
        if(acc_dec_margin && acc_add_margin && check_sync) {
            _last_value = predicted;
            _last_value_real = false;
            _sync_interval--;
        // Else notify observers
        } else {
            _last_value = real;
            _last_value_real = true;
            Smart_Data<Transducer>::send(t, expiry);
            _sync_interval = PT::SYNC_INTERVAL;
        }

        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():_last_value=" << _last_value << endl;
        db<Predictive_Smart_Data>(INF) << "Predictive_Smart_Data::send():_sync_interval=" << _sync_interval << endl;
    }
    
private:
    // Predictor traits
    typedef Traits<Predictive_Smart_Data<Transducer>> PT;
    typedef typename IF<(PT::PREDICTOR == PT::LINEAR), Linear_Predictor<Value>, MLP_Predictor<Value>>::Result P_Type;

    Predictor<Value> * _predictor;
    float _acc_margin;
    Value _last_value;
    bool _last_value_real;
    bool _trusty;
    unsigned int _sync_interval;

};

__END_SYS

#endif

#include <transducer.h>
