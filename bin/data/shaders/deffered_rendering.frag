/**
	deffered_rendering.vert
	
	This is the shader where all of the magic of deffered rendering happends :)
*/
#version 330 core

// Uniforms
uniform sampler2D albedo;
uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D glossy_metal;

// varying
in vec2 o_texcoord;

// Output Color
out vec4 out_color;

void main() {
	vec3 cameraPos = vec3(0.0, 1.0, 1.0);
	vec3 light = vec3(0, 1, 1);
	
	vec4 a = texture(albedo, o_texcoord);
	vec4 p = texture(position, o_texcoord);
	vec3 n = normalize(texture(normal, o_texcoord)).xyz;
	vec2 g_m = texture(glossy_metal, o_texcoord).xy;
	
	vec3 lightDir = normalize(light);
	
	vec3 eyeDir = normalize(cameraPos - p.xyz);
	
	vec3 halfVec = normalize(lightDir.xyz + eyeDir);
	
	vec3 diffuse = a.xyz * max(dot(n, lightDir), 0.0);
	
	vec3 specular = vec3(1.0) * pow(max(dot(n, halfVec), 0.0), 255);
	
	out_color = vec4(diffuse + specular, 1.0);
}