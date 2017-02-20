#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Model.h"
//#include "texture.h"
//#include "game_object.h"


// Represents a single particle and its state
struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;
	glm::vec2 VertexPos;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
	// Constructor
	ParticleGenerator(Shader shader, Model& srcModel);
	// Update all particles
	void Update(GLfloat dt, Model &object, GLuint newParticles);
	// Render all particles
	void Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
private:
	// State
	std::vector<Particle> particles;
	GLuint amount;
	// Render state
	Shader shader;
	//Texture2D texture;
	GLuint VAO;
	// Initializes buffer and vertex attributes
	void init(Model& srcModel);
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle &particle, Model &object);
};

#endif