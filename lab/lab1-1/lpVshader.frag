#version 130

in vec2 outTexCoord;
uniform sampler2D texUnit;
uniform float texSize;
out vec4 out_Color;

void main(void)
{
	float offset = 2.0/texSize;
	vec2 tc = outTexCoord;
	vec4 c = texture(texUnit, tc);
	tc.y = tc.y+offset;
	vec4 u = texture(texUnit, tc);
	tc.y = tc.y-2*offset;
	vec4 d = texture(texUnit, tc);
    out_Color = 0.25*(c+c+u+d);
}
