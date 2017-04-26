#pragma once
class ShaderFile
{
public:
	static const std::string vShader();
	static const std::string fShader();
	static const std::string vShaderAnim();
	static const std::string fShaderAnim();
	static const std::string vShaderText();
	static const std::string fShaderText();
	static const std::string vShaderBlur();
	static const std::string fShaderBlur();
	ShaderFile();
	~ShaderFile();
};

