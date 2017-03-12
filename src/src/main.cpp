/*
	Deffered Rendering Example

	This is an example of a simple deffered rendering using opengl 3.3
	and other libraries. I'm going to write from scratch to test my
	theory of how to get this to work... If I need to move it to a 
	different file I will...
*/

// Defines
#define SCREEN_WIDTH 1024	
#define SCREEN_HEIGHT 800
// Shader Vertex Arrays
#define VT_VERTICES 0
#define VT_TEXCOORD 1
#define VT_NORMALS 2

// Standard Libaries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
// 3rd Party Libraries
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>
// Structs and Enums
// Mesh Object
struct MeshOBJ {
	// ids for buffers
	GLuint vertex_id;
	GLuint texcoord_id;
	GLuint normal_id;
	// id for vertex array
	GLuint vertex_array_id;
	// Length of vertices
	GLuint size;
	// Struct
	struct Attribute {
		GLuint v1;
		GLuint v2;
		GLuint v3;
	};
	struct Face {
		Attribute vertex;
		Attribute texcoord;
		Attribute normal;
	};
};
// Program
struct Program {	
	GLuint progid;
	GLuint vertid;
	GLuint fragid;
	std::map<std::string, GLuint> uniforms;
};
// Material
struct Material {
	glm::vec3 albedo;
	float glossy;
	float metal;
};
// Framebuffer
struct Framebuffer {
	// Framebuffer ID
	GLuint frame_buffer_id;
	// Renderbuffer
	GLuint render_buffer_id; // For Depth
	// Textures
	GLuint albedo_id;
	GLuint position_id;
	GLuint normal_id;
	GLuint glossy_metal_id;
};

struct Screen {
	// Buffers
	GLuint vertex_buffer_id;
	GLuint texcoord_buffer_id;
	// Vertex Array
	GLuint vertex_array_id;
	// Screen Size
	int width;
	int height;
	// Vertex Count
	int count;
};


// Program Object
// Variables
// windows global variables
bool isRunning = true;
SDL_Window* g_window = 0;
SDL_GLContext g_context = 0;

// Functions 
// Demo Functions
void demo_init();
void demo_event(SDL_Event& e);
void demo_update(float delta);
void demo_render();
void demo_release();

// Util Functions
std::string util_loadFile(std::string fn);
void util_loadFile(std::string fn, std::vector<std::string>& out);
void util_strSplit(std::string str, char delim, std::vector<std::string>& out);
int util_toInt(std::string value);
float util_toFloat(std::string value);
double util_toDouble(std::string value);
bool util_toBool(std::string value);

// Mesh OBJ Loader
void obj_load(MeshOBJ* obj, std::string fn);
void obj_handle_face(std::string str, GLuint& vertice, GLuint& texCoord, GLuint& normal);
void obj_render(MeshOBJ* obj);
void obj_release(MeshOBJ* obj);

// Program
void prog_load(Program* prog, std::string vfn, std::string ffn);
void prog_release(Program* prog);
void prog_bind(Program* prog);
void prog_unbind(Program* prog);
// Uniforms
void prog_uniform_create(Program* prog, std::string name);
// Integer
void prog_uniform_set1i(Program* prog, std::string name, int i);
void prog_uniform_set2i(Program* prog, std::string name, const glm::ivec2& v);
void prog_uniform_set3i(Program* prog, std::string name, const glm::ivec3& v);
void prog_uniform_set4i(Program* prog, std::string name, const glm::ivec4& v);
// Float
void prog_uniform_set1f(Program* prog, std::string name, float i);
void prog_uniform_set2f(Program* prog, std::string name, const glm::vec2& v);
void prog_uniform_set3f(Program* prog, std::string name, const glm::vec3& v);
void prog_uniform_set4f(Program* prog, std::string name, const glm::vec4& v);
// Matrix
void prog_uniform_setMat2(Program* prog, std::string name, const glm::mat2& m);
void prog_uniform_setMat3(Program* prog, std::string name, const glm::mat3& m);
void prog_uniform_setMat4(Program* prog, std::string name, const glm::mat4& m);

// Create Material
void mat_create_program_uniforms(Program* prog);
void mat_create(Material* material, glm::vec3 albedo, float glossy, float metal);
void mat_set_material(Program* prog, Material* mat);

