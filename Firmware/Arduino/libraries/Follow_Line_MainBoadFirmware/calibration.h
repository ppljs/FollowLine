#ifndef CALIBRATION_H
#define CALIBRATION_H

//#include "robot_actuator.h"
#include "sensor_reader.h"
#include "constants.h"


/* Class with the objective of calibrate the white and black levels of all sensors */
class Calibration {
public:

	/* Constructor */
	Calibration(/*RobotActuator *rAct, */SensorReader *sReader);

	/* Object destructor */
	~Calibration();

	/* Core function to calibrate the levels of the white and black, and to find the colour of the line */
	void startCalibration(int motorPin1, int motorPin2);

	/* getters for the value of the reflection on the white color and for the black value */
	short int *whiteValue();
	short int *blackValue();

private:

	/* States for the startCalibration function state machine */
	enum {STATE_LINE_COLOR, STATE_TURN};
	/* Holds the current state*/
	short int _state;

	enum {ZERO, NINETY, SECOND_ZERO, MINUS_NINETY, END};
	short int _subState;

	/* Hold the value of the reflection of the white colour and from the black colour for each sensor */
	short int *_whiteValue;
	short int *_blackValue;

	short int _initBlackValue;
	short int _initWhiteValue;

	/* Get access to the robot actuator class and the sensor reader class */
	//RobotActuator *_rAct;
	SensorReader *_sReader;

	/* Stores the color of the Line */
	Colors::Color _lineColor;

	/* Function to set the color of the field line */
	void setLineColor();

};

#endif /* CALIBRATION_H */ 