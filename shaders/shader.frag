#version 330 core

in vec2 _centroid;
in vec3 _color;

layout(location = 0) out vec4 color;

uniform float u_radius;

void main() {
	float l = distance((gl_FragCoord.xy/vec2(1280.0, 720.0) - 0.5) * 2, _centroid);
	if (l >= u_radius){
		discard;
	}

	color = vec4(_color, 1);
}