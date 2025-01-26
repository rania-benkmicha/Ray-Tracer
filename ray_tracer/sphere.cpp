#include <iostream>
#include <math.h>
#include "vecteur.h"
#include "Ray.h"
#include "sphere.h"

Sphere::Sphere(const Vec3& ctr, double rad, Vec3& clr,double ec) :
		center(ctr), radius(rad), clr(clr),eclat(ec) {}

Sphere::Sphere(const Vec3& ctr, double rad, Vec3& clr,double ec, std::string name) :
		center(ctr), radius(rad), clr(clr), name(name),eclat(ec) {}

Vec3 Sphere::GetNormal(const Vec3& pi) const
	{
		return (pi - center) / radius;
	}

bool Sphere::Intersects(const Ray& ray, double& t) const
	{
		const Vec3 origin = ray.getorigin();
		const Vec3 dest = ray.getdirection();
		const Vec3 orgCtr = origin - this->center;
		const double b = 2 * dot(orgCtr, dest);
		const double center = dot(orgCtr, orgCtr) - radius * radius;
		double discrim = b * b - 4 * center;

		if (discrim < 1e-4)
			return false;

		discrim = sqrt(discrim);
		const double t0 = -b - discrim;
		const double t1 = -b + discrim;

		t = (t0 < t1) ? t0 : t1;

		return true;
	}

bool Sphere::operator!=(const Sphere& s) const
	{
		if (s.center.x == center.x && s.center.y == center.y && s.center.z == center.z)
			return false;

		return true;
	}
bool Sphere:: operator==(const Sphere& s) const
	{
		if (s.center.x == center.x && s.center.y == center.y && s.center.z == center.z)
			return true;

		return false;
	}

