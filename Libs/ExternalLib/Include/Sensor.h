
#ifndef SENSOR_H
#define SENSOR_H


#define NUM_SENSORS 18


typedef enum SensorType_t {
	DIGITAL,
	ANALOG
} SensorType;

typedef enum TriggerType_t {
	RISING = 0,
	FALLING = 1,
	ACTIVE_LOW = 0, 
	ACTIVE_HIGH = 1
} TriggerType;

typedef struct Sensor_t {
	SensorType type;
	char name[16];
	int enabled;
	int address;
	int threshold;
	TriggerType triggerType;
	int triggered;
	int timeTriggered;
	int value;
} Sensor;

#endif

