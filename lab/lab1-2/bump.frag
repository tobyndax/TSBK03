#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions
	
    vec3 normal = normalize(out_Normal);
	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.

	vec2 samp = outTexCoord;
	samp.s = samp.s+offset;
	float bs = texture(texUnit, samp).r-texture(texUnit, outTexCoord).r;

	samp = outTexCoord;
	samp.t = samp.t+offset;
	float bt = texture(texUnit, samp).r-texture(texUnit, outTexCoord).r;

	vec3 nprim = normalize(out_Normal + bs*3*cross(Pt,out_Normal)+bt*3*cross(out_Normal,Ps));
    out_Color = vec4( dot(nprim, light)) * vec4(0.3, 0.3,0.3,1);// texture(texUnit, outTexCoord);
}
