#include "world.h"
#include "material.h"
#include "color.h"
#include "lightsource.h"
#include "pointlightsource.h"
#include <math.h>
#include <iostream>
// #include <algorithm>
using namespace std;

// int count=0;
// int kR,kG,kB;


// const Vector3D refraction(const Vector3D& incident, const Vector3D& normal, const double eta, float t)
// {
// 	Vector3D a = 1*(incident-normal*(dotProduct(incident,normal)))/eta;
// 	Vector3D b = normal*sqrt(eta*eta-(1-dotProduct(incident,normal))*(1-dotProduct(incident,normal)))/eta;
// 	return a-b;
// }

Color Material::shade(const Ray& incident, const bool isSolid) const
{
	// Ray refract;
	LightSource *light = world->lightsource();
	Vector3D v=incident.getDirection();
	v.normalize();
	v.operator*=(-1.0);
	

	Vector3D l=light->getPosition()-incident.getPosition();
	l.normalize();

	Vector3D h=operator+(v,l);
	h.normalize();

	Color intensity = light->getIntensity();
	Vector3D normal = incident.getNormal();
	normal.normalize();
	double a = kd*(max(0.0, dotProduct(normal, l)));
	double b = ks*pow(max(0.0, dotProduct(normal, h)), n);
	double c = ka;
	Color f = operator*(intensity,color);
	Color t = operator*(f,a+b+c);
	// Ray shadow(incident.getPosition() + 0.1*l,l);
	// world -> firstIntersection(shadow);
	// if(shadow.didHit()==true){
	// 	Color sh = operator*(f,c);
	// 	return sh;
	// }

	// Vector3D ti = refraction(incident.getDirection(), normal, eta, incident.getParameter());
	// Ray refract(incident.getPosition(), ti);

	
	// if(eta>0){
	// 	if (incident.didHit() == false)
	// 	{
	// 		return t;
	// 	}
	// 	Vector3D r = incident.getDirection() - 2 * dotProduct(incident.getDirection(), normal) * normal; //ray after reflection
	// 	r.normalize(); //reflection ray

	// 	if(dotProduct(incident.getOrigin(),normal)<0){
	// 		// Vector3D t = refraction(incident.getDirection(),normal,eta,incident.getParameter());
	// 		// Ray refract(incident.getPosition(),t);
	// 		// refract = refr;
	// 		c=-1*dotProduct(incident.getDirection(),normal);
	// 		kR=kG=kB=1;
	// 	}
	// 	// else{
	// 		// kR=exp(-1*incident.getParameter());
	// 		// kG=exp(-1*incident.getParameter());
	// 		// kB=exp(-1*incident.getParameter());
	// 		// Vector3D t = refraction(incident.getDirection(),-normal,1/eta,incident.getParameter());
	// 		// if(t==Vector3D(0,0,0)){
	// 		// 	c=dot(t,normal);
	// 		// }
	// 	// }
	// 	float R0 = (eta - 1) * (eta - 1) / ((eta + 1) * (eta + 1));
	// 	float R = R0 + (1 - R0) * pow(1 - c, 5);
	// 	Color k(kR,kG,kB);
		
	// 	Color p = operator*((shade(incident,isSolid) + operator*(shade(refract,isSolid),1-R)),k);

	// 	return p;
	// }
	// return t;
	return color;
}	
