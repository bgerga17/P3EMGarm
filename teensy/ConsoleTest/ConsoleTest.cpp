// ConsoleTest.cpp : Defines the entry point for the console application.
//

#include "CrustCrawler\ArmKinematics.h"
#include "CrustCrawler\Models\Point3D.h"

int main()
{
	ArmKinematics kinematics;
	Point3D<double> point = Point3D<double>(-357.089, 0, 597.089);
	auto result = kinematics.InverseKinematics(point);
    return 0;
}

