// 
// 
// 

#include "ArmControl.h"

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
		computedAcceleration[i] = (thetaDesired[i] - thetaFeedback[i]) * KP + (dThetaDesired[i] - dThetaFeedback[i]) * KV + ddThetaDesired[i];
	}

	//Some stuff here from the dynamics calculating the torque
	array<double, 3> torque = { 0, 0, 0 }; //TODO: Provide method here

	return torque;
}