// Create Framebuffer
void fb_create(Framebuffer* fb, int width, int height);
void fb_bind(Framebuffer* fb);
void fb_unbind(Framebuffer* fb);
//void fb_render(Framebuffer* fb);
void fb_release(Framebuffer* fb);

// Screen
void screen_create(Screen* sc, int width, int height);
void screen_release(Screen* sc);
void screen_render(Screen* sc, Program* prog, Framebuffer* fb);

// Methods for moving around the scene...
void scene_render_object(Program* prog, MeshOBJ* obj, glm::vec3 pos, glm::vec3 rot);

// This is the main file
int main(int argc, char** argv) {

	// Init Everything
	SDL_Init(SDL_INIT_EVERYTHING);
	// Create an Opengl 3.3 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create Window
	g_window = SDL_CreateWindow(
		"Deffered Renderer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	// Create OpenGL
	g_context = SDL_GL_CreateContext(g_window);

	glewInit();

	demo_init();

	// Update Loop
	SDL_Event e;

	int preTime = SDL_GetTicks();
	int curTime = 0;
	float delta = 0.0f;

	while (isRunning) {

		curTime = SDL_GetTicks();

		delta = ((float)(curTime - preTime)) / 1000.0f;

		preTime = curTime;


		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				isRunning = false;
			}

			demo_event(e);
		}


		demo_update(delta);

		demo_render();

		SDL_GL_SwapWindow(g_window);
	}


	demo_release();

	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
	SDL_Quit();

	return 0;
}

// Demo Variables
MeshOBJ monkey;

MeshOBJ the_floor;

Program prog;
Program dr;

float rot = 0.0f;

int dochange = 0;

// Materials
Material red;
Material green;
Material blue;
Material floor_mat;

// Create Framebuffer
Framebuffer fb;
// Screen
Screen screen;

