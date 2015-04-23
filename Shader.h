#ifndef SHADER_H
#define SHADER_H

static char *
shaderLoadSource(const char *filePath);

static GLuint
shaderCompileFromFile(GLenum type, const char *filePath);

static void
shaderAttachFromFile(GLuint program, GLenum type, const char *filePath);

GLuint
initShader(char *fileName);

#endif