#ifndef SHADER
#define SHADER

#include "GL/glew.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

struct Shader{ // 2D VGF shader program, with position and direction inputs
	
	int datan;
	GLuint vbo, vao, program;
	glm::mat4 model;
	glm::mat4 projection;
	
	void set(int n); // WIP : place position & direction data separately (then hopefully allow glm::vec3 value_ptr values to be fed in)
	void update(GLfloat *pos, GLfloat *dir);
	void display(int n);
	~Shader();
	void setSize(glm::vec2 size, float scale);
};

#endif