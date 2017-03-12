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

uniform vec3 cameraPos;

// varying
in vec2 o_texcoord;

// Output Color
out vec4 out_color;

void main() {
	vec3 light = vec3(0.5, -0.4, -0.5);
	
	vec3 albedo = texture(albedo, o_texcoord).xyz;
	vec3 position = texture(position, o_texcoord).xyz;
	vec2 gm = texture(glossy_metal, o_texcoord).xy;
	
	// Do not normalize it will mess up the normals. Only normalize when 
	// You need to.
	vec3 normal = texture(normal, o_texcoord).xyz;
	
	vec3 lightDir = normalize(-light);
	
	vec3 eyeDir = cameraPos - position;
	vec3 halfVec = lightDir + eyeDir;
	
	float ndotl = max(dot(normal, lightDir), 0.0);
	float ndoth = max(dot(normal, halfVec), 0.0);
	
	vec3 diffuse = albedo * ndotl;
	vec3 specular = vec3(1.0) * pow(ndoth, 1);
	
	out_color = vec4(diffuse + specular, 1.0);
	//out_color = vec4(position, 1.0);
	// Note when do deffered rendering don't normailze normals. They have been 
	// normlized alread from the gbuffers
	//out_color = vec4(normalize(normal), 1.0);
}