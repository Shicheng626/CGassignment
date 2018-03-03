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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;


struct impVertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

class myImporter{
public:

	vector<impVertex> vertices;
	vector<GLuint> indices;

void load_obj(string obj_path ) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coods;
	ifstream ifs;
	try {
		ifs.open(obj_path);
		string one_line;
		while (getline(ifs, one_line)) {
			stringstream ss(one_line);
			string type;
			ss >> type;
			if (type == "v") {
				glm::vec3 vert_pos;
				ss >> vert_pos[0] >> vert_pos[1] >> vert_pos[2];
				positions.push_back(vert_pos);
			}
			else if (type == "vt") {
				glm::vec2 tex_coord;
				ss >> tex_coord[0] >> tex_coord[1];
				tex_coods.push_back(tex_coord);
			}
			else if (type == "vn") {
				glm::vec3 vert_norm;
				ss >> vert_norm[0] >> vert_norm[1] >> vert_norm[2];
				normals.push_back(vert_norm);
			}
			else if (type == "f") {
				string s_vertex_0, s_vertex_1, s_vertex_2;
				ss >> s_vertex_0 >> s_vertex_1 >> s_vertex_2;
				int pos_idx, tex_idx, norm_idx;
				sscanf(s_vertex_0.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				// We have to use index -1 because the obj index starts at 1
				impVertex vertex_0;
				vertex_0.Position = positions[pos_idx - 1];
				vertex_0.TexCoords = tex_coods[tex_idx - 1];
				vertex_0.Normal = normals[norm_idx - 1];
				sscanf(s_vertex_1.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				impVertex vertex_1;
				vertex_1.Position = positions[pos_idx - 1];
				vertex_1.TexCoords = tex_coods[tex_idx - 1];
				vertex_1.Normal = normals[norm_idx - 1];
				sscanf(s_vertex_2.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				impVertex vertex_2;
				vertex_2.Position = positions[pos_idx - 1];
				vertex_2.TexCoords = tex_coods[tex_idx - 1];
				vertex_2.Normal = normals[norm_idx - 1];

				vertices.push_back(vertex_0);
				indices.push_back(vertices.size() - 1);
				vertices.push_back(vertex_1);
				indices.push_back(vertices.size() - 1);
				vertices.push_back(vertex_2);
				indices.push_back(vertices.size() - 1);
			}
		}
	}
	catch (const std::exception&) {
		cout << "Error: Obj file cannot be read\n";
	}
}
};
