#include "sensor_reader.h"
#include "pins.h"
#include "constants.h"
#include <Arduino.h>

SensorReader::SensorReader() {
	_sensorData = (short int*)malloc(SENSOR_NUMBER*sizeof(short int));
}

short int* SensorReader::getSensorDataWithUpdate() {
	_sensorData[0] = analogRead(SENSOR_PIN_FL);
	_sensorData[1] = analogRead(SENSOR_PIN_L);
	_sensorData[2] = analogRead(SENSOR_PIN_M);
	_sensorData[3] = analogRead(SENSOR_PIN_R);
	_sensorData[4] = analogRead(SENSOR_PIN_FR);

	return _sensorData;
}

short int* SensorReader::getSensorData() {
	return _sensorData;
}