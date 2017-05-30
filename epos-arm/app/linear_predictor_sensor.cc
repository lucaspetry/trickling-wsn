#include <alarm.h>
#include <transducer.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

int main()
{
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + 5 * 60 * 1000000;
  
    TSTP::Coordinates sensor_coord(300, 320, 40);
    TSTP::Region region_sensor(sensor_coord, 50, start, end);
  
    Switch_Sensor sensor(0, 'C', 3, GPIO::OUT);
    Switch_Test data(region_sensor, 100000000, 100);

    while(true);
    return 0;
}