// Demo Functions
void demo_init() {
	glEnable(GL_DEPTH_TEST);
	// Load Program
	
	// Prog 1
	prog_load(&prog, "data/shaders/gbuffer.vert", "data/shaders/gbuffer.frag");
	prog_bind(&prog);
	prog_uniform_create(&prog, "projection");
	prog_uniform_create(&prog, "view");
	prog_uniform_create(&prog, "model");
	prog_uniform_create(&prog, "normalMatrix");
	prog_uniform_create(&prog, "sw_view");
	mat_create_program_uniforms(&prog);
	prog_unbind(&prog);

	// Deffered Renderer
	prog_load(&dr, "data/shaders/deffered_rendering.vert", "data/shaders/deffered_rendering.frag");
	prog_bind(&dr);
	prog_uniform_create(&dr, "projection");
	prog_uniform_create(&dr, "view");
	prog_uniform_create(&dr, "model");
	prog_uniform_create(&dr, "albedo");
	prog_uniform_set1i(&dr, "albedo", 0);
	prog_uniform_create(&dr, "position");
	prog_uniform_set1i(&dr, "position", 1);
	prog_uniform_create(&dr, "normal");
	prog_uniform_set1i(&dr, "normal", 2);
	prog_uniform_create(&dr, "glossy_metal");
	prog_uniform_set1i(&dr, "glossy_metal", 3);
	prog_unbind(&dr);

	// Load Object
	obj_load(&the_floor, "data/meshs/floor.obj");
	obj_load(&monkey, "data/meshs/sphere.obj");
	// Create Materials
	mat_create(&red, glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f);
	mat_create(&green, glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, 0.0f);
	mat_create(&blue, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f);
	mat_create(&floor_mat, glm::vec3(1.0f, 0.5f, 0.0f), 0.5f, 1.0f);

	// Create framebuffer
	fb_create(&fb, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Screen
	screen_create(&screen, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void demo_event(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {

		if (e.key.keysym.scancode == SDL_SCANCODE_TAB) {


			++dochange;

			if (dochange >= 6) {
				dochange = 0;
			}
		}
	}
}

void demo_update(float delta) {

	rot += 32.0f * delta;

	if (rot >= 360.0f) {
		rot -= 360.0f;
	}
}

void demo_render() {

	/*
		GBuffers Rendering

		There are 4 gbuffers
	*/

	fb_bind(&fb);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog_bind(&prog);

	glm::mat4 proj = glm::perspective(
		glm::radians(60.0f), 
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 
		0.1f, 
		1024.0f
	);

	glm::mat4 view = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));

	prog_uniform_set1i(&prog, "sw_view", dochange);

	prog_uniform_setMat4(&prog, "projection", proj);
	prog_uniform_setMat4(&prog, "view", view);

	mat_set_material(&prog, &floor_mat);
	scene_render_object(&prog, &the_floor, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	mat_set_material(&prog, &red);
	scene_render_object(&prog, &monkey, glm::vec3(-3.0f, 1.0f, -6.0f), glm::vec3(rot, 0.0f, 0.0f));

	mat_set_material(&prog, &green);
	scene_render_object(&prog, &monkey, glm::vec3(0.0f, 1.0f, -6.0f), glm::vec3(0.0f, rot, 0.0f));

	mat_set_material(&prog, &blue);
	scene_render_object(&prog, &monkey, glm::vec3(3.0f, 1.0f, -6.0f), glm::vec3(0.0f, 0.0f, rot));

	prog_unbind(&prog);

	fb_unbind(&fb);


	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	prog_bind(&dr);

	proj = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);
	view = glm::mat4(1.0f);

	prog_uniform_setMat4(&dr, "projection", proj);
	
	prog_uniform_setMat4(&dr, "view", view);

	screen_render(&screen, &dr, &fb);

	prog_unbind(&dr);

	glEnable(GL_DEPTH_TEST);
}

void demo_release() {
	screen_release(&screen);
	fb_release(&fb);
	obj_release(&the_floor);
	obj_release(&monkey);
	prog_release(&dr);
	prog_release(&prog);
}

// Util Functions
std::string util_loadFile(std::string fn) {
	std::ifstream in(fn.c_str());
	std::stringstream ss;
	std::string temp;
	while (std::getline(in, temp)) {
		ss << temp << std::endl;
	}
	in.close();
	return ss.str();
}

void util_loadFile(std::string fn, std::vector<std::string>& out) {
	std::ifstream in(fn.c_str());
	std::string temp;
	while (std::getline(in, temp)) {
		out.push_back(temp);
	}
	in.close();
}

void util_strSplit(std::string str, char delim, std::vector<std::string>& out) {
	std::stringstream ss(str);
	std::string temp;
	while (std::getline(ss, temp, delim)) {
		out.push_back(temp);
	}
}

int util_toInt(std::string value) {
	return atoi(value.c_str());
}

float util_toFloat(std::string value) {
	return atof(value.c_str());
}

double util_toDouble(std::string value) {
	return atof(value.c_str());
}

bool util_toBool(std::string value) {
	return (value == "true") ? true : false;
}

template<typename T>
void _obj_create_buffer(GLuint* id, std::vector<T>& in) {
	glGenBuffers(1, id);
	glBindBuffer(GL_ARRAY_BUFFER, *id);
	glBufferData(GL_ARRAY_BUFFER, in.size() * sizeof(T), &in[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void _obj_delete_buffer(GLuint* id) {
	glDeleteBuffers(1, id);
}

// Mesh OBJ Loader
void obj_load(MeshOBJ* obj, std::string fn) {
	// Content
	std::vector<std::string> contents;
	std::vector<std::string> temp;
	// Holdes for object file stuff
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;
	std::vector<MeshOBJ::Face> f;

	// Load File
	util_loadFile(fn, contents);

	// Grab Data from object file
	for (GLint i = 0; i < contents.size(); i++) {
		util_strSplit(contents[i], ' ', temp);

		if (temp[0] == "v") {
			glm::vec3 t;
			t.x = util_toFloat(temp[1]);
			t.y = util_toFloat(temp[2]);
			t.z = util_toFloat(temp[3]);
			v.push_back(t);
		}

		else if (temp[0] == "vt") {
			glm::vec2 t;
			t.x = util_toFloat(temp[1]);
			t.y = 1.0f - util_toFloat(temp[2]);
			vt.push_back(t);
		}

		else if (temp[0] == "vn") {
			glm::vec3 t;
			t.x = util_toFloat(temp[1]);
			t.y = util_toFloat(temp[2]);
			t.z = util_toFloat(temp[3]);
			vn.push_back(glm::normalize(t));
		}

		else if (temp[0] == "f") {
			MeshOBJ::Face face;
			obj_handle_face(temp[1], face.vertex.v1, face.texcoord.v1, face.normal.v1);
			obj_handle_face(temp[2], face.vertex.v2, face.texcoord.v2, face.normal.v2);
			obj_handle_face(temp[3], face.vertex.v3, face.texcoord.v3, face.normal.v3);
			f.push_back(face);
		}

		temp.clear();
	}

	// Compute Vertice Data
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	for (GLuint i = 0; i < f.size(); i++) {
		// Vertices
		vertices.push_back(v[f[i].vertex.v1]);
		vertices.push_back(v[f[i].vertex.v2]);
		vertices.push_back(v[f[i].vertex.v3]);
		// TexCoords
		texCoords.push_back(vt[f[i].texcoord.v1]);
		texCoords.push_back(vt[f[i].texcoord.v2]);
		texCoords.push_back(vt[f[i].texcoord.v3]);
		// Normals
		normals.push_back(vn[f[i].normal.v1]);
		normals.push_back(vn[f[i].normal.v2]);
		normals.push_back(vn[f[i].normal.v3]);
	}

	// Set Object Size
	obj->size = vertices.size();
	// Create Vertex Buffer

	// Vertice
	_obj_create_buffer(&obj->vertex_id, vertices);
	// TexCoords
	_obj_create_buffer(&obj->texcoord_id, texCoords);
	// Normals
	_obj_create_buffer(&obj->normal_id, normals);
	
	// Create Vertex Array
	glGenVertexArrays(1, &obj->vertex_array_id);
	glBindVertexArray(obj->vertex_array_id);
	// Bind Vertex ID
	glBindBuffer(GL_ARRAY_BUFFER, obj->vertex_id);
	glVertexAttribPointer(VT_VERTICES, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(VT_VERTICES);
	// Bind TexCoord ID
	glBindBuffer(GL_ARRAY_BUFFER, obj->texcoord_id);
	glVertexAttribPointer(VT_TEXCOORD, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(VT_TEXCOORD);
	// Bind Normal ID
	glBindBuffer(GL_ARRAY_BUFFER, obj->normal_id);
	glVertexAttribPointer(VT_NORMALS, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(VT_NORMALS);
	// Unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind Vertex Array
	glBindVertexArray(0);
	glDisableVertexAttribArray(VT_VERTICES);
	glDisableVertexAttribArray(VT_TEXCOORD);
	glDisableVertexAttribArray(VT_NORMALS);
}

void obj_handle_face(std::string str, GLuint& vertice, GLuint& texCoord, GLuint& normal) {
	std::vector<std::string> temp;
	util_strSplit(str, '/', temp);
	vertice = util_toInt(temp[0]) - 1;
	texCoord = util_toInt(temp[1]) - 1;
	normal = util_toInt(temp[2]) - 1;
	temp.clear();
}

void obj_render(MeshOBJ* obj) {
	// Bind Vertex Array
	glBindVertexArray(obj->vertex_array_id);
	glDrawArrays(GL_TRIANGLES, 0, obj->size);
	// Unbind Vertex Array
	glBindVertexArray(0);
}

void obj_release(MeshOBJ* obj) {
	glDeleteVertexArrays(1, &obj->vertex_array_id);
	_obj_delete_buffer(&obj->normal_id);
	_obj_delete_buffer(&obj->texcoord_id);
	_obj_delete_buffer(&obj->vertex_id);
	
}

void prog_printShaderLog(int id) {
	int len;
	char infoLog[1024];
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		glGetShaderInfoLog(id, len, 0, infoLog);
		std::cout << infoLog << std::endl;
	}
}

void prog_printProgLog(int id) {
	int len;
	char infoLog[1024];
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		glGetShaderInfoLog(id, len, 0, infoLog);
		std::cout << infoLog << std::endl;
	}
}
// Program
void prog_load(Program* prog, std::string vfn, std::string ffn) {
	prog->vertid = glCreateShader(GL_VERTEX_SHADER);
	prog->fragid = glCreateShader(GL_FRAGMENT_SHADER);
	// Grab Shader Source
	std::string vsrc = util_loadFile(vfn);
	std::string fsrc = util_loadFile(ffn);
	const char* cvsrc = vsrc.c_str();
	const char* cfsrc = fsrc.c_str();
	glShaderSource(prog->vertid, 1, &cvsrc, 0);
	glShaderSource(prog->fragid, 1, &cfsrc, 0);
	// Compile Shaders
	glCompileShader(prog->vertid);
	prog_printShaderLog(prog->vertid);
	glCompileShader(prog->fragid);
	prog_printShaderLog(prog->fragid);
	// Create Program
	prog->progid = glCreateProgram();
	// Attaching Shaders
	glAttachShader(prog->progid, prog->vertid);
	glAttachShader(prog->progid, prog->fragid);
	// Linking Program
	glLinkProgram(prog->progid);
}

void prog_release(Program* prog) {
	glDetachShader(prog->progid, prog->vertid);
	glDetachShader(prog->progid, prog->fragid);
	glDeleteProgram(prog->progid);
	glDeleteShader(prog->vertid);
	glDeleteShader(prog->fragid);
}

void prog_bind(Program* prog) {
	glUseProgram(prog->progid);
}

void prog_unbind(Program* prog) {
	glUseProgram(0);
}
// Uniforms
void prog_uniform_create(Program* prog, std::string name) {
	prog->uniforms[name] = glGetUniformLocation(prog->progid, name.c_str());
}
// Integer
void prog_uniform_set1i(Program* prog, std::string name, int i) {
	glUniform1i(prog->uniforms[name], i);
}

void prog_uniform_set2i(Program* prog, std::string name, const glm::ivec2& v) {
	glUniform2i(prog->uniforms[name], v.x, v.y);
}

void prog_uniform_set3i(Program* prog, std::string name, const glm::ivec3& v) {
	glUniform3i(prog->uniforms[name], v.x, v.y, v.z);
}

void prog_uniform_set4i(Program* prog, std::string name, const glm::ivec4& v) {
	glUniform4i(prog->uniforms[name], v.x, v.y, v.z, v.w);
}

// Float
void prog_uniform_set1f(Program* prog, std::string name, float i) {
	glUniform1f(prog->uniforms[name], i);
}

void prog_uniform_set2f(Program* prog, std::string name, const glm::vec2& v) {
	glUniform2f(prog->uniforms[name], v.x, v.y);
}

void prog_uniform_set3f(Program* prog, std::string name, const glm::vec3& v) {
	glUniform3f(prog->uniforms[name], v.x, v.y, v.z);
}

void prog_uniform_set4f(Program* prog, std::string name, const glm::vec4& v) {
	glUniform4f(prog->uniforms[name], v.x, v.y, v.z, v.w);
}

// Matrix
void prog_uniform_setMat2(Program* prog, std::string name, const glm::mat2& m) {
	glUniformMatrix2fv(prog->uniforms[name], 1, 0, &m[0][0]);
}

void prog_uniform_setMat3(Program* prog, std::string name, const glm::mat3& m) {
	glUniformMatrix3fv(prog->uniforms[name], 1, 0, &m[0][0]);
}

void prog_uniform_setMat4(Program* prog, std::string name, const glm::mat4& m) {
	glUniformMatrix4fv(prog->uniforms[name], 1, 0, &m[0][0]);
}

// Create Material
void mat_create_program_uniforms(Program* prog) {
	prog_uniform_create(prog, "mat.albedo");
	prog_uniform_create(prog, "mat.glossy");
	prog_uniform_create(prog, "mat.metal");
}

void mat_create(Material* material, glm::vec3 albedo, float glossy, float metal) {
	material->albedo = albedo;
	material->glossy = glossy;
	material->metal = metal;
}

void mat_set_material(Program* prog, Material* mat) {
	prog_uniform_set3f(prog, "mat.albedo", mat->albedo);
	prog_uniform_set1f(prog, "mat.glossy", mat->glossy);
	prog_uniform_set1f(prog, "mat.metal", mat->metal);
}


void fb_create_texture(GLuint* id, int width, int height) {
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Create Framebuffer
void fb_create(Framebuffer* fb, int width, int height) {
	// Create Framebuffer
	glGenFramebuffers(1, &fb->frame_buffer_id);

	// Create Albedo Texture
	fb_create_texture(&fb->albedo_id, width, height);
	// Create Position Texture
	fb_create_texture(&fb->position_id, width, height);
	// Create Normal Texture
	fb_create_texture(&fb->normal_id, width, height);
	// Create Glossy and Metal Texture
	fb_create_texture(&fb->glossy_metal_id, width, height);

	// Create Renderbuffer for Depthbuffer
	glGenRenderbuffers(1, &fb->render_buffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, fb->render_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// Bind textures to framebuffer

	//fb_bind(fb);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->frame_buffer_id);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->render_buffer_id);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb->albedo_id, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fb->position_id, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, fb->normal_id, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, fb->glossy_metal_id, 0);


	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Do a check to see if the framebuffer is complete...
}

void fb_bind(Framebuffer* fb) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->frame_buffer_id);

	static GLuint attachments[4] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};

	glDrawBuffers(4, attachments);
}

void fb_unbind(Framebuffer* fb) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void fb_release(Framebuffer* fb) {
	// Delete Renderbuffer
	glDeleteRenderbuffers(1, &fb->render_buffer_id);
	// Delete Textures
	glDeleteTextures(1, &fb->albedo_id);
	glDeleteTextures(1, &fb->position_id);
	glDeleteTextures(1, &fb->normal_id);
	glDeleteTextures(1, &fb->glossy_metal_id);
	glDeleteFramebuffers(1, &fb->frame_buffer_id);
}
// Screen
void screen_create(Screen* sc, int width, int height) {
	sc->width = width;
	sc->height = height;
	std::vector<glm::vec3> vlist;
	std::vector<glm::vec2> tlist;
	// Vertex List Data
	vlist.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vlist.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	vlist.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	vlist.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	vlist.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	vlist.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	// Texcoord List
	tlist.push_back(glm::vec2(0.0f, 0.0f));
	tlist.push_back(glm::vec2(1.0f, 0.0f));
	tlist.push_back(glm::vec2(0.0f, 1.0f));
	tlist.push_back(glm::vec2(0.0f, 1.0f));
	tlist.push_back(glm::vec2(1.0f, 0.0f));
	tlist.push_back(glm::vec2(1.0f, 1.0f));

	for (int i = 0; i < tlist.size(); i++) {
		tlist[i].y = 1.0f - tlist[i].y;
	}
	sc->count = vlist.size();
	// Create Buffers
	glGenBuffers(1, &sc->vertex_buffer_id);
	glGenBuffers(1, &sc->texcoord_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, sc->vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sc->count * sizeof(glm::vec3), &vlist[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, sc->texcoord_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sc->count * sizeof(glm::vec2), &tlist[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Create Vertex Array
	glGenVertexArrays(1, &sc->vertex_array_id);
	glBindVertexArray(sc->vertex_array_id);
	// Bind Vertex ID
	glBindBuffer(GL_ARRAY_BUFFER, sc->vertex_buffer_id);
	glVertexAttribPointer(VT_VERTICES, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(VT_VERTICES);
	// Bind Texcoord ID
	glBindBuffer(GL_ARRAY_BUFFER, sc->texcoord_buffer_id);
	glVertexAttribPointer(VT_TEXCOORD, 2, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(VT_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	// Disable 
	glDisableVertexAttribArray(VT_VERTICES);
	glDisableVertexAttribArray(VT_TEXCOORD);
}

void screen_release(Screen* sc) {
	glDeleteBuffers(1, &sc->vertex_buffer_id);
	glDeleteBuffers(1, &sc->texcoord_buffer_id);
	glDeleteVertexArrays(1, &sc->vertex_array_id);
}

void screen_render(Screen* sc, Program* prog, Framebuffer* fb) {

	glm::mat4 model = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
					  glm::scale(glm::vec3(sc->width, sc->height, 0.0f));

	prog_uniform_setMat4(prog, "model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fb->albedo_id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fb->position_id);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fb->normal_id);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, fb->glossy_metal_id);

	glBindVertexArray(sc->vertex_array_id);
	glDrawArrays(GL_TRIANGLES, 0, sc->count);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Methods for moving around the scene...
void scene_render_object(Program* prog, MeshOBJ* obj, glm::vec3 pos, glm::vec3 rot) {

	glm::mat4 model, normalMatrix;

	model = glm::translate(pos) *
		glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));


	normalMatrix = glm::inverseTranspose(model);

	prog_uniform_setMat4(prog, "model", model);

	prog_uniform_setMat4(prog, "normalMatrix", normalMatrix);

	obj_render(obj);
}
