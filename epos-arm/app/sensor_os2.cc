#include <alarm.h>
#include <gpio.h>
#include <transducer.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "LED BLINK\n\n";
    
    Switch_Sensor sensor(0, 'C', 3, GPIO::OUT);
    Switch data(0, 100000000);
  
    bool light = false;

    while(true) {
      Alarm::delay(5000000); // 10 seconds
      
      Switch::Value v;
      v = data;
      cout << "Led: " << v << endl;
      
      sensor.set(light);
      light = !light;      
    }
    return 0;
}
