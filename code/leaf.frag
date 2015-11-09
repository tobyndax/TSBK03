#version 150

out	vec4 outColor;

in	vec2 texCoord;
in	vec3 light_Normal;

uniform sampler2D tex;
uniform	vec3 camPosition;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];


void main(void)
{	const vec3 light = vec3(2,2,0);
	float shading = clamp(dot(light,light_Normal),0,1);
    vec4 texel =texture(tex, texCoord);
    
    if (texel.a < 0.1 ){
        discard;
    }
    outColor = 0.8*texel;
}
