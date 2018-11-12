// ConsoleTest.cpp : Defines the entry point for the console application.
//

#include "CrustCrawler/ArmTrajectory.h"
#include <iostream>

int main()
{
	array<double, 3> startAngles = { 0,0,-0.3 };
	array<double, 3> goalAngles = { 0,0,1.5};
	array<double, 3> goalAccelerations = { 0,0,1 };
	array<double, 3> goalVelocities = { 0,0,2 };
	int desiredTime = 3000;
	ArmTrajectory trajectory;

	//trajectory.startContinousMove(startAngles, goalAccelerations, goalVelocities);
	trajectory.setNewGoal(startAngles, goalAngles, goalAccelerations, desiredTime);

	while (trajectory.goalReachedFlag == 0)
	{
		trajectory.time += 50;
		//trajectory.calculateContinousMove();
		trajectory.calculate();
		std::cout << trajectory.output[2][0] <<"     "<<trajectory.output[2][1]<<"     "<<trajectory.output[2][2]<< std::endl;
		//if (trajectory.time > 5000) trajectory.stopContinousMove();
	}
	int a = 1;
    return 0;
}

