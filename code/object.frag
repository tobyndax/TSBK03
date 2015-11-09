#version 130

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
{
/*
	int i =0;
	vec3 source;
	vec3 I;
	vec3 out_Color_temp=vec3(0,0,0);

	for(i=0;i<4;i=i+1){
		if(isDirectional[i]){
			source = normalize(-lightSourcesDirPosArr[i]);
		}
		else{
			source = normalize(lightSourcesDirPosArr[i]-vec3(world_Position));
		}

		vec3 view = normalize(camPosition-vec3(world_Position));
		vec3 radial = normalize(2*light_Normal*dot(source,light_Normal)-source);

		vec3 i_diff=vec3(0.4,0.3,0.7)*0.5*dot(source,light_Normal);//clamp(0.5*lightSourcesColorArr[i]*dot(source,light_Normal),0,1);

		float cosn = clamp(dot(radial,view),0,1);
		vec3 i_spec=clamp(0.5*lightSourcesColorArr[i]*pow(cosn,specularExponent[i]),0,1);

		I = i_spec+i_diff;
		out_Color_temp = out_Color_temp+I;
	}

	if( world_Position[1]<=0){
		outColor = texture(water, texCoord) * vec4(out_Color_temp,1);
	}else{
		outColor = texture(grass, texCoord) * vec4(out_Color_temp,1);
	}
*/
	const vec3 light = vec3(2,2,0);
	//const vec3 light1 = vec3(0,0.5,-10);
	//const vec3 light2 = vec3(10,2,0);
	float shading = clamp(dot(light,light_Normal),0,1);//+clamp(dot(light1,light_Normal),0,1);//+clamp(dot(light2,light_Normal),0,1);
	outColor = shading*texture(tex, texCoord);
}
