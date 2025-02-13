#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 of_Color;

uniform vec2 u_lightPos;
uniform float u_lightRadius; 
uniform float u_lightIntensity;
uniform vec3 u_lightColor;

void main()
{
	float distToMouse = sqrt(pow(abs(u_lightPos.x - gl_FragCoord.x), 2) + pow(abs(u_lightPos.y - gl_FragCoord.y), 2));
	if(distToMouse < u_lightRadius) 
	{
		float normDist = distToMouse / u_lightRadius;
		//o_Color = vec4(of_Color.xyz * (1.0f - normDist * normDist), 1.0f);
		//o_Color = vec4(of_Color.xyz * (1.0f - normDist), 1.0f);
		//o_Color = vec4(of_Color.xyz * (1.0f - normDist), 1.0f - normDist);
		o_Color = vec4(u_lightColor * u_lightIntensity * (1.0f - normDist), 1.0f - normDist);
	}
	else
	{
		discard;
		//o_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}	
	//gl_FragCoord() ... gibt die position des aktuellen fragments zurück
}