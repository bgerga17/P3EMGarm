#pragma once

#include "servo.h"
#include "Arduino.h"
#include "Utilities.h"

class ServoHelper
{
public:
	ServoHelper();
	ServoHelper(servo &dynamixel);
	~ServoHelper();
	bool CheckOverspeed(double speedLimit);
	void SoftEstop();
	void SendTorquesAllInOne(array<double, 3> torques);
	double ReadPositionRad(int id);
	double ReadVelocityRad(int id);
	array<double, 3> ReadPositionRadArray();
	array<double, 3> ReadVelocityRadArray();

private:
	servo *dynamixel;
	Utilities utilities;
};

