// ConsoleTest.cpp : Defines the entry point for the console application.
//

#include "CrustCrawler/ArmTrajectory.h"
#include "CrustCrawler/ArmKinematics.h"
#include "CrustCrawler/Models/Point3D.h"
#include "CrustCrawler/Models/KinematicInverseAngles.h"
#include <iostream>
#include <intrin.h>
#include <random>
#include <time.h>

using namespace std;

int main()
{
	//array<double, 3> startAngles = { 0,0,-0.3 };
	//array<double, 3> goalAngles = { 0,0,1.5};
	//array<double, 3> goalAccelerations = { 0,0,1 };
	//array<double, 3> goalVelocities = { 0,0,2 };
	//int desiredTime = 3000;
	//ArmTrajectory trajectory;

	////trajectory.startContinousMove(startAngles, goalAccelerations, goalVelocities);
	//trajectory.setNewGoal(startAngles, goalAngles, goalAccelerations, desiredTime);

	//while (trajectory.goalReachedFlag == 0)
	//{
	//	trajectory.time += 50;
	//	//trajectory.calculateContinousMove();
	//	trajectory.calculate();
	//	std::cout << trajectory.output[2][0] <<"     "<<trajectory.output[2][1]<<"     "<<trajectory.output[2][2]<< std::endl;
	//	//if (trajectory.time > 5000) trajectory.stopContinousMove();
	//}
	//int a = 1;

	ArmKinematics kine;
	Point3D<double> p;
	//cout << p.getX() << "   " << p.getY() << "   " << p.getZ() << endl;
	double x, y, z;

	KinematicInverseAngles inv;

	Point3D<double> p2;
	Point3D<double> p3;
	srand(time(NULL));
	double r1, r2, r3;
		
	for (int i = 0; i < 1000000; i++)
	{
		r1 = rand()*0.00005 - 1;
		r2 = rand()*0.00005 - 1;
		r3 = rand()*0.00005 - 1;
		while (r3 == 0) 
		{
			r3 = rand()*0.00005 - 1;
		}
		r3 = 1.2;
		r2 = 0;
		r1 = -4;
		p = kine.ForwardKinematics(r1,r2,r3);
		//cout << p.getX() << "   " << p.getY() << "   " << p.getZ() << endl;
		x = p.getX();
		y = p.getY();
		z = p.getZ();
		Point3D<double> pInput = Point3D<double>(-2450, 0,555);
		inv = kine.InverseKinematics(pInput);
		bool t = isnan(inv.SolutionOne[2]);
		double a = atan2(-8, 3);

		p2 = kine.ForwardKinematics(inv.SolutionOne[0], inv.SolutionOne[1], inv.SolutionOne[2]);
		p3 = kine.ForwardKinematics(inv.SolutionTwo[0], inv.SolutionTwo[1], inv.SolutionTwo[2]);
		if ((abs(p2.getX()-x)>10 || abs(p2.getY() - y) > 10 || abs(p2.getZ() - z) > 10) || (abs(p3.getX() - x) > 10 || abs(p3.getY() - y) > 10 || abs(p3.getZ() - z) > 10))
		{
			//__debugbreak();
		}
		if (inv.SolutionOne[2] == 0 || inv.SolutionTwo[2] == 0)
		{
			
		}
	}	

	
    return 0;
}

