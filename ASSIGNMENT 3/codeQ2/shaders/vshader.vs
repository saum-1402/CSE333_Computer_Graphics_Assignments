#version 330 core

in vec3 vVertex;
in vec3 vNormal;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
// uniform vec3 vColor;
out vec3 fColor;
// vec3 pColor;

uniform vec3 lpos_world;
uniform vec3 eye_normal;


out vec4 otColor;

out vec3 n;
out vec3 e;
out vec3 l;
out vec3 v;
out vec3 lpos;

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.3, 0.4);
// ls=ls*50;
// ld=ld*50;
// la=la*50;

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

// float spec_exp = 32;

// // //ambient
// vec3 Ia = La * ka;

// // //diffuse
// // vec3 Id = kd* max(dot(n, l)*Ld, 0.0);
// //diffuse
// vec3 Id = kd* max(dot(normalize(-l), normalize(n))*Ld, 0.0);


// //specular
// vec3 Is = ks*Ls*max(pow(dot(n,normalize(l+e)),spec_exp),0.0);



void main() {
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
	// n = normalize(vNormal);         //use this when you want the source of light to move with the object
    n=normalize(mat3(transpose(inverse(vModel))) * vNormal);       //use this when you want the source of light to be fixed
    l = normalize(lpos_world - vVertex);
    e = eye_normal;
    v = vVertex;
    lpos= normalize(lpos_world);
	
	// vec3 pColor = Ia + Id + Is; //Interpolate color
    // otColor = vec4(pColor, 1.0);
}



