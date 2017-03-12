/**
	deffered_rendering.vert
	
	This is the shader where all of the magic of deffered rendering happends :)
*/
#version 330 core

// Attribues
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texcoords;
// Uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
// TexCoords
out vec2 o_texcoord;

void main() {
	
	gl_Position = projection * view * model * vec4(vertices, 1.0);
	
	o_texcoord = texcoords;
	
}