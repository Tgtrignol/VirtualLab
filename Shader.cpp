#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <windows.h>
#include <string>
#include <iostream>

#include "Shader.h"

std::string DevDir() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	char buffer2[MAX_PATH];
	wcstombs(buffer2, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer2).find_last_of("\\/");
	return std::string(buffer2).substr(0, pos);
}

/*
* Returns a string containing the text in
* a vertex/fragment shader source file.
*/
static char *
shaderLoadSource(const char *filePath)
{
	const size_t blockSize = 512;
	FILE *fp;
	char buf[blockSize];
	char *source = NULL;
	size_t tmp, sourceLength = 0;

	/* open file */
	fopen_s(&fp, filePath, "r");
	if (!fp) {
		fprintf(stderr, "shaderLoadSource(): Unable to open %s for reading\n", filePath);
		return NULL;
	}

	/* read the entire file into a string */
	while ((tmp = fread(buf, 1, blockSize, fp)) > 0) {
		char *newSource = (char *)malloc(sourceLength + tmp + 1);
		if (!newSource) {
			fprintf(stderr, "shaderLoadSource(): malloc failed\n");
			if (source)
				free(source);
			return NULL;
		}

		if (source) {
			memcpy(newSource, source, sourceLength);
			free(source);
		}
		memcpy(newSource + sourceLength, buf, tmp);

		source = newSource;
		sourceLength += tmp;
	}

	/* close the file and null terminate the string */
	fclose(fp);
	if (source)
		source[sourceLength] = '\0';

	return source;
}

/*
* Returns a shader object containing a shader
* compiled from the given GLSL shader file.
*/
static GLuint
shaderCompileFromFile(GLenum type, const char *filePath)
{
	char *source;
	GLuint shader;
	GLint length, result;

	/* get shader source */
	source = shaderLoadSource(filePath);
	if (!source)
		return 0;

	/* create shader object, set the source, and compile */
	shader = glCreateShader(type);
	length = strlen(source);
	glShaderSource(shader, 1, (const char **)&source, &length);
	glCompileShader(shader);
	free(source);

	/* make sure the compilation was successful */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char *log;

		/* get the shader info log */
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetShaderInfoLog(shader, length, &result, log);

		/* print an error message and the info log */
		fprintf(stderr, "shaderCompileFromFile(): Unable to compile %s: %s\n", filePath, log);
		free(log);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

/*
* Compiles and attaches a shader of the
* given type to the given program object.
*/
void
shaderAttachFromFile(GLuint program, GLenum type, const char *filePath)
{
	/* compile the shader */
	GLuint shader = shaderCompileFromFile(type, filePath);
	if (shader != 0) {
		/* attach the shader to the program */
		glAttachShader(program, shader);

		/* delete the shader - it won't actually be
		* destroyed until the program that it's attached
		* to has been destroyed */
		glDeleteShader(shader);
	}
}

GLuint initShader(char *fileName)
{
	GLint result;

	/* create program object and attach shaders */

	GLuint uiShaderID = glCreateProgram();

	char filePath[MAX_PATH];
	sprintf_s(filePath, "%s%s%s", "C:/VrCave/Development/VirtualLab/Data/", fileName, ".vp");
	shaderAttachFromFile(uiShaderID, GL_VERTEX_SHADER, filePath);

	sprintf_s(filePath, "%s%s%s", "C:/VrCave/Development/VirtualLab/Data/", fileName, ".fp");
	shaderAttachFromFile(uiShaderID, GL_FRAGMENT_SHADER, filePath);

	/* link the program and make sure that there were no errors */
	glLinkProgram(uiShaderID);
	glGetProgramiv(uiShaderID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length;
		char *log;

		/* get the program info log */
		glGetProgramiv(uiShaderID, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetProgramInfoLog(uiShaderID, length, &result, log);

		/* print an error message and the info log */
		fprintf(stderr, "initShader(): Program linking failed: %s\n", log);
		free(log);

		/* delete the program */
		glDeleteProgram(uiShaderID);
		uiShaderID = 0;
	}

	return uiShaderID;
}