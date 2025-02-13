#version 330 core

layout(location = 0) in vec2 i_Pos; //input position
layout(location = 1) in vec4 i_Color;

out vec4 of_Color; //output color

void main()
{
	gl_Position = vec4(i_Pos, 0.0f, 1.0f);
	of_Color = i_Color;
}