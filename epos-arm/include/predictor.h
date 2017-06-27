// EPOS Predictor Declarations

#ifndef __predictor_h
#define __predictor_h

__BEGIN_SYS

template<typename Type>
class Predictor
{
public:
    virtual Type predict_next(Type last_value, bool is_real = false) = 0;
    virtual bool reliable() { return false; };
};

__END_SYS

#endif
