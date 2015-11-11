#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 light_Normal;
out vec4 world_Position;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;



void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	light_Normal = normalize(normalMatrix1 * inNormal);

	texCoord = inTexCoord;
	gl_Position = projMatrix* viewMatrix * mdlMatrix * vec4(inPosition, 1.0);
	world_Position = projMatrix *mdlMatrix*vec4(inPosition, 1.0);
}
