#include "Voyage/shader_program.hpp"
#include "Voyage/maths.hpp"
#include <fstream>

namespace Voyage {

	uint32_t ShaderProgram::DEFAULT_NUM_TEXTURES = 4;
	uint32_t ShaderProgram::MAX_TEXTURE_UNITS = 0;

	ShaderProgram::ShaderProgram(const char* const vertex_file, const char* const fragment_file, uint32_t num_textures) noexcept: id(0), num_textures(num_textures) {
		if(!MAX_TEXTURE_UNITS) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int*)&MAX_TEXTURE_UNITS);
		this->num_textures = clamp<uint32_t>(num_textures, 0, MAX_TEXTURE_UNITS);
		location_textureUnits.resize(this->num_textures);
		uint32_t vs = loadShader(vertex_file, GL_VERTEX_SHADER), fs = loadShader(fragment_file, GL_FRAGMENT_SHADER);
		id = glCreateProgram();
		glAttachShader(id, vs);
		glAttachShader(id, fs);
		glLinkProgram(id);
		int linked = GL_FALSE;
		glGetProgramiv(id, GL_LINK_STATUS, &linked);
		if(linked == GL_FALSE) {
			int length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) malloc(length * sizeof(char));
			glGetProgramInfoLog(id, length, &length, message);
			std::cerr << "Could not link shader: " << '\n' << message << (message[length - 1] == '\n' ? "" : "\n");
			delete message;
		}

		glValidateProgram(id);
		int validated = GL_FALSE;
		glGetProgramiv(id, GL_VALIDATE_STATUS, &validated);
		if(validated == GL_FALSE) {
			int length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) malloc(length * sizeof(char));
			glGetProgramInfoLog(id, length, &length, message);
			std::cerr << "Could not validate shader: " << '\n' << message << (message[length - 1] == '\n' ? "" : "\n");
			delete message;
		}

		getAllTextureLocations();
		glDetachShader(id, vs);
		glDetachShader(id, fs);
		glDeleteShader(vs);
		glDeleteShader(fs);
		if((linked & validated) == GL_FALSE) {
			glDeleteProgram(id);
			std::cerr << "Could not compile shader program. Deleting shader from the GPU." << std::endl;
		}
	}

	ShaderProgram::ShaderProgram(const ShaderProgram& shader) {
		if(id) dispose();
		id = shader.id;
		num_textures = shader.num_textures;
		uniformLocations.swap(shader.uniformLocations);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shader) {
		if(id) dispose();
		id = shader.id;
		num_textures = shader.num_textures;
		uniformLocations.swap(shader.uniformLocations);
		shader.id = 0;
	}

	ShaderProgram::~ShaderProgram() { dispose(); }

	ShaderProgram& ShaderProgram::operator=(const ShaderProgram& shader) {
		if(this == &shader) return *this;
		if(id) dispose();
		id = shader.id;
		num_textures = shader.num_textures;
		uniformLocations.swap(shader.uniformLocations);
		return *this;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader) {
		if(this == &shader) return *this;
		if(id) dispose();
		id = shader.id;
		num_textures = shader.num_textures;
		uniformLocations.swap(shader.uniformLocations);
		shader.id = 0;
		return *this;
	}

	uint32_t ShaderProgram::loadShader(const char* source, uint32_t type) {
		uint32_t id = glCreateShader(type);
		std::string result;
		std::ifstream in(source, std::ios::binary);
		if(!in) { std::cerr << "Shader file: " << source << " either doesn't exist or has some problem being opened!" << std::endl; exit(1); }
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
		const GLchar* r = result.data();
		const int l = result.length();
		glShaderSource(id, 1, &r, &l);
		glCompileShader(id);
		int compiled;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		if(compiled == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) malloc(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cerr << "Could not compile shader: " << source << '\n' << message << (message[length - 1] == '\n' ? "" : "\n");
			delete message;
		}
		return id;
	}

	void ShaderProgram::start() const { glUseProgram(id); }

	void ShaderProgram::stop() const { glUseProgram(0); }

	const int ShaderProgram::getUniformLocation(const char* const name, const bool& store) const {
		if(uniformLocations.find(name) != uniformLocations.end()) return uniformLocations[name];
		int loc = glGetUniformLocation(id, name);
		if(store) uniformLocations[name] = loc;
		return loc;
	}

	void ShaderProgram::getAllTextureLocations() { for(uint32_t i = 0; i < num_textures; i++) { location_textureUnits[i] = getUniformLocation(("texture" + std::to_string(i)).data(), false); } }

	void ShaderProgram::loadUniform(const char* const name, const float& value) { glUniform1f(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char* const name, const int& value) { glUniform1i(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char* const name, const uint32_t& value) { glUniform1ui(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char* const name, const bool& value) { glUniform1i(getUniformLocation(name), value); }

	void ShaderProgram::loadUniform(const char*  const name, const glm::vec2& value) { glUniform2f(getUniformLocation(name), value[0], value[1]); }

	void ShaderProgram::loadUniform(const char* const name, const glm::vec3& value) { glUniform3f(getUniformLocation(name), value[0], value[1], value[2]); }

	void ShaderProgram::loadUniform(const char* const name, const glm::vec4& value) { glUniform4f(getUniformLocation(name), value[0], value[1], value[2], value[3]); }

	void ShaderProgram::loadUniform(const char* const name, const float& x, const float& y, const float& z) { glUniform3f(getUniformLocation(name), x, y, z); }

	void ShaderProgram::loadUniform(const char* const name, const float& x, const float& y) { glUniform2f(getUniformLocation(name), x, y); }

	void ShaderProgram::loadUniform(const char* const name, const float& x, const float& y, const float& z, const float& w) { glUniform4f(getUniformLocation(name), x, y, z, w); }

	void ShaderProgram::loadUniform(const char* const name, const glm::mat3& value, const bool& to_be_transposed) { glUniformMatrix3fv(getUniformLocation(name), 1, to_be_transposed, &value[0][0]); }

	void ShaderProgram::loadUniform(const char* const name, const glm::mat4& value, const bool& to_be_transposed) { glUniformMatrix4fv(getUniformLocation(name), 1, to_be_transposed, &value[0][0]); }

	void ShaderProgram::dispose() { glDeleteProgram(id); uniformLocations.clear(); }

	bool ShaderProgram::remapTextureSampleName(const uint32_t& location, const char* const name) const {
		if(location < num_textures) {
			location_textureUnits.at(location) = getUniformLocation(name, false);
			glUniform1i(location_textureUnits[location], location);
			return true;
		}
		return false;
	}

}
