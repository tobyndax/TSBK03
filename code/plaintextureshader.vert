#version 150

in vec3 in_Position;
in vec2 in_TexCoord;

out vec2 outTexCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;


void main(void)
{
	outTexCoord = in_TexCoord;
	gl_Position = projMatrix* viewMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
