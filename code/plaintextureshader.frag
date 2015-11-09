#version 130

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec4 texel = texture(texUnit,outTexCoord);
    if(texel.a < 0)
    discard;

    out_Color = texture(texUnit, outTexCoord);
}
