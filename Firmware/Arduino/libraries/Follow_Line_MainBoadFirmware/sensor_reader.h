#ifndef SENSOR_READER_H 
#define SENSOR_READER_H

class SensorReader {

public:
	SensorReader();
	short int* getSensorDataWithUpdate();
	short int* getSensorData();

private:
	short int* _sensorData;

};

#endif /* SENSOR_READER_H */