#include "../../lib/glad.h"
#include "shader_program.hpp"
#include <iostream>
#include "../utils/maths.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <string>

using namespace Cyclone;

unsigned int ShaderProgram::DEFAULT_NUM_TEXTURES = 4;
int ShaderProgram::MAX_TEXTURE_UNITS = -1;

ShaderProgram::ShaderProgram(const char* vertex_file, const char* fragment_file, unsigned int num_textures) noexcept {
	if(MAX_TEXTURE_UNITS == -1) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_UNITS);
	this->num_textures = clamp<unsigned int>(num_textures, DEFAULT_NUM_TEXTURES, MAX_TEXTURE_UNITS);
	unsigned int vs = loadShader(vertex_file, GL_VERTEX_SHADER), fs = loadShader(fragment_file, GL_FRAGMENT_SHADER);
	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	glValidateProgram(id);
	glDetachShader(id, vs);
	glDetachShader(id, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

ShaderProgram::ShaderProgram(const ShaderProgram& shader): id(shader.id), num_textures(shader.num_textures) {}

ShaderProgram::ShaderProgram(ShaderProgram&& shader):id(std::move(shader.id)), num_textures(std::move(shader.num_textures)) {}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& shader) { id = shader.id; return *this; }

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader) { id = shader.id; return *this; }

unsigned int ShaderProgram::loadShader(const char* source, unsigned int type) {
	unsigned int id = glCreateShader(type);
	std::string result;
	std::ifstream in(source, std::ios::in);
	if(!in) { std::cerr << "Shader file " << source << " either doesn't exist or has some problem being opened!" << std::endl; exit(1); }
	in.seekg(0, std::ios::end);
	result.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&result[0], result.size());
	const GLchar* r = result.data();
	const int l = result.length();
	glShaderSource(id, 1, &r, &l);
	in.close();
	// std::cout << "Shader file content :\n" << result << std::endl;
	glCompileShader(id);
	int compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	// if(compiled == GL_FALSE) std::cerr << "Could not compile shader\n" << glGetShaderInfoLog(id, 500) << std::endl;
	return id;
}

void ShaderProgram::start() { glUseProgram(id); }

void ShaderProgram::stop() { glUseProgram(0); }

void ShaderProgram::dispose() { glDeleteProgram(id); }

