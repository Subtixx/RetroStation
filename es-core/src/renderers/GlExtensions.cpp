#include "GlExtensions.h"
#include <SDL.h>
#include <loguru.hpp>

#ifdef OPENGL_EXTENSIONS
namespace glext
{
	PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
	PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
	PFNGLGENBUFFERSPROC	glGenBuffers = nullptr;		
	PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
	PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
	PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
	PFNGLATTACHSHADERPROC glAttachShader = nullptr;
	PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
	PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
	PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
	PFNGLUNIFORM1IPROC glUniform1i = nullptr;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
	PFNGLBUFFERDATAPROC glBufferData = nullptr;
	PFNGLBUFFERSUBDATAARBPROC glBufferSubData = nullptr;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
	PFNGLCREATESHADERPROC glCreateShader = nullptr;
	PFNGLACTIVETEXTUREPROC glActiveTexture_ = nullptr;
	PFNGLUNIFORM1FPROC glUniform1f = nullptr;
	PFNGLUNIFORM2FPROC glUniform2f = nullptr;
	PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
	PFNGLDELETESHADERPROC glDeleteShader = nullptr;
	
	void* _glProcAddress(const char *proc)
	{
		void* ret = SDL_GL_GetProcAddress(proc);
		if (ret == nullptr)
			LOG_S(ERROR) << "FAILED TO INITIALISE GL Extension: " << proc;

		return ret;
	}

	bool initializeGlExtensions()
	{
		if (!SDL_GL_ExtensionSupported("GL_ARB_shader_objects"))
		{
			LOG_S(ERROR) << "GL Extensions not supported. GL_ARB_shader_objects";
			return false;
		}

		if (!SDL_GL_ExtensionSupported("GL_ARB_shading_language_100"))
		{
			LOG_S(ERROR) << "GL Extensions not supported. GL_ARB_shading_language_100";
			return false;
		}

		if (!SDL_GL_ExtensionSupported("GL_ARB_vertex_shader"))
		{
			LOG_S(ERROR) << "GL Extensions not supported. GL_ARB_vertex_shader";
			return false;
		}

		if (!SDL_GL_ExtensionSupported("GL_ARB_fragment_shader"))
		{
			LOG_S(ERROR) << "GL Extensions not supported. GL_ARB_fragment_shader";
			return false;
		}

		glCreateShader = (PFNGLCREATESHADERPROC)_glProcAddress("glCreateShader");
		glCompileShader = (PFNGLCOMPILESHADERPROC)_glProcAddress("glCompileShader");
		glCreateProgram = (PFNGLCREATEPROGRAMOBJECTARBPROC)_glProcAddress("glCreateProgram");
		glGenBuffers = (PFNGLGENBUFFERSPROC)_glProcAddress("glGenBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC)_glProcAddress("glBindBuffer");
		glShaderSource = (PFNGLSHADERSOURCEPROC)_glProcAddress("glShaderSource");
		glGetShaderiv = (PFNGLGETSHADERIVPROC)_glProcAddress("glGetShaderiv");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)_glProcAddress("glGetShaderInfoLog");
		glAttachShader = (PFNGLATTACHSHADERPROC)_glProcAddress("glAttachShader");
		glLinkProgram = (PFNGLLINKPROGRAMPROC)_glProcAddress("glLinkProgram");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)_glProcAddress("glGetProgramiv");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)_glProcAddress("glGetProgramInfoLog");
		glUseProgram = (PFNGLUSEPROGRAMPROC)_glProcAddress("glUseProgram");
		glUniform1i = (PFNGLUNIFORM1IPROC)_glProcAddress("glUniform1i");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)_glProcAddress("glGetUniformLocation");
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)_glProcAddress("glGetAttribLocation");
		glBufferData = (PFNGLBUFFERDATAPROC)_glProcAddress("glBufferData");
		glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)_glProcAddress("glBufferSubData");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)_glProcAddress("glVertexAttribPointer");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)_glProcAddress("glEnableVertexAttribArray");
		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)_glProcAddress("glDisableVertexAttribArray");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)_glProcAddress("glUniformMatrix4fv");
		glActiveTexture_ = (PFNGLACTIVETEXTUREPROC)_glProcAddress("glActiveTexture");
		glUniform1f = (PFNGLUNIFORM1FPROC)_glProcAddress("glUniform1f");
		glUniform2f = (PFNGLUNIFORM2FPROC)_glProcAddress("glUniform2f");		
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)_glProcAddress("glDeleteProgram");
		glDeleteShader = (PFNGLDELETESHADERPROC)_glProcAddress("glDeleteShader");

		typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);

		return 
			glCreateShader != nullptr && glCompileShader != nullptr && glCreateProgram != nullptr && glGenBuffers != nullptr && 
			glBindBuffer != nullptr && glGetShaderiv != nullptr && glGetShaderInfoLog != nullptr && glAttachShader != nullptr &&
			glLinkProgram != nullptr && glGetProgramiv != nullptr && glGetProgramInfoLog != nullptr && glUseProgram != nullptr &&
			glUniform1i != nullptr && glGetUniformLocation != nullptr && glGetAttribLocation != nullptr && glBufferData != nullptr &&
			glVertexAttribPointer != nullptr && glBufferData != nullptr && glBufferSubData != nullptr && glVertexAttribPointer != nullptr && glEnableVertexAttribArray != nullptr &&
			glDisableVertexAttribArray != nullptr && glUniformMatrix4fv != nullptr && glActiveTexture_ != nullptr && glUniform1f != nullptr && glUniform2f != nullptr && glDeleteShader != nullptr && glDeleteProgram != nullptr;
	}
}
#endif

#if defined(_DEBUG)
#include <loguru.hpp>

void _GLCheckError(const char* _funcName)
{
	const GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR)
		LOG_S(ERROR) << "GL error: " << _funcName << " failed with error code: " << errorCode;
}
#endif
