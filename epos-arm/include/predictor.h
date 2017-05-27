// EPOS Predictor Declarations

#ifndef __predictor_h
#define __predictor_h

__BEGIN_SYS

template<typename T>
class Predictor
{
public:
    virtual T predict_next(T last_value) = 0;
};

__END_SYS

#endif
