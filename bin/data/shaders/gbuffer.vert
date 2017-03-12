/**
	test.vert
	
	This is a test vertex shader that i'm using to test to see if the program is working
*/

#version 330 core
// Vertex Data
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;

// Uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

// Outputs
out vec4 o_position;
out vec2 o_texcoords;
out vec3 o_normals;

void main() {
	gl_Position = projection * view * model * vec4(vertices, 1.0);
	
	o_position = model * vec4(vertices, 0.0);
	
	o_texcoords = texCoords;
	
	o_normals = normalize(vec3(normalMatrix * vec4(normals, 0.0)));
}