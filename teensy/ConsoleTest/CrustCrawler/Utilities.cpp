#include "Utilities.h"


Utilities::Utilities()
{
}


Utilities::~Utilities()
{
}

void Utilities::LogArray(String text, array<double, 3> data)
{
	Serial.print(text);
	Serial.print(": ");
	Serial.print(data[0]);
	Serial.print("   ");
	Serial.print(data[1]);
	Serial.print("   ");
	Serial.println(data[2]);
}

void Utilities::Log(String text, double data)
{
	Serial.print(text);
	Serial.print(": ");
	Serial.println(data);
}

void Utilities::Pause()
{
	Serial.println("Paused. Send r to resume or x to reset.");
	while (!Serial.available());
	if (Serial.read() == 'x')
	{
		WRITE_RESTART(0x5FA0004);
	}
}

double Utilities::millisDouble()
{
	return (double)micros() / 1000;
}

double Utilities::secondsDouble()
{
	return millisDouble() / 1000;
}

Point3D<double> Utilities::ArrayToPoint(array<double, 3> a)
{
	return Point3D<double>(a[0], a[1], a[2]);
}

//Returns radians pr. sec
double Utilities::ConvertVelocitySignal(int16_t signal)
{
	return signal * VELOCITY_UNIT;
}

//Returns radians
double Utilities::ConvertPositionSignal(int16_t signal)
{
	return signal * POSITION_UNIT;
}

double Utilities::ComputeOutputPWM(double desiredTorque, ServoType servoType)
{
	switch (servoType)
	{
	case ServoType::MX64:
		if (desiredTorque >= 0)
		{
			return lround(209.4336*desiredTorque);
		}
		else
		{
			return lround(196.95*desiredTorque);
		}
	case ServoType::MX106:
		if (desiredTorque >= 0)
		{
			return lround(123.6096*desiredTorque);
		}
		else
		{
			return lround(131.4082*desiredTorque);
		}
	default:
		return 0.0;
	}
}