#include <predictive_smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 10 * 1000000;
const TSTP::Time INTEREST_EXPIRY = 5 * 60 * 1000000;

int main()
{
    OStream cout;
    cout << "[ GATEWAY ]  " << TSTP::now() << "  Pass: Initializing..." << endl;
  
    // Region of interest (Temperature Sensor)
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region_temp(TSTP::Coordinates(1, 1, 1), 10, start, end);
    cout << "[ GATEWAY ]  " << TSTP::now() << "  Pass: Region" << endl;

    // Data of interest
    Predictive_Smart_Data<Stub_Sensor> data(region_temp, DATA_PERIOD);
    cout << "[ GATEWAY ]  " << TSTP::now() << "  Pass: Interest" << endl;
    cout << "[ GATEWAY ]  " << TSTP::now() << "  Location at: " << data.location() << endl;

    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Temperature::Value v;
        
        v = data;
	    cout << "[ GATEWAY ]  " << TSTP::now() << "  Data read: " << v << " (Trusty: " << data.trusty() << ")" << endl;
        cout << "[ GATEWAY ]  " << TSTP::now() << "  Location at: " << data.location() << endl;
    }

    return 0;
}
