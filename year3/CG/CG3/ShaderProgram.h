 
class ShaderProgram
{
public:	
	bool loadShaderProgram(const std::string & name);
	const GLuint & getProgram() const;

	const std::string & getErrorMessage() const;

private:
	std::string name;
	GLuint vs, fs, p;
	std::string errormsg;

	bool readShaderFile(const std::string & filename, std::string & shader);
	bool loadShader(const std::string & filename, GLuint & shaderid, GLenum type);
};
