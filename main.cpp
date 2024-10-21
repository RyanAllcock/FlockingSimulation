#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "lib/shader.hpp"
#include "source/boids.hpp"

#define PI 3.1415

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FRAMERATE 60.f
#define FRAMEPERIOD (1000.f / FRAMERATE)

#define INPUT_QUIT -1
#define INPUT_RESIZE 1

int getInput(glm::ivec2 *windowSize){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				return INPUT_QUIT;
				break;
			case SDL_WINDOWEVENT:
				return INPUT_RESIZE;
				break;
			default:
				break;
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	
	//data
	glm::ivec2 windowSize = glm::ivec2(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	//init SDL + OpenGL
	SDL_Window *window = NULL;
	SDL_GLContext context = NULL;
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL init failed: %s\n", SDL_GetError());
		return -1;
	}
	if((window = SDL_CreateWindow("Boids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)) == NULL){
		printf("SDL window creation failed: %s\n", SDL_GetError());
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	if((context = SDL_GL_CreateContext(window)) == NULL){
		printf("GL context creation failed: %s\n", SDL_GetError());
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	GLfloat ratio = (float)windowSize.x / (float)windowSize.y;
	
	//OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//GLEW init
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK){
		printf("GLEW init failed: %s\n", glewGetErrorString(err));
		return -1;
	}
	
	//data
	float simScale = 100;
	glm::vec2 simSize = glm::vec2(simScale * ratio, simScale);
	Boids2D boids(50, simSize);
	
	//shader
	Shader shader;
	shader.set(boids.n * 4);
	shader.setSize(simSize, simScale);
	
	//display
	glClearColor(0, 0, 0, 1.f);
	
	//loop
	const float stepMult = 1000.f;
	const float timeStep = .001f;
	clock_t tstep = (clock_t)(CLOCKS_PER_SEC * timeStep);
	clock_t tcur = clock();
	clock_t tframe = (clock_t)(CLOCKS_PER_SEC * FRAMEPERIOD / 1000.f);
	clock_t tprev = clock();
	clock_t tcurrent = clock();
	bool running = true;
	while(running){
		
		//frame limit
		tcurrent = clock();
		if(tcurrent > tprev + tframe){
			tprev += tframe;
				
			//input
			switch(getInput(&windowSize)){
				case INPUT_QUIT:
					running = false;
					break;
				case INPUT_RESIZE:
					SDL_GetWindowSize(window, (int*)&windowSize.x, (int*)&windowSize.y);
					glViewport(0, 0, windowSize.x, windowSize.y);
					ratio = (float)windowSize.x / (float)windowSize.y;
					simSize = glm::vec2(simScale * ratio, simScale);
					boids.setSize(simSize);
					shader.setSize(simSize, simScale);
					break;
				default:
					break;
			}
			
			//time step
			if(tprev > tcur + tstep){
				tcur += tstep;
				
				//simulation
				boids.update(timeStep * stepMult);
				shader.update(glm::value_ptr(boids.pos[0]), glm::value_ptr(boids.dir[0]));
			}
			
			//display
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader.display(boids.n);
			SDL_GL_SwapWindow(window);
			SDL_Delay(FRAMEPERIOD);
		}
	}
	
	//cleanup
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}