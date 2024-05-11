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
	if(ray.didHit()){
	return (ray.intersected())->shade(ray);
	}
	return background;
}
