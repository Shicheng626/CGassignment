#pragma once


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other includes
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;


struct Light {
public:
	enum light_type
	{
		point = 0,
		direct
	};


	glm::vec3 ambient = { 1.0f, 1.0f, 1.0f };
	glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f };
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	glm::vec3 pos_dir = { 0.0f,0.0f,0.0f };

	bool on = true;
	light_type type = point;

	Light(light_type type) {
		this->type = type;
	}
};


struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
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
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
	}

	// Render the mesh
	void Draw(Shader shader)
	{
		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	glm::vec3 get_meddle() {
		vector<float> x;
		vector<float> y;
		vector<float> z;
		for (int i = 0; i < this->vertices.size(); i++) {
			x.push_back(vertices[i].Position.x);
			y.push_back(vertices[i].Position.y);
			z.push_back(vertices[i].Position.z);
		}
		sort(x.begin(), x.end());
		sort(y.begin(), y.end());
		sort(z.begin(), z.end());
		return glm::vec3((x.back() + x.front()) / 2, (y.back() + y.front()) / 2, (z.back() + z.front()) / 2);
	}


private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	GLuint lightVAO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};
