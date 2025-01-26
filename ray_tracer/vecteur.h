#ifndef vecteur_H_
#define vecteur_H_

class Vec3 
{

	public :
		Vec3();

		Vec3(double x, double y, double z);

		Vec3 operator+(const Vec3& v) const;
		Vec3 operator+(const int num) const;
		Vec3 operator-(const Vec3& v) const;

		Vec3 operator-(const double val) const;

		Vec3 operator*(double mul) const;
		Vec3 operator*(const Vec3 v) const;
		Vec3 operator/(double div) const;

		Vec3 operator/(const Vec3& div) const;

		bool operator==(const Vec3& v1);

		bool operator!=(const Vec3& v1);

		bool operator>(const Vec3& v1);

		bool operator<(const Vec3& v1);
		Vec3 Normalize() const;
		
		
		double x, y, z;
};

inline double dot(const Vec3& vec1, const Vec3& vec2)
{
	return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}
#endif
