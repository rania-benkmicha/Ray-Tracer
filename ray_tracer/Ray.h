#ifndef Ray_H_
#define Ray_H_
#include"vecteur.h"

class Ray
{
	public :
		
		Ray(const Vec3& org, const Vec3& dest) : origin(org), direction(dest) {}
		Vec3 getorigin()const
		{
		return origin;
		}
		Vec3 getdirection()const
		{
		return direction;
		}
		
	private :
	Vec3 origin, direction;
};

#endif
