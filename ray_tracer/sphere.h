#ifndef Sphere_H_
#define Sphere_H_
#include "vecteur.h"
#include "Ray.h"

class Sphere
{
	
	public : 
		Sphere(const Vec3& ctr, double rad, Vec3& clr,double ec);

		Sphere(const Vec3& ctr, double rad, Vec3& clr,double ec,std::string name) ;
		Vec3 GetNormal(const Vec3& pi) const;

		bool Intersects(const Ray& ray, double& t) const;

		bool operator!=(const Sphere& s) const;
		bool operator==(const Sphere& s) const;
		
		Vec3 center;
		double radius;
		Vec3 clr;
		std::string name;
		double eclat;
	
};
#endif


