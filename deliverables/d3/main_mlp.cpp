#include <iostream>
#include "mlp_predictor.hpp"

#define HIDDEN_UNITS 10

using array = std::array<float, HIDDEN_UNITS>;

int main(int argc, char* argv[]) {
	array wInput = {-0.8757484831774061, 0.638223177490687, 0.47439169507765233, -0.6241506181598983, 0.5809774819295261, -0.7266780203467283, -1.147251210290187, 0.25788441254322364};
	array biasInput = {-0.3414027283264503, -0.39535832398743515, -0.3901928811585848, -0.42176012510025584, -0.38254522241975947, -0.367525877577434, -0.2181034777366183, -0.4042664364294075};
	array wHiddenLayer = {-0.8152340897831858, 0.5940886042485971, 0.4444262681600428, -0.5257855337232896, 0.5410347272911142, -0.6329135540816879, -1.3565282423001854, 0.2553571353168649};
	float biasHidden = 0.6710505765521378;

	float minValue = 3.3;
	float maxValue = 33.3;

 	auto predictor = new MLPPredictor<float, HIDDEN_UNITS>(wInput, biasInput, wHiddenLayer, biasHidden, true, minValue, maxValue);

 	float lastValue = 16.1;
 	for(int i = 0; i < 400; i++) {
 		lastValue = predictor->predictNext(lastValue);
 		std::cout << lastValue << std::endl;	
 	}

  	return 0;
}