#include "calibration.h"
#include "pins.h"
#include <Arduino.h>

#define SENSOR_VALUE_MARGIN 100


/* Constructor */
Calibration::Calibration(/*RobotActuator *rAct, */SensorReader *sReader) {
	_state = STATE_LINE_COLOR;
	_whiteValue = (short int*)calloc(SENSOR_NUMBER, sizeof(short int));
	_blackValue = (short int*)calloc(SENSOR_NUMBER, sizeof(short int));
	//_rAct = rAct;
	_sReader = sReader;
	_subState = ZERO;
	_initBlackValue = 0;
	_initWhiteValue = 0;
}


/* Destructor */
Calibration::~Calibration() {
	free(_whiteValue);
	free(_blackValue);
}


/* Main function to make the color calibration for each sensor */
void Calibration::startCalibration(int motorPin1, int motorPin2) {


	/* Main State Machine */
	switch(_state) {

		/* State to set the line collor and the initial black and white reference values */
		case STATE_LINE_COLOR: {

			/* Calls the method that does it all */
			setLineColor();

			/* If the line is black, the low-bat led will blink once */
			if(_lineColor == Colors::BLACK) {

				digitalWrite(LOW_BATTERY_LED_PIN, HIGH);
				delay(500);
				digitalWrite(LOW_BATTERY_LED_PIN, LOW);

			/* If the line is white, the low-bat led will blink twice */
			} else {

				digitalWrite(LOW_BATTERY_LED_PIN, HIGH);
				delay(500);
				digitalWrite(LOW_BATTERY_LED_PIN, LOW);
				delay(500);
				digitalWrite(LOW_BATTERY_LED_PIN, HIGH);
				delay(500);
				digitalWrite(LOW_BATTERY_LED_PIN, LOW);

			}

			/* Changes the state */
			_state = STATE_TURN;

		} break;

		/* State to make turns and calibrate the reflectance values of each sensor */
		/* Has some code repetition, fix latter!!! */
		case STATE_TURN: {

			while(_subState != END) {

				/* Get the current sensor data */
				short int *sensorData = _sReader->getSensorDataWithUpdate();
	
					/* Iterates over all the sensor data */
					for(int i = 0; i < SENSOR_NUMBER; i++) {
	
						/* Sets the maximum white level of the specific sensor */
						if(sensorData[i] > _whiteValue[i]) {
							_whiteValue[i] = sensorData[i];
						}
	
						/* Sets the minimum white level of the specific sensor (black level of the sensor) */
						if(sensorData[i] < _blackValue[i]) {
							_blackValue[i] = sensorData[i];
						}
					}
	
					/* bool to set if the follow-line has arrived */
					bool hasArrived = true;
	
					bool turnRight;
	
					/* Get the line color reflectance value */
					short int toBeCompared = ((_lineColor == Colors::BLACK)? _initBlackValue : _initWhiteValue);
	
					switch(_subState) {
	
						case ZERO: {
	
							/* Starts the rotation of the follow-line*/
							turnRight = false;
												
							/* Iterates over all sensor data */
							for(int i = 0; i < SENSOR_NUMBER; i++) {
	
								/* If some sensor have a value close to the line color means you do not have arrived */
								if(abs(sensorData[i] - toBeCompared) < SENSOR_VALUE_MARGIN) {
									hasArrived = false;
								}
							}
	
							/* When all the sensors are off the line, it had arrived */
							if(hasArrived) {
								_subState = NINETY;
							}
							
	
						} break;
	
						case NINETY: {
	
							/* Starts the rotation of the follow-line*/
							turnRight = true;
	 	
	 							/* To invert the logic the value is the oposite */
							hasArrived = false;
	
							/* Iterates over all sensor data */
							for(int i = 0; i < SENSOR_NUMBER; i++) {
	
								/* If some sensor have a value close to the line color means you have arrived */
								if(abs(sensorData[i] - toBeCompared) < SENSOR_VALUE_MARGIN) {
									hasArrived = true;
								}
							}
	
							/* When all the sensors are off the line, it had arrived */
							if(hasArrived) {
								_subState = SECOND_ZERO;
							}
	
						} break;
	
						case SECOND_ZERO: {
	
							/* Starts the rotation of the follow-line*/
							turnRight = true;
	
							/* Iterates over all sensor data */
							for(int i = 0; i < SENSOR_NUMBER; i++) {
	
								/* If some sensor have a value close to the line color means you do not have arrived */
								if(abs(sensorData[i] - toBeCompared) < SENSOR_VALUE_MARGIN) {
									hasArrived = false;
								}
							}
	
							/* When all the sensors are off the line, it had arrived */
							if(hasArrived) {
								_subState = MINUS_NINETY;
							}
	
						} break;
	
						case MINUS_NINETY: {
	
							/* Starts the rotation of the follow-line*/
							turnRight = false;
	
							/* To invert the logic the value is the oposite */
							hasArrived = false;
	
							/* Iterates over all sensor data */
							for(int i = 0; i < SENSOR_NUMBER; i++) {
	
								/* If some sensor have a value close to the line color means you have arrived */
								if(abs(sensorData[i] - toBeCompared) < SENSOR_VALUE_MARGIN) {
									hasArrived = true;
								}
							}
	
							/* When all the sensors are off the line, it had arrived */
							if(hasArrived) {
								_subState = END;
							}
	
						} break;
	
	
						/* Turns to the right direction */
						if(turnRight) {
							//_rAct->calibrationRightTurn();
							turnRight = true;
						} else {
							//_rAct->calibrationLeftTurn();
							turnRight = false;
						}
				}
			}
		} break;
	}
}


/* Returns the white values of each sensor */
short int* Calibration::whiteValue() {
	return _whiteValue;
}


/* Returns the black values of each sensor */
short int* Calibration::blackValue() {
	return _blackValue;
}


/* Function to set an initial value to black and white and to say the color of the line */
void Calibration::setLineColor() {

	/* Temp variables */
	short int centerSensorValue;
	short int edgesMeanSensorValue;

	/* Get the current sensor data */
	short int *sensorData = _sReader->getSensorDataWithUpdate();

	/* Get the value of the middle sensor */
	centerSensorValue = sensorData[int(SENSOR_NUMBER/2)];

	/* Get the mean value of the edges sensors */
	edgesMeanSensorValue = (sensorData[0]+sensorData[SENSOR_NUMBER-1])/2;

	/* Sets the values according to the color of the line */
	if(centerSensorValue > edgesMeanSensorValue) {
		_lineColor = Colors::WHITE;
		_initWhiteValue = centerSensorValue;
		_initBlackValue = edgesMeanSensorValue;
	} else {
		_lineColor = Colors::BLACK;
		_initWhiteValue = edgesMeanSensorValue;
		_initBlackValue = centerSensorValue;
	}
}