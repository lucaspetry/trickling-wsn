#include <predictive_smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 8 * 1000000;

int main()
{
    OStream cout;
    cout << "\n\n\n========================   SENSOR START   =======================" << endl;
    cout << "[ SENSOR ]    Initializing..." << endl;

    Stub_Sensor sensor;
    Predictive_Smart_Data<Stub_Sensor> data(0, 100000000);
    cout << "[ SENSOR ]    Location at: " << data.location() << endl;
    cout << "\n\n\n" << endl;
  
    while(true) {
        Alarm::delay(DATA_PERIOD);

        Temperature::Value v;
        
        v = data;
	    cout << "[ SENSOR ]    Real Data Read: " << v << "\n" << endl;
    }
  
    return 0;
}