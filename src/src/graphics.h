#pragma once

// Static Buffer
template<typename T>
class StaticVertexBuffer {
private:
	std::vector<T> list;
	GLuint id;
public:
	StaticVertexBuffer() {
		id = 0;
	}
	void add(T t) {
		list.push_back(t);
	}
	void clear() {
		list.clear();
	}
	void init() {
		glGenBuffers(1, &id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size(), &list[0], GL_STATIC_DRAW);
		unbind();
	}
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void release() {
		list.clear();
		glDeleteBuffers(1, &id);
	}
	GLuint type_size() {
		return sizeof(T);
	}
	GLuint count() {
		list.size();
	}
	GLuint size() {
		return type_size() * count();
	}
};

class VertexArray {
public:
	enum VertexArrayType {
		VAT_VERTICES = 0,
		VAT_TEXCOORDS,
		VAT_NORMALS
	};
private:
	GLuint id;
public:
	VertexArray();
	void init();
	void bind();
	void unbind();
	void release();
	void enable(VertexArrayType type);
	void disable(VertexArrayType type);
	void pointer(VertexArrayType type, GLuint size, GLenum gltype, bool normalize = false);
};

// Program
class Program {
public:
	// Shader
	class Shader {
	public:
		enum ShaderType{
			ST_VERTEX = 0,
			ST_FRAGMENT,
			ST_SIZE
		};
	private:
		GLuint id;
		void printLog();
	public:
		Shader();
		void init(std::string fn, ShaderType type);
		GLuint getID();
		void release();
	};
	class Uniform {
	private:
		Program* program;
		std::map<std::string, GLint> values;
	public:
		Uniform();
		void init(Program* prog);
		void create(std::string name);
		// Integer Uniforms
		void uniform1i(std::string name, GLint i);
		void uniform2i(std::string name, const glm::ivec2& v);
		void uniform3i(std::string name, const glm::ivec3& v);
		void uniform4i(std::string name, const glm::ivec4& v);
		// Float Uniforms
		void uniform1f(std::string name, GLfloat f);
		void uniform2f(std::string name, const glm::vec2& v);
		void uniform3f(std::string name, const glm::vec3& v);
		void uniform4f(std::string name, const glm::vec4& v);
		// Matricies
		void uniformMat2(std::string name, const glm::mat2& m, bool transpose = false);
		void uniformMat3(std::string name, const glm::mat3& m, bool transpose = false);
		void uniformMat4(std::string name, const glm::mat4& m, bool transpose = false);
		void release();
	};
private:
	Shader vertexShader;
	Shader fragmentShader;
	Uniform uniforms;
	GLuint id;
	void printLog();
public:
	Program();
	void init(std::string vfn, std::string ffn);
	void bind();
	void unbind();
	void release();
	GLuint getID();
	Uniform* getUniforms();
};