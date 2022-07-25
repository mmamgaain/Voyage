#pragma once
#include <voyage.hpp>

#define SAFE_DELETE(ptr) (if(ptr) { delete ptr; ptr = NULL; })

#define GLCheckError() (glGetError() == GL_NO_ERROR)

#define exitIfGLError()																									\
{																														\
	GLenum err = glGetError();																							\
	if(err != GL_NO_ERROR) {																							\
		std::cout << "OpenGL error occured in " << __FILE__ << ":" << __LINE__ << " , " << err << std::endl;			\
		exit(1);																										\
	}																													\
}																														\

