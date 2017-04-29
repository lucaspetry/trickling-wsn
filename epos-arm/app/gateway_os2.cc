#include <smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 10 * 1000000;
const TSTP::Time DATA_EXPIRY = 2 * DATA_PERIOD;
const TSTP::Time INTEREST_EXPIRY = 5 * 60 * 1000000;

int main()
{
    // Interest center points
    TSTP::Coordinates center_lux(300, 320, 40);
    TSTP::Coordinates center_lamp(-140, 50, 250);

    // Regions of interest
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region_lux(center_lux, 50, start, end);
    TSTP::Region region_lamp(center_lamp, 50, start, end);

    // Data of interest
    Luminous_Intensity data_lux(region_lux, DATA_EXPIRY, DATA_PERIOD);
    Luminous_Intensity data_lamp(region_lamp, DATA_EXPIRY, DATA_PERIOD);

    OStream cout;

    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Luminous_Intensity::Value lux;
        Current::Value set;
        
        lux = data_lux;
	
//        if(lux == 0)
             set = 0;
  //      else
    //         set = 1;

        data_lamp = set;
	cout << lux << endl;
    }

    cout << ""  << endl;

    return 0;
}
