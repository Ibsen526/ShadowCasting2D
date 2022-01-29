#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 of_Color;

uniform vec2 a_mouseMiddle; //uniform

void main()
{
	float distToMouse = sqrt(pow(abs(a_mouseMiddle.x - gl_FragCoord.x), 2) + pow(abs(a_mouseMiddle.y - gl_FragCoord.y), 2));
	if(distToMouse < 300.0f) 
	{
		o_Color = vec4(of_Color.xyz * (1.0f - distToMouse / 300.0f), 1.0f);
	}
	else
	{
		o_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}	
	//gl_FragCoord() ... gibt die position des aktuellen fragments zurück
}