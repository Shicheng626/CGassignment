#pragma once
# define M_PI           3.14159265358979323846f  /* pi */
#include "myModel.h"
#include "myCamera.h"




struct Transform
{
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform(glm::vec3 translation,glm::vec3 rotation,glm::vec3 scale) {
		this->translation = translation;
		this->rotation = rotation;
		this->scale = scale;
	}

	Transform() {
		this->translation = {0.0,0.0,0.0};
		this->rotation = { 0.0,0.0,0.0 };
		this->scale = { 1.0,1.0,1.0 };
	}
};

class GameObject {
public:
	Model* model;
	Transform transform;
	glm::vec3 color;
	float shininess = 32.0;

	GameObject(Model* model,Transform transform) {
		this->model = model;
		this->transform = transform;
		this->color = { 1.0f,1.0f,1.0f };
	}

	void LoadModel(string path) {
		try {
			model->~Model();
			model = nullptr;
			model = new Model(path);
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
		}
	}

	void Draw(Shader shader,Camera camera,vector<Light> lights) {
		glm::mat4 model_matrix;
		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;

		projection_matrix = camera.GetProjectionMatrix();
		view_matrix = camera.GetViewMatrix();

		model_matrix = glm::translate(model_matrix, transform.translation);
		model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation[0]) , glm::vec3(1.0f, 0.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation[2]) , glm::vec3(0.0f, 0.0f, 1.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));

		model_matrix = glm::scale(model_matrix, transform.scale);

		///////////////////////////////////
		GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
		
		GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
		glUniform3f(objectColorLoc, color.x, color.y, color.z);
		glUniform1f(glGetUniformLocation(shader.Program, "objectShininess"), shininess);
		for (int i = 0; i < lights.size();i++) {

			
			char p[] = "].pos_dir";
			char a[] = "].ambient";
			char d[] = "].diffuse";
			char s[] = "].specular";
			char o[] = "].on";
			char t[] = "].type";
			char s_p[20], s_a[20], s_d[20], s_s[20], s_o[20], s_t[20];
			sprintf(s_p,"lights[%d%s",i,p);
			sprintf(s_a, "lights[%d%s", i, a);
			sprintf(s_d, "lights[%d%s", i, d);
			sprintf(s_s, "lights[%d%s", i, s);
			sprintf(s_o, "lights[%d%s", i, o);
			sprintf(s_t, "lights[%d%s", i, t);
			glUniform3f(glGetUniformLocation(shader.Program, s_p), lights[i].pos_dir.x, lights[i].pos_dir.y, lights[i].pos_dir.z);
			glUniform3f(glGetUniformLocation(shader.Program, s_a), lights[i].ambient.x, lights[i].ambient.y, lights[i].ambient.z);
			glUniform3f(glGetUniformLocation(shader.Program, s_d), lights[i].diffuse.x, lights[i].diffuse.y, lights[i].diffuse.z);
			glUniform3f(glGetUniformLocation(shader.Program, s_s), lights[i].specular.x, lights[i].specular.y, lights[i].specular.z);
			glUniform1i(glGetUniformLocation(shader.Program, s_o), lights[i].on);
			glUniform1i(glGetUniformLocation(shader.Program, s_t), lights[i].type);

		}

		glUniform3f(viewPosLoc, camera.position[0], camera.position[1], camera.position[2]);
		////////////////////////////////////////////////////////////

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		//color

		//GLint vertexColorLocation = glGetUniformLocation(shader.Program, "color");
		//glUniform3f(vertexColorLocation, this->color[0], this->color[1], this->color[2]);
	
		this->model->Draw(shader);

	}

	void setupLight() {

	}
};