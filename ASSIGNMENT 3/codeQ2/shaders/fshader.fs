#version 330 core

in vec3 n;
in vec3 e;
in vec3 l;
in vec3 v;
in vec3 lpos;

uniform vec3 lpos_world;
uniform vec3 eye_normal;

uniform float cutoff_angle_in=0.939; //cos(20 degree in radians)

uniform float cutoff_angle_out=0.999; //cos(25 degree in radians)

// in vec3 fColor;
out vec4 outColor;

// out vec4 outColor;

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.3, 0.4);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

float spec_exp = 32;


//ambient
vec3 Ia = La * ka;       // ambient 


//diffuse
vec3 Id = kd*max(dot(n, l)*Ld, 0.0);		// diffuse


//specular
vec3 Is = ks*Ls*max(pow(dot(n,normalize(l+e)),spec_exp),0.0);        // specular

vec3 fcolor;



void main(void) {

    float theta = dot(n,normalize(e));         //cosine of angle between normal and eye vector to get the angle to get the angle between them
	float ratio=1-(1-cutoff_angle_out)/(1-cutoff_angle_in);       //ratio of the intensity of the light outside the cone to the intensity of the light inside the cone
	if(theta>cutoff_angle_in){            //if the angle is greater than the cutoff angle
		fcolor = Is+Id+Ia;				//add all the components
	}

	else{
		fcolor = (Ia+Is)*ratio;          //else multiply the intensity of the light by the ratio to get a smooth spread of light around the cutoff angle
		// fcolor = Ia;						//else just show the ambient light and hard edges around the cutoff angle
	}

    outColor = vec4(fcolor, 1.0);

}