/**
	test.frag
	
	This is a test vertex shader that i'm using to test to see if the program is working
*/

#version 330 core

// Structures
struct Material {
	vec3 albedo;
	float glossy;
	float metal;
};

// Uniforms
uniform int sw_view;
uniform Material mat;
// Input
in vec4 o_position;
in vec2 o_texcoords;
in vec3 o_normals;
// Out put to framebuffer
layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;
layout(location = 3) out vec4 out_glossy_metal;

void main() {
	out_albedo = vec4(mat.albedo, 1.0);
	out_position = o_position;
	out_normal = vec4(normalize(o_normals), 0.0);
	out_glossy_metal = vec4(mat.glossy, mat.metal, 0.0, 0.0);
}