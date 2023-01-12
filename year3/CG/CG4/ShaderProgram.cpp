#include "libs.h"
#include "ShaderProgram.h"
 


bool ShaderProgram::loadShaderProgram(const std::string & name)
{
	GLint result, size = 0;
	const size_t infosize = 2048;
	GLchar info[infosize];

	this->name = name;

	if (!loadShader(name + ".vs", vs, GL_VERTEX_SHADER))
		return false;
	if (!loadShader(name + ".fs", fs, GL_FRAGMENT_SHADER))
		return false;

	p = glCreateProgram();
	glAttachShader(p, vs);
	glAttachShader(p, fs);
	glLinkProgram(p);

	glGetProgramiv(p, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(p, infosize, &size, info);
		errormsg = "Shader linking error (" + name + "): " + std::string(info, size);
		return false;
	}

	return true;
}

const GLuint & ShaderProgram::getProgram() const
{
	return p;
}

const std::string & ShaderProgram::getErrorMessage() const
{
	return errormsg;
}


bool ShaderProgram::readShaderFile(const std::string & filename, std::string & shader)
{
	try {
		std::streamoff size;
		std::ifstream file(filename.c_str(), std::ios::binary);
		if (!file.is_open())
		{
			throw("Unable to open the file.");
		}
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		size -= file.tellg();
		shader.resize((size_t)size, '\0');
		file.read(&shader[0], size);
		file.close();
	}
	catch(const char * str)
	{
		errormsg = std::string("Failed to read shader file ") + filename + ": " + str;
		return false;
	}
	return true;
}

bool ShaderProgram::loadShader(const std::string & filename, GLuint & shaderid, GLenum type)
{
	std::string shader;
	GLint result, size = 0;
	const size_t infosize = 2048;
	GLchar info[infosize];


	shaderid = glCreateShader(type);
	if (!readShaderFile(filename, shader))
		return false;
	const GLchar * glshader = shader.c_str();
	glShaderSource(shaderid, 1, &glshader, NULL);
	glCompileShader(shaderid);

	glGetShaderiv(shaderid, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderid, infosize, &size, info);
		errormsg = "Shader compiling error (" + filename + "):\n" + shader + "\n" + std::string(info, size);
		return false;
	}

	return true;
}
