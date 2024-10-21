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

int main(int argc, char *argv[]){
	
	// window
	Window window("Boids", WindowResize | WindowGraphic, WINDOW_WIDTH, WINDOW_HEIGHT, DISPLAY_RATE, INPUT_RATE);
	
	// boids
	float simulationScale = 100;
	glm::vec2 simulationSize = glm::vec2(simulationScale * window.getAspectRatio(), simulationScale);
	Boids2D boids(50, simulationSize);
	
	// shader
	Shader shader;
	shader.set(boids.n * 4);
	shader.setSize(simulationSize, simulationScale);
	
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
						shader.setSize(simulationSize, simulationScale);
						break;
					case WindowResized:
						simulationSize = glm::vec2(simulationScale * window.getAspectRatio(), simulationScale);
						boids.setSize(simulationSize);
						shader.setSize(simulationSize, simulationScale);
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
				shader.update(glm::value_ptr(boids.pos[0]), glm::value_ptr(boids.dir[0]));
			}
			
			// window
			window.clear();
			shader.display(boids.n);
			window.swap();
		}
	}
	
	return 0;
}