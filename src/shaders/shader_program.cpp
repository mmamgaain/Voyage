#include <glad.h>
#include "shader_program.hpp"
#include <iostream>
#include "../extra/maths.hpp"
#include <fstream>

namespace Cyclone {

	unsigned int ShaderProgram::DEFAULT_NUM_TEXTURES = 4;
	int ShaderProgram::MAX_TEXTURE_UNITS = -1;

	ShaderProgram::ShaderProgram(const char* vertex_file, const char* fragment_file, unsigned int num_textures) noexcept: id(0), num_textures(num_textures) {
		if(MAX_TEXTURE_UNITS == -1) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_UNITS);
		this->num_textures = clamp<unsigned int>(num_textures, DEFAULT_NUM_TEXTURES, MAX_TEXTURE_UNITS);
		location_textureUnits.resize(this->num_textures);
		unsigned int vs = loadShader(vertex_file, GL_VERTEX_SHADER), fs = loadShader(fragment_file, GL_FRAGMENT_SHADER);
		id = glCreateProgram();
		glAttachShader(id, vs);
		glAttachShader(id, fs);
		glLinkProgram(id);
		int linked = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &linked);
		if(linked == GL_FALSE) {
			int length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) alloca(length * sizeof(char));
			glGetProgramInfoLog(id, length, &length, message);
			std::cerr << "Could not link shader" << '\n' << message << (message[length -1] == '\n' ? "" : "\n");
			delete message;
		}

		glValidateProgram(id);
		int validated = 0;
		glGetProgramiv(id, GL_VALIDATE_STATUS, &validated);
		if(validated == GL_FALSE) {
			int length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) alloca(length * sizeof(char));
			glGetProgramInfoLog(id, length, &length, message);
			std::cerr << "Could not validate shader" << '\n' << message << (message[length -1] == '\n' ? "" : "\n");
			delete message;
		}

		getAllTextureLocations();
		glDetachShader(id, vs);
		glDetachShader(id, fs);
		glDeleteShader(vs);
		glDeleteShader(fs);
		if((linked | validated) == GL_FALSE) {
			glDeleteProgram(id);
			std::cerr << "Could not compile shader program. Deleting shader from the GPU." << std::endl;
		}
	}

	ShaderProgram::ShaderProgram(const ShaderProgram& shader): id(shader.id), num_textures(shader.num_textures) {}

	ShaderProgram::ShaderProgram(ShaderProgram&& shader):id(std::move(shader.id)), num_textures(std::move(shader.num_textures)) {}

	ShaderProgram& ShaderProgram::operator=(const ShaderProgram& shader) { id = shader.id; return *this; }

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader) { id = shader.id; return *this; }

	unsigned int ShaderProgram::loadShader(const char* source, unsigned int type) {
		unsigned int id = glCreateShader(type);
		std::string result;
		std::ifstream in(source, std::ios::binary);
		if(!in) { std::cerr << "Shader file " << source << " either doesn't exist or has some problem being opened!" << std::endl; exit(1); }
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		const GLchar* r = result.data();
		const int l = result.length();
		glShaderSource(id, 1, &r, &l);
		in.close();
		glCompileShader(id);
		int compiled;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		if(compiled == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cerr << "Could not compile shader" << source << '\n' << message << (message[length -1] == '\n' ? "" : "\n");
			delete message;
		}
		return id;
	}

	void ShaderProgram::start() const { glUseProgram(id); }

	void ShaderProgram::stop() const { glUseProgram(0); }

	const int ShaderProgram::getUniformLocation(const char* name, const bool& store) {
		if(uniformLocations.find(name) != uniformLocations.end()) return uniformLocations[name];
		int loc = glGetUniformLocation(id, name);
		if(store) uniformLocations[name] = loc;
		return loc;
	}

	void ShaderProgram::getAllTextureLocations() { for(unsigned int i = 0; i < num_textures; i++) { location_textureUnits[i] = getUniformLocation(("texture" + std::to_string(i)).data(), false); } }

	void ShaderProgram::loadUniform(const char* name, const float& value) { glUniform1f(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char* name, const int& value) { glUniform1i(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char* name, const bool& value) { glUniform1i(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char* name, const glm::vec3& value) { glUniform3f(getUniformLocation(name), value[0], value[1], value[2]); }

	void ShaderProgram::loadUniform(const char* name, const glm::vec4& value) { glUniform4f(getUniformLocation(name), value[0], value[1], value[2], value[3]); }

	void ShaderProgram::loadUniform(const char* name, const float& x, const float& y, const float& z) { glUniform3f(getUniformLocation(name), x, y, z); }

	void ShaderProgram::loadUniform(const char* name, const float& x, const float& y, const float& z, const float& w) { glUniform4f(getUniformLocation(name), x, y, z, w); }

	void ShaderProgram::loadUniform(const char* name, const glm::mat3& value) { glUniformMatrix3fv(getUniformLocation(name), 1, false, &value[0][0]); }

	void ShaderProgram::loadUniform(const char* name, const glm::mat4& value) { glUniformMatrix4fv(getUniformLocation(name), 1, false, &value[0][0]); }

	void ShaderProgram::dispose() { glDeleteProgram(id); }


}
