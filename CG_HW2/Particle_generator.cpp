/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "particle_generator.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/constants.hpp> // glm::pi

ParticleGenerator::ParticleGenerator(Shader shader, Model& srcModel)
	: shader(shader)
{
	this->init(srcModel);
}

void ParticleGenerator::Update(GLfloat dt, Model &object, GLuint newParticles)
{
	// Add new particles 
	dt = dt;
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object);
	}
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= 0.5f * dt; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position += p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

// Render all particles
void ParticleGenerator::Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			//this->shader.SetVector2f("offset", particle.Position);
			glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(glGetUniformLocation(this->shader.Program, "offset"), particle.Position.x, particle.Position.y, particle.Position.z);
			glUniform4f(glGetUniformLocation(this->shader.Program, "color"), particle.Color.r, particle.Color.g, particle.Color.b, particle.Color.w);
			//this->shader.SetVector4f("color", particle.Color);
			//this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
	}
	// Don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init(Model& srcModel)
{
	// Set up mesh and attribute properties
	GLfloat particle_cube[] = {
		-0.01f, -0.01f, -0.01f, 0.0f, 0.0f,
		0.01f, -0.01f, -0.01f, 1.0f, 0.0f,
		0.01f, 0.01f, -0.01f, 1.0f, 1.0f,
		0.01f, 0.01f, -0.01f, 1.0f, 1.0f,
		-0.01f, 0.01f, -0.01f, 0.0f, 1.0f,
		-0.01f, -0.01f, -0.01f, 0.0f, 0.0f,

		-0.01f, -0.01f, 0.01f, 0.0f, 0.0f,
		0.01f, -0.01f, 0.01f, 1.0f, 0.0f,
		0.01f, 0.01f, 0.01f, 1.0f, 1.0f,
		0.01f, 0.01f, 0.01f, 1.0f, 1.0f,
		-0.01f, 0.01f, 0.01f, 0.0f, 1.0f,
		-0.01f, -0.01f, 0.01f, 0.0f, 0.0f,

		-0.01f, 0.01f, 0.01f, 1.0f, 0.0f,
		-0.01f, 0.01f, -0.01f, 1.0f, 1.0f,
		-0.01f, -0.01f, -0.01f, 0.0f, 1.0f,
		-0.01f, -0.01f, -0.01f, 0.0f, 1.0f,
		-0.01f, -0.01f, 0.01f, 0.0f, 0.0f,
		-0.01f, 0.01f, 0.01f, 1.0f, 0.0f,

		0.01f, 0.01f, 0.01f, 1.0f, 0.0f,
		0.01f, 0.01f, -0.01f, 1.0f, 1.0f,
		0.01f, -0.01f, -0.01f, 0.0f, 1.0f,
		0.01f, -0.01f, -0.01f, 0.0f, 1.0f,
		0.01f, -0.01f, 0.01f, 0.0f, 0.0f,
		0.01f, 0.01f, 0.01f, 1.0f, 0.0f,

		-0.01f, -0.01f, -0.01f, 0.0f, 1.0f,
		0.01f, -0.01f, -0.01f, 1.0f, 1.0f,
		0.01f, -0.01f, 0.01f, 1.0f, 0.0f,
		0.01f, -0.01f, 0.01f, 1.0f, 0.0f,
		-0.01f, -0.01f, 0.01f, 0.0f, 0.0f,
		-0.01f, -0.01f, -0.01f, 0.0f, 1.0f,

		-0.01f, 0.01f, -0.01f, 0.0f, 1.0f,
		0.01f, 0.01f, -0.01f, 1.0f, 1.0f,
		0.01f, 0.01f, 0.01f, 1.0f, 0.0f,
		0.01f, 0.01f, 0.01f, 1.0f, 0.0f,
		-0.01f, 0.01f, 0.01f, 0.0f, 0.0f,
		-0.01f, 0.01f, -0.01f, 0.0f, 1.0f
	};
	GLuint VBO;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(this->VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_cube), particle_cube, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);

	// Create this->amount default particle instances
	/*for (GLuint i = 0; i < this->amount; ++i)
	this->particles.push_back(Particle());*/
	this->amount = 0;
	for (GLuint i = 0; i < srcModel.meshes.size(); i++){
		for (GLuint j = 0; j < srcModel.meshes[i].vertices.size(); j++){
			Particle particle = Particle();
			particle.Position = srcModel.meshes[i].vertices[j].Position;
			particle.Velocity = srcModel.meshes[i].vertices[j].Normal;
			particle.VertexPos.x = i;
			particle.VertexPos.y = j;
			particle.Life = 1.0f;
			this->particles.push_back(particle);
			this->amount++;
		}
	}
	int i = 0;
	return;
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i){
		if (this->particles[i].Life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i){
		if (this->particles[i].Life <= 0.0f){
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, Model &object)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = ((rand() % 100) / 100.0f);
	GLfloat gColor = ((rand() % 100) / 100.0f);
	GLfloat bColor = ((rand() % 100) / 100.0f);
	particle.Position = object.meshes[particle.VertexPos.x].vertices[particle.VertexPos.y].Position;
	particle.Velocity = ((rand() % 100) / 100.0f) * object.meshes[particle.VertexPos.x].vertices[particle.VertexPos.y].Normal;
	particle.Color = glm::vec4(rColor, gColor, bColor, 1.0f);
	particle.Life = 1.0f;
	//particle.Velocity = object.Velocity * 0.1f;
}