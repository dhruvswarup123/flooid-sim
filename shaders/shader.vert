#version 330 core

layout(location = 0) in vec2 i_position;
layout(location = 2) in vec2 i_centroid;
layout(location = 4) in vec3 i_color;

out vec2 _centroid;
out vec3 _color;

void main() {
	gl_Position = vec4(i_position, 0, 1);

	_centroid = i_centroid;
	_color = i_color;
	;
}