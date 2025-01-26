#include <iostream>
#include "vecteur.h"
#include <math.h>
Vec3::Vec3() {}

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3 Vec3 :: operator+(const Vec3& v) const
{
			return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3 :: operator+(const int num) const
		{
			return Vec3(x + num, y + num, z + num);
		}

Vec3 Vec3 :: operator-(const Vec3& v) const
		{
			return Vec3(x - v.x, y - v.y, z - v.z);
		}

Vec3 Vec3 :: operator-(const double val) const
		{
			return Vec3(x - val, y - val, z - val);
		}

Vec3 Vec3 :: operator*(double mul) const
		{
			return Vec3(x * mul, y * mul, z * mul);
		}

Vec3 Vec3 :: operator*(const Vec3 v) const
		{
			return Vec3(v.x * x, v.y * y, v.z * z);
		}

Vec3 Vec3 :: operator/(double div) const
		{
			return Vec3(x / div, y / div, z / div);
		}

Vec3 Vec3 ::  operator/(const Vec3& div) const
		{
			return Vec3(x / div.x, y / div.y, z / div.z);
		}

bool Vec3:: operator==(const Vec3& v1)
		{
			if (v1.x == x && v1.y == y && v1.z == z)
				return true;

			return false;
		}
bool Vec3:: operator!=(const Vec3& v1)
		{
			if (v1.x != x && v1.y != y && v1.z != z)
				return true;

			return false;
		}

bool Vec3:: operator>(const Vec3& v1)
		{
			if (v1.x > x && v1.y > y && v1.z > z)
				return true;

			return false;
		}

bool Vec3:: operator<(const Vec3& v1)
		{
			if (v1.x < x && v1.y < y && v1.z < z)
				return true;

			return false;
		}

Vec3 Vec3 :: Normalize() const
		{
			double magnitude = sqrt(x * x + y * y + z * z);
			return Vec3(x / magnitude, y / magnitude, z / magnitude);
		}


