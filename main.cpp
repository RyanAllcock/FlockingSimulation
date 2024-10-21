#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "lib/window/window.hpp"
#include "lib/shader.hpp"
#include "source/boids.hpp"

#define PI 3.1415

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DISPLAY_RATE 144.f
#define INPUT_RATE 60.f

#define SIMULATION_RATE 60.f
#define SIMULATION_STEP .4f

void setUniforms(Program &boidProgram, glm::vec2 size, float scale){
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.015f * scale / size.x, 0.015f * scale / size.y, 0));
	glm::mat4 projection = glm::ortho(0.f, (float)size.x, 0.f, (float)size.y);
	std::array<float,16> modelArray;
	std::array<float,16> projectionArray;
	for(int i = 0; i < 16; i++){
		modelArray[i] = glm::value_ptr(model)[i];
		projectionArray[i] = glm::value_ptr(projection)[i];
	}
	boidProgram.setUniform("model", DataMatrix4(modelArray, DataUnchanged));
	boidProgram.setUniform("projection", DataMatrix4(projectionArray, DataUnchanged));
}

int main(int argc, char *argv[]){
	
	// window
	Window window("Boids", WindowResize | WindowGraphic, WINDOW_WIDTH, WINDOW_HEIGHT, DISPLAY_RATE, INPUT_RATE);
	
	// boids
	float simulationScale = 100;
	glm::vec2 simulationSize = glm::vec2(simulationScale * window.getAspectRatio(), simulationScale);
	Boids2D boids(50, simulationSize);
	
	// shader sources
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
			
			// "gl_Position = gl_in[0].gl_Position + vec4(scale * vert_rotate[0] * vec3(-1, -1, 0), 0);" //left wing WIP
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
	
	// shader
	Buffer boidBuffer(BufferDynamic, NULL, sizeof(float) * boids.n * 4);
	Index pointIndex(boidBuffer, 2, IndexFloat, IndexUnchanged, sizeof(float) * 2, 0);
	Index directionIndex(boidBuffer, 2, IndexFloat, IndexUnchanged, sizeof(float) * 2, (void*)(sizeof(float) * boids.n * 2));
	Shader vertexShader(ShaderVertex, std::vector<const char*>{vShaderSrc});
	Shader geometryShader(ShaderGeometry, std::vector<const char*>{gShaderSrc});
	Shader fragmentShader(ShaderFragment, std::vector<const char*>{fShaderSrc});
	Program boidProgram(std::vector<Shader*>{&vertexShader, &geometryShader, &fragmentShader});
	DrawArray boidDraw(DrawPoint, std::vector<Index*>{&pointIndex, &directionIndex}, boids.n);
	Renderer boidRenderer(boidProgram, boidDraw);
	setUniforms(boidProgram, simulationSize, simulationScale);
	
	// display
	glClearColor(0, 0, 0, 1.f);
	
	// loop
	window.timer();
	float simulationCurrent = 0;
	bool isRunning = true;
	while(isRunning){
		
		// input
		if(window.cap(WindowInput)){
			
			// window
			WindowState windowState;
			while((windowState = window.get(), windowState) != WindowDefault){
				switch(windowState){
					case WindowQuit:
						isRunning = false;
						break;
					case WindowResizing:
						simulationSize = glm::vec2(simulationScale * window.getAspectRatio(), simulationScale);
						boids.setSize(simulationSize);
						setUniforms(boidProgram, simulationSize, simulationScale);
						break;
					case WindowResized:
						simulationSize = glm::vec2(simulationScale * window.getAspectRatio(), simulationScale);
						boids.setSize(simulationSize);
						setUniforms(boidProgram, simulationSize, simulationScale);
						break;
				}
			}
		}
		
		// display
		if(window.cap(WindowDisplay)){
			simulationCurrent += SIMULATION_RATE / DISPLAY_RATE;
			
			// simulation
			if(simulationCurrent > 1){
				simulationCurrent--;
				boids.update(SIMULATION_STEP);
				boidBuffer.update(glm::value_ptr(boids.pos[0]), sizeof(GLfloat) * boids.n * 2, 0);
				boidBuffer.update(glm::value_ptr(boids.dir[0]), sizeof(GLfloat) * boids.n * 2, sizeof(GLfloat) * boids.n * 2);
			}
			
			// window
			window.clear();
			boidRenderer.display();
			window.swap();
		}
	}
	
	return 0;
}