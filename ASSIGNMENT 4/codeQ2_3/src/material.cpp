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

	Color intensity = light->getIntensity();  //intensity of the light
	Vector3D normal = incident.getNormal();    //normal at the point of intersection
	normal.normalize();
	double a = kd*(max(0.0, dotProduct(normal, l))); //diffuse
	double b = ks*pow(max(0.0, dotProduct(normal, h)), n);  //specular
	double c = ka;   //ambient
	Color f = operator*(intensity,color);  //color of the object
	Color t = operator*(f,a+b+c);  //final color


	//shadow
	// Ray shadow(incident.getPosition() + 0.8*l,l);
	// world -> firstIntersection(shadow);
	// if(shadow.didHit()==true){
	// 	Color sh = operator*(f,c);
	// 	return sh;
	// }
	//comment this section to show only phong shading
	//shadow
	
	return t;
}	
