#include "sys.h"

// Vertex Array
VertexArray::VertexArray() {
	this->id = 0;
}

void VertexArray::init() {
	glGenVertexArrays(1, &this->id);
}

void VertexArray::bind() {
	glBindVertexArray(this->id);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

void VertexArray::release() {
	glDeleteVertexArrays(1, &this->id);
}

void VertexArray::enable(VertexArrayType type) {
	glEnableVertexAttribArray(type);
}

void VertexArray::disable(VertexArrayType type) {
	glDisableVertexAttribArray(type);
}

void VertexArray::pointer(VertexArrayType type, GLuint size, GLenum gltype, bool normalize) {
	glVertexAttribPointer(
		type,
		size,
		gltype,
		(normalize)? GL_TRUE : GL_FALSE,
		0,
		0
	);
}

// Shader
void Program::Shader::printLog() {
	int len;
	char infoLog[1024];
	glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		glGetShaderInfoLog(id, len, 0, infoLog);
		std::cout << infoLog << std::endl;
	}
}

Program::Shader::Shader() {
	this->id = 0;
}

void Program::Shader::init(std::string fn, ShaderType type) {
	if (type == ShaderType::ST_VERTEX) {
		this->id = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == ShaderType::ST_FRAGMENT) {
		this->id = glCreateShader(GL_FRAGMENT_SHADER);
	}
	std::string src = util_loadFile(fn);
	const char* csrc = src.c_str();
	glShaderSource(this->id, 1, &csrc, 0);
	glCompileShader(this->id);
	this->printLog();
}

GLuint Program::Shader::getID() {
	return this->id;
}

void Program::Shader::release() {
	glDeleteShader(this->id);
}

// Uniform
Program::Uniform::Uniform() {
	this->program = 0;
}

void Program::Uniform::init(Program* prog) {
	this->program = prog;
}

void Program::Uniform::create(std::string name) {
	this->values[name] = glGetUniformLocation(this->program->getID(), name.c_str());
}

// Integer Uniforms
void Program::Uniform::uniform1i(std::string name, GLint i) {
	glUniform1i(values[name], i);
}

void Program::Uniform::uniform2i(std::string name, const glm::ivec2& v) {
	glUniform2i(values[name], v.x, v.y);
}

void Program::Uniform::uniform3i(std::string name, const glm::ivec3& v) {
	glUniform3i(values[name], v.x, v.y, v.z);
}

void Program::Uniform::uniform4i(std::string name, const glm::ivec4& v) {
	glUniform4i(values[name], v.x, v.y, v.z, v.w);
}

// Float Uniforms
void Program::Uniform::uniform1f(std::string name, GLfloat f) {

}

void Program::Uniform::uniform2f(std::string name, const glm::vec2& v) {

}

void Program::Uniform::uniform3f(std::string name, const glm::vec3& v) {

}

void Program::Uniform::uniform4f(std::string name, const glm::vec4& v) {

}

// Matricies
void Program::Uniform::uniformMat2(std::string name, const glm::mat2& m, bool transpose = false) {

}

void Program::Uniform::uniformMat3(std::string name, const glm::mat3& m, bool transpose = false) {

}

void Program::Uniform::uniformMat4(std::string name, const glm::mat4& m, bool transpose = false) {

}

void Program::Uniform::release() {

}
