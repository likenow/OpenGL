#version 330 core

layout (location = 0) out vec4 o_Color;

uniform vec4 u_Color;

void main()
{
	o_Color = vec4(0.2, 0.3, 0.8, 1.0);
}