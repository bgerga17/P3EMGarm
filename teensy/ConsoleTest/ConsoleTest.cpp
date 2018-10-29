// ConsoleTest.cpp : Defines the entry point for the console application.
//

#include "CrustCrawler\ArmKinematics.h"
#include "CrustCrawler\Models\Point3D.h"

int main()
{
	ArmKinematics kinematics;
	Point3D<double> point = Point3D<double>(242.997, -325.286, 421.529);
	auto result = kinematics.InverseKinematics(point);
    return 0;
}

