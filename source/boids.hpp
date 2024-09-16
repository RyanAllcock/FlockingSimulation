#ifndef BOIDS
#define BOIDS

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

struct Boids2D{
	
	// parameters
	float speedLimit, accelLimit; // basic motion limit
	float cruiseSpeed, cruiseAccel; // isolated motion
	float alignRadius, separateRadius, cohereRadius, sightAngle, boundRange; // sight & range
	float separateWeight, alignWeight, cohereWeight, boundWeight; // behaviour rules
	
	// data
	int n;
	glm::vec2 *body, *pos, *vel, *acc, *dir; // serialised position, velocity, acceleration, and normalised direction
	glm::vec2 bounds; // outer restrictive bounding box
	
	// usage functions
	Boids2D(int boids, glm::ivec2 size); // initialise
	~Boids2D();
	void randomize(); // randomise positions and directions
	void setSize(glm::ivec2 size); // set bounding box
	void update(float timeStep); // update bodies using behaviour
	
	// internal functions
	protected:
		
		// behaviour
		glm::vec2 behave(int index); // produce behavioural influence on body acceleration
		glm::vec2 cruise(int index); // approach cruise speed when isolated
		glm::vec2 pushBounds(int index); // avoid borders of environment
		
		// motion
		void accelerate(float timeStep);
		void move(float timeStep);
		
		// extra
		void wrapBounds(int index);
};

#endif