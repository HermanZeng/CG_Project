#ifndef MESH_H
#define MESH_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"


struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
	// OriginPos
	glm::vec3 OriginPos;
	// OriginColor
	glm::vec2 OriginTex;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	/*  Functions  */
	// Constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

	// Render the mesh
	void Draw(Shader shader);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
public:
	void setupMesh();
};

#endif
