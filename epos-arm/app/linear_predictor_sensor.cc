#include <predictive_smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 10 * 1000000;

int main()
{
    OStream cout;
    cout << "Temperature Sensor\n" << endl;
    cout << "[ SENSOR ]  " << TSTP::now() << "  Pass: Initializing..." << endl;

    Temperature_Sensor sensor;
    Predictive_Smart_Data<Temperature_Sensor> data(0, 100000000);
    cout << "[ SENSOR ]  " << TSTP::now() << "  Pass: Sensor" << endl;
    cout << "[ SENSOR ]  " << TSTP::now() << "  Location at: " << data.location() << endl;
  
    while(true) {
        Alarm::delay(DATA_PERIOD);

	    cout << "[ SENSOR ]  " << TSTP::now() << "  Still running" << endl;      
        cout << "[ SENSOR ]  " << TSTP::now() << "  Location at: " << data.location() << endl;
    }
  
    return 0;
}