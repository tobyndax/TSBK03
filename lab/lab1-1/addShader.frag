#version 130

in vec2 outTexCoord;
uniform sampler2D texUnit;
uniform sampler2D texUnit2;
uniform float texSize;
out vec4 out_Color;

void main(void)
{
    out_Color = 0.2*texture(texUnit, outTexCoord)+texture(texUnit2, outTexCoord);
}
