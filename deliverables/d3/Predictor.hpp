#ifndef PREDICTOR_HPP_
#define PREDICTOR_HPP_

template<class Type>
class Predictor {

public:
  virtual Type predictNext(Type lastValue) = 0;
  
};

#endif // PREDICTOR_HPP_