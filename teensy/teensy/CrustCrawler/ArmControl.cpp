#include "ArmControl.h"

#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

ArmControl::ArmControl()
{
}

ArmControl::~ArmControl()
{
}

array<double, 3> ArmControl::ComputeControlTorque(array<double, 3> thetaDesired, array<double, 3> dThetaDesired, 
	array<double, 3> ddThetaDesired, array<double, 3> thetaFeedback, array<double, 3> dThetaFeedback)
{
	array<double, 3> computedAcceleration;
	for (int i = 0; i < 3; i++)
	{
		computedAcceleration[i] = (thetaDesired[i] - thetaFeedback[i]) * kpTemp + (dThetaDesired[i] - dThetaFeedback[i]) * kvTemp + ddThetaDesired[i];
	}
	//Some stuff here from the dynamics calculating the torque
	array<double, 3> torque = dynamics.ComputeOutputTorque(computedAcceleration, thetaFeedback, dThetaFeedback); 

	return torque;
}

//Moves with maximum acceleration and velocity
array<double, 3> ArmControl::ComputeControlTorque(array<double, 3> thetaDesired, array<double, 3> thetaFeedback, array<double, 3> dThetaFeedback)
{
	array<double, 3> acceleration = { 0,0,0 };
	array<double, 3> velocity = { 0,0,0 };
	return ComputeControlTorque(thetaDesired, acceleration, velocity, thetaFeedback, dThetaFeedback);
}

double ArmControl::ComputeOutputCurrent(double desiredTorque, ServoType servoType)
{
	switch (servoType)
	{
	case ServoType::MX28:
		return desiredTorque * K_MX28;
	case ServoType::MX64:
		return desiredTorque * K_MX64;
	case ServoType::MX106:
		return desiredTorque * K_MX106;
	default:
		return 0.0;
	}
}

double ArmControl::ComputeOutputPWM(double desiredTorque, ServoType servoType)
{

	switch (servoType)
	{
	case ServoType::MX64:
		return lround(216.85*desiredTorque);// +16;
	case ServoType::MX106:
		return lround(125.48*desiredTorque);// +18;
	default:
		return 0.0;
	}
	//if (servo_id == 1)
	//	pwm = lround(140.54*tau);
	//else if (servo_id == 2)
	//	pwm = lround(125.48*tau) + 18;
	//else if (servo_id == 3)
	//	pwm = lround(216.85*tau) + 16;
}

int16_t ArmControl::ConvertCurrentToSignalValue(double currentInAmps)
{
	double currentInMilliAmps = currentInAmps * 1000.0;
	int16_t value = (int16_t)roundf(currentInMilliAmps / (SUPPLIED_CURRENT_UNIT));
	return value; // We want torque the other way - NO
}

//Returns radians pr. sec
double ArmControl::ConvertVelocitySignal(int16_t signal)
{
	return signal * VELOCITY_UNIT;
}

//Returns radians
double ArmControl::ConvertPositionSignal(int16_t signal)
{
	return signal * POSITION_UNIT;
}

double ArmControl::ReadPositionRad(int id)
{
	int32_t pos;
	dxl.readPosition(id, pos);
	return ConvertPositionSignal(pos);
}

double ArmControl::ReadVelocityRad(int id)
{
	int32_t vel;
	dxl.readVelocity(id, vel);
	return ConvertVelocitySignal(vel);
}

array<double, 3> ArmControl::ReadPositionRadArray()
{
	array<double, 3> output;
	for (int i = 0; i < 3; i++)
	{
		int32_t pos;
		dxl.readPosition(i+1, pos);
		output[i] = ConvertPositionSignal(pos);
	}
	return output;
}

array<double, 3> ArmControl::ReadVelocityRadArray()
{
	array<double, 3> output;
	for (int i = 0; i < 3; i++)
	{
		int32_t vel;
		dxl.readVelocity(i+1, vel);
		output[i] = ConvertVelocitySignal(vel);
	}
	return output;
}

void ArmControl::SendTorquesAllInOne(array<double, 3> torques)
{
	//REMOVE LATER
	torques[0] = 0;
	int stopFlag = false;
	for (int i = 0; i < 3; i++)
	{
		if (abs(torques[i]) > 10)
		{
			Serial.print("[ERROR] Control: Overtorque on servo #");
			Serial.println(i + 1);
			stopFlag = 1;
		}
	}
	if (stopFlag)
	{
		SoftEstop();
		Serial.println("Aborting!");
		Pause();
	}
	double pwm1 = ComputeOutputPWM(torques[0], ServoType::MX106);
	double pwm2 = ComputeOutputPWM(torques[1], ServoType::MX106);
	double pwm3 = ComputeOutputPWM(torques[2], ServoType::MX64);
	dxl.setGoalPwm(1, pwm1);
	dxl.setGoalPwm(2, pwm2);
	dxl.setGoalPwm(3, pwm3);
}

bool ArmControl::CheckOverspeed(double speedLimit)
{
	bool triggered = 0;
	for (int i = 1; i < 4; i++)
	{
		if (ReadVelocityRad(i) > speedLimit)
		{
			Serial.print("WARNING: Overspeed servo #");
			Serial.println(i);
			Serial.println("Stopping all!");
			triggered = 1;
		}
	}
	if (triggered)
	{
		SoftEstop();
	}
	return triggered;
}

void ArmControl::SoftEstop()
{
	for (int j = 1; j < 4; j++)
	{
		dxl.torqueEnable(j, 0);
		dxl.setOperatingMode(j, 3);
		dxl.torqueEnable(j, 1);
	}
	Serial.println("[INFO] Control: SoftEstop has been called");
}

void ArmControl::LogArray(String text, array<double, 3> data)
{
	Serial.print(text);
	Serial.print(": ");
	Serial.print(data[0]);
	Serial.print("   ");
	Serial.print(data[1]);
	Serial.print("   ");
	Serial.println(data[2]);
}

void ArmControl::Log(String text, double data)
{
	Serial.print(text);
	Serial.print(": ");
	Serial.println(data);
}

void ArmControl::Pause()
{
	Serial.println("Paused. Send r to resume or x to reset.");
	while (!Serial.available());
	if (Serial.read() == 'x')
	{
		WRITE_RESTART(0x5FA0004);
	}
}
