#pragma once
#pragma once

#include "OpenGL.h"
#include "IDrawable.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <cstdio>
#include <math.h> 
namespace Crawfis
{
	namespace Graphics
	{
		class DrawableObjectwireframe : public IDrawable
		{
		public:
			DrawableObjectwireframe(const char* filename)
			{
				bool load = loadOBJ(filename);
				if (!load) {
					std::cout << " Error load obj file .  " << std::endl;
				}
			}
			virtual void Draw()
			{
				if (!created)
					CreateObject();
				int error = glGetError();
				glBindVertexArray(vaoObject);
				// for lines
				for (int i = 0; i < vertices.size(); i += 3) {
					glDrawArrays(GL_LINES, i, 3);
				}
				error = glGetError();
			}
		private:
			bool loadOBJ(const char * filename)
			{
				// temporary temporary variables in which we will store the contents of the .obj
				std::vector< unsigned int > vertexIndices, uvIndices;
				std::vector< glm::vec3 > temp_vertices;
				std::vector< glm::vec2 > temp_uvs;
				FILE * file = fopen(filename, "r");
				if (file == NULL) {
					printf("Impossible to open the file !\n");
					return false;
				}
				while (1) {
					char lineHeader[128];
					// read the first word of the line
					int res = fscanf(file, "%s", lineHeader);
					if (res == EOF) {
						break; // EOF = End Of File. Quit the loop.
					}
					// ex. v 1.0 -1.0 -1.0
					if (strcmp(lineHeader, "v") == 0) {
						glm::vec3 vertex;
						fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
						temp_vertices.push_back(vertex);
					}
					// ex. vt 0.748573 0.750412
					else if (strcmp(lineHeader, "vt") == 0) {
						glm::vec2 uv;
						fscanf(file, "%f %f\n", &uv.x, &uv.y);
						temp_uvs.push_back(uv);
					}
					// ex. f 5/1 1/2 4/3
					else if (strcmp(lineHeader, "f") == 0) {
						std::string vertex1, vertex2, vertex3;
						unsigned int vertexIndex[3], uvIndex[3];
						int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
						if (matches != 6) {
							printf("File can't be read by our simple parser : ( Try exporting with other options\n");
							return false;
						}

						vertexIndices.push_back(vertexIndex[0]);
						vertexIndices.push_back(vertexIndex[1]);
						vertexIndices.push_back(vertexIndex[2]);
						uvIndices.push_back(uvIndex[0]);
						uvIndices.push_back(uvIndex[1]);
						uvIndices.push_back(uvIndex[2]);
					}
				}
				for (unsigned int i = 0; i < vertexIndices.size(); i++) {
					unsigned int vertexIndex = vertexIndices[i];
					glm::vec3 vertex = temp_vertices[vertexIndex - 1];
					vertices.push_back(vertex);
				}
				for (unsigned int i = 0; i < uvIndices.size(); i++) {
					unsigned int uvIndex = uvIndices[i];
					glm::vec2 uv = temp_uvs[uvIndex - 1];
					uvs.push_back(uv);
				}
			}
			void CreateObject()
			{
				// Create a Vertex Array Object to organize all of the bindings.
				glGenVertexArrays(1, &vaoObject);
				glBindVertexArray(vaoObject);
				// Steps:
				//  1) Create buffer for vertex positions
				//  2) Set vertex positions to be sent to the shaders at slot 0.
				//  3) Create buffer for colors
				//  4) Set colors to be sent to the shaders at slot 1.
				//  5) Create buffer for indices of the cube.
				//
				// Vertex positions, layout(location = 0)
				// Allocate Vertex Buffer Object (get a handle or ID)
				glGenBuffers(1, &vboVertices);
				// VBO for vertex data
				glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
				// Set the model data into the VBO.
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
				// Define the layout of the vertex data.
				// This also set's the vertex array's location for slot 0.
				glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);

				// Texture Coordinates, layout(location = 3 )
				// Allocate Vertex Buffer Object (get a handle or ID)
				glGenBuffers(1, &vboUV);
				//VBO for vertex data
				glBindBuffer(GL_ARRAY_BUFFER, vboUV);
				// Set the model data into the VBO.
				glBufferData(GL_ARRAY_BUFFER, 2 * uvs.size() * sizeof(GLfloat), &uvs[0], GL_STATIC_DRAW);
				// Define the layout of the vertex uv.
				glVertexAttribPointer((GLuint)3, 2, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(3);
				// Reset 
				glBindVertexArray(0);
				created = true;
			}
		private:
			bool created = false;
			unsigned int vaoObject, vboVertices, vboUV;
			std::vector< glm::vec3 > vertices;
			std::vector< glm::vec2 > uvs;
		};
	}
}