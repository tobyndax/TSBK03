#version 150

in  vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
out vec2 texCoord;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;


void main(void)
{
	vec3 light_Normal = inNormal;
	texCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
