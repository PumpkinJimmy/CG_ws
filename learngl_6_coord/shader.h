#pragma once
#include <glad/glad.h>
#include <fstream>
#include <iterator>
#include <string>
using std::string;
using std::istreambuf_iterator;
using std::ifstream;
class ShaderProgram
{
public:
	ShaderProgram(): isVShaderLoad(false), isFShaderLoad(false) {
		pid = glCreateProgram();
		vShader = glCreateShader(GL_VERTEX_SHADER);
		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	bool loadVertexShaderFromString(const char* src) {
		glShaderSource(vShader, 1, &src, NULL);
		glCompileShader(vShader);
		isVShaderLoad = true;
		if (isVShaderLoad && isFShaderLoad) {
			linkProgram();
		}
		return getVertexCompileStatus();
	}
	bool loadVertexShaderFromString(const string& src) {
		return loadVertexShaderFromString(src.c_str());
	}
	bool loadVertexShader(const string& fname) {
		ifstream fin(fname);
		return loadVertexShaderFromString(
			string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>())
		);
	}
	

	bool loadFragmentShaderFromString(const char* src) {
		glShaderSource(fShader, 1, &src, NULL);
		glCompileShader(fShader);
		isFShaderLoad = true;
		if (isVShaderLoad && isFShaderLoad) {
			linkProgram();
		}
		return getFragmentCompileStatus();
	}
	bool loadFragmentShaderFromString(const string& src) {
		return loadFragmentShaderFromString(src.c_str());
	}
	bool loadFragmentShader(const string& fname) {
		ifstream fin(fname);
		return loadFragmentShaderFromString(
			string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>())
		);
	}
	
	bool getID() const {
		return pid;
	}
	bool getVertexCompileStatus() const {
		int res;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &res);
		return res;
	}
	bool getFragmentCompileStatus() const {
		int res;
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &res);
		return res;
	}
	string getVertexCompileInfo() const {
		char infoLog[512];
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		return string(infoLog);
	}
	string getFragmentCompileInfo() const {
		char infoLog[512];
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		return string(infoLog);
	}
	bool getLinkStatus() const {
		int res;
		glGetProgramiv(pid, GL_LINK_STATUS, &res);
		return res;
	}

	string getLinkInfo() const {
		char infoLog[512];
		glGetProgramInfoLog(pid, 512, NULL, infoLog);
		return string(infoLog);
	}
	bool linkProgram() {
		glAttachShader(pid, vShader);
		glAttachShader(pid, fShader);
		glLinkProgram(pid);
		bool ok = getLinkStatus();
		if (ok) {
			glDeleteShader(vShader);
			glDeleteShader(fShader);
		}
		return ok;
	}
	void use() const {
		glUseProgram(pid);
	}
	template <typename... Args>
	void set(const string& name,Args... val);

	template <>
	void set<int>(const string& name, int val) {
		glUniform1i(glGetUniformLocation(pid, name.c_str()), val);
	}

	template <>
	void set<int, int>(const string& name, int val1, int val2) {
		glUniform2i(glGetUniformLocation(pid, name.c_str()), val1, val2);
	}

	template <>
	void set<float>(const string& name, float val) {
		glUniform1i(glGetUniformLocation(pid, name.c_str()), val);
	}

	template <>
	void set<glm::mat4>(const string& name, glm::mat4 mat) {
		glUniformMatrix4fv(glGetUniformLocation(pid, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
private:

	unsigned int vShader;
	unsigned int fShader;
	unsigned int pid;
	bool isVShaderLoad;
	bool isFShaderLoad;
};