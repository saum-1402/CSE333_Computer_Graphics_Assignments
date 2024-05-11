#include "world.h"
#include "material.h"
#include "color.h"
#include "lightsource.h"
#include "pointlightsource.h"
#include <vector>
#include <iostream>

using namespace std;
double kR=0;double kG=0;double kB=0;
double eta= 1.35;// int count=0;
double c ;
Vector3D t;

const Vector3D refraction(const Vector3D &incident, const Vector3D &normal, const double eta)
{
	Vector3D a = (incident - (normal * (dotProduct(incident, normal)))) / eta;
	Vector3D b = normal * sqrt(1 - ( (1 - dotProduct(incident, normal) * dotProduct(incident, normal)) / (eta*eta)));

	Vector3D ans = a-b;
	ans.normalize();
	return ans;
}

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
		objectList[i]->intersect(ray);
	return ray.getParameter();
}

Color World::shade_ray(Ray& ray,int count)
{
	
	firstIntersection(ray);
	//getting and normalizing normals
	Vector3D normal = ray.getNormal();
	normal.normalize();

	
	//reflection ray
	Vector3D r = ray.getDirection() - (2 * dotProduct(ray.getDirection(), normal) * normal);
	r.normalize();
	Ray reflection(ray.getPosition(), r);  // reflection ray

	if(ray.didHit()){
		if (ray.intersected()->isSolid == true)   //if object is solid
		{//this handles refelection.
			return (ray.intersected())->shade(ray) + operator*(shade_ray(reflection, count + 1), 0.5);
		}
		if(count<=20){
			
		if(ray.intersected()->isSolid == false)  //if object is dielectric
		{

			if (dotProduct(ray.getDirection(), normal) < 0)
			{
				c = -1.0 * dotProduct(ray.getDirection(), normal);
				t=refraction(ray.getDirection(),normal,eta);	
			}
			else{
				double b = 1 - ((1 - dotProduct(ray.getDirection(), -1 * normal) * dotProduct(ray.getDirection(), -1 * normal)) /( (1.0 / eta) * (1.0 / eta))); // check for TIR
				if(b>=0){   //if TIR does not occur
					t = refraction(ray.getDirection(), -1*normal, 1.0 / eta);
					c = dotProduct(t,normal);
				}
				else{   //if TIR occurs
					// Color k = Color(0.0, 0.0, 0.0);
					Color k = Color(1.0, 1.0, 1.0);
					return k*(shade_ray(reflection,count+1));
				}
			}
			// Schlick's approximation
			Ray refract(ray.getPosition(), t); // refraction ray
			double R0 = (eta - 1.0) * (eta - 1.0) / ((eta + 1.0) * (eta + 1.0));
			double R = R0 + (1 - R0) * pow((1 - c), 5);
			//
			Color k = Color(1.0, 1.0, 1.0);
			//for beers law
			// k.operator=(Color(exp(-0.019*ray.getParameter()),exp(-0.010*ray.getParameter()),exp(-0.019*ray.getParameter())));  //beer_law
			// k=beer_law;
			// Color beer = operator*()

			Color p = k * (1 - R) * (shade_ray(refract,count+1)) + k * R * (shade_ray(reflection,count+1));
			return p;

			}
		}//count
	return (ray.intersected())->shade(ray);
	}
	return background;
}
