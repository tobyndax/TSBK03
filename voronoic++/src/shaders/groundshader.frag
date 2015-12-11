#version 150

out vec4 out_Color;

uniform float t;
uniform vec3 camPos;	// Kamernapositionen.
uniform vec3 lightSourcePos;	// Ljuspositionen.
uniform int isDirectional;
uniform float specularExponent;
uniform vec3 lightSourceColor;

vec3 r;
vec3 s;				// Infallande ljus.
vec3 eye;			// Vektor fr�n objektet till kameran.


// Phong-modellen:
float kamb;
float kdiff;
float kspec;
vec3 ambLight;		// Ambient.
vec3 diffLight;		// Diffuse.
vec3 specLight;		// Specular.
vec3 totalLight;	// Totalt ljus.

void main(void)
{
	out_Color = vec4(vec3(0.5f,0.75f,0.5f), 1.0f);
}
