//sphere.cpp

#include "sphere.h"
#include <math.h>
#include <vector>
using namespace std;


bool Sphere::intersect(Ray& r) const
{
	Vector3D centerVector = r.getOrigin() - position;
	double a = 1.0;
	double b = 2*dotProduct(r.getDirection(), centerVector);
	double c = dotProduct(centerVector, centerVector) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

	//now check if discr. is posivtive or zero, then only we have an intersection!
	if(discriminant >=0.0)
	{

		if(discriminant == 0)
		{
			double t;
			t = -b/(2.0*a);
			bool u = r.setParameter(t, this);
			if(u==true){
				Vector3D p = r.getPosition()-position;
				p.normalize();
				r.normal = p;
			}
			return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);

			bool b1 = r.setParameter(t1, this);
			bool b2 = r.setParameter(t2, this);

			if(b1==true || b2==true){
				Vector3D p = r.getPosition()-position;
				p.normalize();
				r.normal = p;
			}

			return b1||b2;    
		}
	}
	return false;

}
