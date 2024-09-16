#include "boids.hpp"

// usage WIP optional parameters to be selected; present as separate methods/compositions or remove

Boids2D::Boids2D(int boids, glm::ivec2 size){
	
	// set parameters (version 1)
	// speedLimit = 2.1f;
	// accelLimit = .02f;
	// cruiseSpeed = .9f;
	// cruiseAccel = .5f;
	// alignRadius = 60.f;
	// separateRadius = 46.f;
	// cohereRadius = 90.f;
	// sightAngle = ((180.f / 180.f) * 3.1415f);
	// boundRange = -1;
	// separateWeight = 1.f;
	// alignWeight = 1.f;
	// cohereWeight = .05f;
	
	// set parameters (version 2)
	speedLimit = 1.f;
	accelLimit = .1f;
	cruiseSpeed = .9f;
	cruiseAccel = .5f;
	alignRadius = 15.f;
	separateRadius = 1.5f;
	cohereRadius = 15.f;
	sightAngle = ((180.f / 180.f) * 3.1415f);
	boundRange = 5.f;
	separateWeight = 1.f;
	alignWeight = .125f;
	cohereWeight = .01f;
	boundWeight = 1.f;
	
	// set environment
	n = boids;
	setSize(size);
	
	// set bodies
	body = (glm::vec2*)malloc(sizeof(glm::vec2) * 4 * n);
	if(body == NULL)
		printf("allocation error\n");
	pos = body;
	vel = &body[n];
	acc = &body[2 * n];
	dir = &body[3 * n];
	
	// set scenario
	for(int i = 0; i < n; i++){
		float value = (float)i / (float)n;
		pos[i] = value * bounds;
		vel[i] = glm::vec2(0);
		acc[i] = glm::vec2(0);
		dir[i] = glm::normalize(glm::vec2(1));
	}
	randomize();
}

Boids2D::~Boids2D(){
	free(body);
}

void Boids2D::randomize(){
	int rn = sizeof(glm::vec2) / sizeof(float);
	srand(12345678);
	for(int i = 0; i < n; i++){
		for(int r = 0; r < rn; r++) glm::value_ptr(pos[i])[r] = fmod(rand(), glm::value_ptr(bounds)[r]);
		for(int r = 0; r < rn; r++) glm::value_ptr(vel[i])[r] = (float)(rand() % 1000 - 500) / 500;
		vel[i] = glm::normalize(vel[i]) * (float)(rand() % 1000) / 1000.f * speedLimit;
		acc[i] = glm::vec2(0);
		dir[i] = glm::normalize(vel[i]);
	}
}

void Boids2D::setSize(glm::ivec2 size){
	bounds = (glm::vec2)size;
}

void Boids2D::update(float timeStep){
	
	// behaviour
	for(int i = 0; i < n; i++)
		acc[i] = behave(i);
	
	// motion
	accelerate(timeStep);
	move(timeStep);
}

// behaviour

glm::vec2 Boids2D::behave(int index){
	
	int 
		separateLocals = 0, 
		alignLocals = 0, 
		cohereLocals = 0;
	glm::vec2 
		difTotal = glm::vec2(0), 
		dirTotal = glm::vec2(0), 
		posTotal = glm::vec2(0);
	for(int j = 0; j < n; j++){
		if(index == j)
			continue;
		
		// sight (ignores boids wrapped around the edge of borders)
		glm::vec2 diff = pos[j] - pos[index];
		float dist = sqrt(glm::dot(diff, diff));
		float angle = acos(dist == 0 ? 1.f : glm::dot(dir[index], diff));
		
		// steering
		if(angle < sightAngle){ // determine whether another body is perceived
			// if(dist < separateRadius){ separateLocals++; difTotal += normalize(-diff) / dist; } // version 1
			if(dist < separateRadius){ separateLocals++; difTotal += normalize(-diff); } // version 2
			if(dist < alignRadius){ alignLocals++; dirTotal += dir[j]; }
			if(dist < cohereRadius){ cohereLocals++; posTotal += pos[j]; }
		}
	}
	
	// behaviour influence
	glm::vec2 
		separate = difTotal * separateWeight, // approach local separation from neighbours
		align = alignLocals > 0 ? (dirTotal / (float)alignLocals - vel[index]) * alignWeight : glm::vec2(0), // approach local direction of neighbours
		cohere = cohereLocals > 0 ? (posTotal / (float)cohereLocals - pos[index]) * cohereWeight : glm::vec2(0), // approach local centre of neighbours
		isolate = separateLocals + alignLocals + cohereLocals == 0 ? cruise(index) : glm::vec2(0), // isolated motion
		bound = pushBounds(index);
	// return separate + align + cohere; // version 1
	return separate + align + cohere + bound; // version 2
}

glm::vec2 Boids2D::cruise(int index){
	float speedSq = glm::dot(vel[index], vel[index]);
	if(speedSq < cruiseSpeed * cruiseSpeed)
		return glm::normalize(vel[index]) * cruiseAccel;
	return glm::vec2(0.f);
}

glm::vec2 Boids2D::pushBounds(int index){
	glm::vec2 out = glm::vec2(0);
	float *p = glm::value_ptr(pos[index]);
	float *b = glm::value_ptr(bounds);
	float *o = glm::value_ptr(out);
	int bn = sizeof(glm::vec2) / sizeof(float);
	for(int i = 0; i < bn; i++){
		if(p[i] < boundRange) o[i] += boundWeight;
		if(p[i] >= b[i] - boundRange) o[i] -= boundWeight;
	}
	return out;
}

// motion

void Boids2D::accelerate(float timeStep){
	
	for(int i = 0; i < n; i++){
		float accelSq = glm::dot(acc[i], acc[i]);
		if(accelSq > accelLimit * accelLimit)
			acc[i] = glm::normalize(acc[i]) * accelLimit;
		
		vel[i] += timeStep * acc[i];
		dir[i] = glm::normalize(vel[i]);
	}
}

void Boids2D::move(float timeStep){
	
	for(int i = 0; i < n; i++){
		float speedSq = glm::dot(vel[i], vel[i]);
		if(speedSq > speedLimit * speedLimit)
			vel[i] = glm::normalize(vel[i]) * speedLimit;
		
		pos[i] += timeStep * vel[i];
		
		// wrapBounds(i); // version 1
	}
}

// extra

void Boids2D::wrapBounds(int index){
	float *p = glm::value_ptr(pos[index]);
	float *b = glm::value_ptr(bounds);
	int bn = sizeof(glm::vec2) / sizeof(float);
	for(int i = 0; i < bn; i++){
		if(p[i] < 0) p[i] += b[i];
		if(p[i] >= b[i]) p[i] -= b[i];
	}
}