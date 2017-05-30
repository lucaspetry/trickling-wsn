// EPOS Predictor Declarations

#ifndef __predictor_h
#define __predictor_h

__BEGIN_SYS

template<typename Type>
class Predictor
{
public:
    virtual Type predict_next(Type last_value) = 0;
};

__END_SYS

#endif
