#version 330 core

in vec3 n;
in vec3 e;
in vec3 l;

uniform vec3 lpos_world;
uniform vec3 eye_normal;

uniform float cutoff_angle=0.866; //cos(15 degree in radians)

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
vec3 Ia = La * ka;     //calculation of ambient 

//diffuse
vec3 Id = kd* Ld*max(dot(l,-n), 0.0);       //calculation of diffuse

//specular
vec3 Is = ks*Ls*max(pow(dot(n,normalize(l+e)),spec_exp),0.0);          //calcualtion of specular

// vec3 fcolor=Ia+Id+Is;
vec3 fcolor;

void main(void) {

    // float theta = dot(n,-l);

	// if(theta>cutoff_angle){
	// 	fcolor = Ia;
	// }

	// else{
		fcolor = Ia + Id+Is;               //adding all 3 Ia , Id, Is for phong shading
	// }

    outColor = vec4(fcolor, 1.0);
}
