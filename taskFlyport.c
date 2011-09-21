#include "taskFlyport.h"
#include "Sensor.h"
#include "Config.h"
#include "LCDutils.h"
#include <time.h>
#include <string.h>

Sensor sensors[NUM_SENSORS];

int alarmGreen = 17;
int alarmRed = 6;
static int n = 0;
int triggered(int value, SensorType st, TriggerType tt, int th)
{
	if (st == DIGITAL) {
		if (tt == ACTIVE_LOW) {
			return value;
		} else {
			return !value;
		}
	} else { //ANALOG
		if (tt == RISING){
			return value > th ? 1 : 0;
		} else {
			return value <= th ? 1 : 0;
		}
	}
}

void alarm()
{
	if (n == 0) {
		IOPut(alarmGreen, 1);
		IOPut(alarmRed, 0);
		LCDWrite("Intrusion Detected!!");
	} else {
		if ((n % 30) == 0) {
			IOPut(alarmGreen, toggle);
			IOPut(alarmRed, toggle);
			n = 1;
		}
	}
	n++;
}

void FlyportTask()
{
	char buf[10];
	int i;
	int intrusion = 0;
	
	IOInit(p7, inup);
		
	IOInit(p9, inup);
	//loadConfig();
	
	WFConnect(WF_DEFAULT);
	UARTInit(2,115200);
	UARTOn(2);
	vTaskDelay(50);
	UARTWrite(1,"Started!!!\r\n");
	//memset(sensors, 0, (sizeof (Sensor))*NUM_SENSORS);
	saveConfig();
	ClearScreen();
	LCDWrite("Puppaaaa!!!!\n");
	while(1) {
		// read sensors status and update sensors structure.
		for (i = 0 ; i < NUM_SENSORS ; i++) {
			Sensor *s = &sensors[i];
			int value;
			int r;
			//r = ADCVal(1);
			//sprintf(buf, "%d\r\n", r);
			//UARTWrite(1,buf);
			if (s->enabled) { 
				// XXX Analog I/O
				if (s->type == DIGITAL)
					value = IOGet(s->address);
				else
					value = ADCVal(s->address);
				//sprintf(buf, "%d\r\n", )
				s->triggered = triggered(value, s->type, s->triggerType, s->threshold);
				s->value = value;
				if (s->triggered) {
					intrusion = 1;
				}
				
				if (intrusion) {
					alarm();
				}
			}
		}
	}
}
