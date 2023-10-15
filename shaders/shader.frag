#version 330 core

in vec2 _centroid;
in vec3 _color;

layout(location = 0) out vec4 color;


void main() {
	float l = distance(gl_FragCoord.xy, _centroid)/100;
	if (l > 0.1){
		discard;
	}
	color = vec4(1, 0, 0, 1);
}