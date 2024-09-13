#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include "GL/glew.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#define PI 3.1415

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FRAMERATE 60.f
#define FRAMEPERIOD (1000.f / FRAMERATE)

#define INPUT_QUIT -1
#define INPUT_RESIZE 1

#endif