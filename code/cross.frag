#version 150

out	vec4 outColor;
in	vec2 texCoord;
uniform sampler2D tex;


void main(void){
    vec4 texel =texture(tex, texCoord);
    if (texel.a < 0.1 ){
        discard;
    }
	outColor = texture(tex, texCoord);
}
