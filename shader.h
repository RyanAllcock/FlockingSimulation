#ifndef SHADER
#define SHADER

#include "main.h"

struct Shader{ // 2D VGF shader program, with position and direction inputs
	
	int datan;
	GLuint vbo, vao, program;
	glm::mat4 model;
	glm::mat4 projection;
	
	void set(int n);
	
	void update(GLfloat *pos, GLfloat *dir){
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * datan / 2, pos);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * datan / 2, sizeof(GLfloat) * datan / 2, dir);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void display(int n){
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, n);
		glBindVertexArray(0);
	}
	~Shader(){
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteProgram(program);
	}
	void setSize(glm::vec2 size, float scale){
		model = glm::scale(glm::mat4(1.f), glm::vec3(0.015f * scale / size.x, 0.015f * scale / size.y, 0));
		projection = glm::ortho(0.f, (float)size.x, 0.f, (float)size.y);
	}
};

void Shader::set(int n){ // WIP : place position & direction data separately (then hopefully allow glm::vec3 value_ptr values to be fed in)
	datan = n;
	
	//objects
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * datan, NULL, GL_DYNAMIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*)(sizeof(GLfloat) * datan / 2));
	glEnableVertexAttribArray(1);
	
	//shaders
	const char *vShaderSrc = 
		"#version 330 core\n"
		"layout (location = 0) in vec2 pos;"
		"layout (location = 1) in vec2 dir;"
		"uniform mat4 projection;"
		"out mat3 vert_rotate;"
		"void main(){"
			"vec3 right = vec3(0, 0, 1);"
			"vec3 dir3 = vec3(dir, 0);"
			"vec3 up = cross(dir3, right);"
			"mat3 rotate = mat3(up, dir3, right);"
			"vert_rotate = rotate;"
			"gl_Position = projection * vec4(pos, 0, 1.0);"
		"}";
	const char *gShaderSrc = 
		"#version 330 core\n"
		"layout (points) in;"
		"layout (line_strip, max_vertices = 3) out;"
		"uniform mat4 model;"
		"uniform mat4 projection;"
		"in mat3 vert_rotate[];"
		"out vec3 geom_colour;"
		"void main(){"
			
			"mat4 s = mat4(100, 0, 0, 0, 	0, 100, 0, 0, 	0, 0, 100, 0, 	0, 0, 0, 1);"
			
			"gl_Position = (gl_in[0].gl_Position + model * vec4(vert_rotate[0] * vec3(-1, -1, 0), 0));" //left wing
			"geom_colour = vec3(0, 0.5, 0);"
			"EmitVertex();"
			
			"gl_Position = (gl_in[0].gl_Position + model * vec4(vert_rotate[0] * vec3(0, 1, 0), 0));" //front
			"geom_colour = vec3(1, 1, 1);"
			"EmitVertex();"
			
			"gl_Position = (gl_in[0].gl_Position + model * vec4(vert_rotate[0] * vec3(1, -1, 0), 0));" //right wing
			"geom_colour = vec3(0.8, 0, 0);"
			"EmitVertex();"
			
			// "gl_Position = gl_in[0].gl_Position + vec4(scale * vert_rotate[0] * vec3(-1, -1, 0), 0);" //left wing
			// "geom_colour = vec3(0, 0.5, 0);"
			// "EmitVertex();"
			
			// "gl_Position = gl_in[0].gl_Position + vec4(scale * vert_rotate[0] * vec3(0, 1, 0), 0);" //front
			// "geom_colour = vec3(1, 1, 1);"
			// "EmitVertex();"
			
			// "gl_Position = gl_in[0].gl_Position + vec4(scale * vert_rotate[0] * vec3(1, -1, 0), 0);" //right wing
			// "geom_colour = vec3(0.8, 0, 0);"
			// "EmitVertex();"
			
			"EndPrimitive();"
		"}";
	const char *fShaderSrc = 
		"#version 330 core\n"
		"in vec3 geom_colour;"
		"out vec4 frag_colour;"
		"void main(){"
			"frag_colour = vec4(geom_colour, 1);"
		"}";
	
	//program
	GLint status;
	GLchar infoLog[1024];
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLint vShaderSize = sizeof(vShaderSrc);
	glShaderSource(vs, 1, &vShaderSrc, NULL);
	glCompileShader(vs);
	if(!(glGetShaderiv(vs, GL_COMPILE_STATUS, &status), status)){
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		printf("vs compile error: %s\n", infoLog);
	}
	GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(gs, 1, &gShaderSrc, NULL);
	glCompileShader(gs);
	if(!(glGetShaderiv(gs, GL_COMPILE_STATUS, &status), status)){
		glGetShaderInfoLog(gs, sizeof(infoLog), NULL, infoLog);
		printf("gs compile error: %s\n", infoLog);
	}
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fShaderSrc, NULL);
	glCompileShader(fs);
	if(!(glGetShaderiv(fs, GL_COMPILE_STATUS, &status), status)){
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		printf("fs compile error: %s\n", infoLog);
	}
	if((program = glCreateProgram()) == 0) printf("shader program not created\n");
	glAttachShader(program, vs);
	glAttachShader(program, gs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	if(!(glGetProgramiv(program, GL_LINK_STATUS, &status), status)){
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		printf("shader program error: %s\n", infoLog);
	}
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
}

#endif