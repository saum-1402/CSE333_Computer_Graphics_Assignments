#version 330 core

in vec3 vVertex;
in vec3 vNormal;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
// uniform vec3 vColor;
out vec3 fColor;
// vec3 pColor;

uniform vec3 lpos_world;     //coordinates of light position from main.cpp
uniform vec3 eye_normal;      //coordinates of eye position from main.cpp


out vec4 otColor;

out vec3 n;      //below calulated n which will be further needed by fshaders
out vec3 e;      //below calulated n which will be further needed by fshaders
out vec3 l;      //below calulated n which will be further needed by fshaders

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.3, 0.4);


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
	// n = normalize(vNormal);       //we use this if we dont want the source of light to be at the given place it rotates with the object
    n = normalize(mat3(transpose(inverse(vModel))) * vNormal);      //part which ensures that the source of light is constant if we rotate the sphere
    l = normalize(lpos_world - vVertex);
    e = eye_normal;
	
	// vec3 pColor = Ia + Id + Is; //Interpolate color
    // otColor = vec4(pColor, 1.0);
}



