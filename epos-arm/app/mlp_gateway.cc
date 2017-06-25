#include <predictive_smart_data.h>
#include <mlp_predictor.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 8 * 1000000;
const TSTP::Time INTEREST_EXPIRY = 5 * 60 * 1000000;

int main()
{
    OStream cout;
    cout << "\n\n\n=======================   GATEWAY START   =======================" << endl;
    cout << "[ GATEWAY ]    MLP Predictor - Initializing..." << endl;

    // Region of interest (Temperature Sensor)
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region_temp(TSTP::Coordinates(1, 1, 1), 10, start, end);

    // Data of interest
    Predictive_Smart_Data<Stub_Sensor> data(region_temp, DATA_PERIOD);
    cout << "[ GATEWAY ]    Location at: " << data.location() << endl;
    cout << "[ GATEWAY ]    Interest:    " << region_temp << endl;
    cout << "\n\n" << endl;

    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Temperature::Value v;
        
        v = data;
	    cout << "[ GATEWAY ]    Data read: " << v << " (Trusty: " << data.trusty() << ")\n" << endl;
    }

    return 0;
}
