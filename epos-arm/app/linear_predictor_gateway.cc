#include <smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 5 * 1000000;
const TSTP::Time DATA_EXPIRY = 2 * DATA_PERIOD;
const TSTP::Time INTEREST_EXPIRY = 5 * 60 * 1000000;

int main()
{
    // Temperature Sensor
    TSTP::Coordinates data_temp(10, 10, 2);

    // Regions of interest
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region_temp(data_temp, 50, start, end);

    // Data of interest
    Temperature data(region_temp, DATA_EXPIRY, DATA_PERIOD);

    OStream cout;

    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Temperature::Value v;
        
        v = data;
	    cout << "Data read: " << v << endl;
    }

    return 0;
}
