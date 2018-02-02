#pragma once

#include "ITransformNode.h"
#include "IKeyboardHandler.h"
#include "IMouseHandler.h"
#include "OpenGL.h"
#include "ITextureDataObject.h"
#include "TextureBindManager.h"
#include "DrawableBrush.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "DrawableObject.h"
#include <iostream>

using namespace Crawfis::Controller;

namespace Crawfis
{
	namespace Graphics 
	{
		//
		// A composite ITransformNode used for moving a paint brush
		// Also supports the IKeyboardHandler and IMouseHandler interfaces.
		//
		class PaintController : public ITransformNode, public IKeyboardHandler, public IMouseHandler
		{
		public:
			//
			// Constructor. 
			//
			PaintController(std::string name, ISceneNode* subject)
			{
				this->name = name;
				windowWidth = glutGet(GLUT_WINDOW_WIDTH);
				windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
				//windowWidth = 600;
				//windowHeight = 600;


				isPainting = false;
				glm::vec3 init = glm::vec3(0.0, 0.0, 0.0);
				brushPath.push_back(init);
				//Draw();
				CreateBrush();
				int error = glGetError();
				glBindVertexArray(vaoBrush);
				glDrawArrays(GL_POINTS, 0, brushPath.size());
			}
			//
			// Overrided from ISceneNode
			//
			virtual void Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}
			//
			// Traverse the child node.
			//
			void AcceptSubject(IVisitor* visitor)
			{
				rootNode->Accept(visitor);
			}
			// 
			// Apply the transform.
			// This is a no-op here, as the real transforms are in
			// the composite.
			//
			virtual void Apply() { 	}
			// 
			// Remove or undo the application of the transform.
			// This is a no-op here, as the real transforms are in
			// the composite.
			//
			virtual void Unapply() { }
			//
			// Overrrided from ISceneNode
			//
			virtual std::string ToString()
			{
				return name;
			}
			//
			// Handle the main keyboard key presses. 
			// Called on both key down and key up.
			//
			virtual void KeyPress(unsigned char key, int x, int y)
			{
				switch(key)
				{
					case 'z':
						glutPostRedisplay();
						break;
					case 'Z':
						glutPostRedisplay();
						break;
					case 'e':
						std::cout << "erase all the brush." << std::endl;
						brushPath.clear();
						glutPostRedisplay();
					case 'g':
						glutPostRedisplay();
				}
			}
			//
			// Handle the arrow keys and numeric keypad.
			//
			virtual void NumPadPress(int key, int x, int y)
			{
				switch(key)
				{
					case GLUT_KEY_LEFT:
						glutPostRedisplay();
						break;
					case GLUT_KEY_RIGHT:
						glutPostRedisplay();
						break;
					case GLUT_KEY_UP:
						glutPostRedisplay();
						break;
					case GLUT_KEY_DOWN:
						glutPostRedisplay();
						break;
				}
			}
			//
			// Handle any mouse button events, both press and release.
			//
			virtual void MouseEvent(int button, int state, int ix, int iy)
			{
				//Draw();
				if( button == GLUT_RIGHT_BUTTON)
				{
					if( state == GLUT_DOWN)
					{
						isPainting = true;
						//Draw();
					}
					else
					{
						isPainting = false;
						//Draw();
					}
				}
			}
			//
			// Handle any mouseMove events. Note, with GLUT this 
			// is only called when a mouse button is pressed by
			// default.
			//
			virtual void MouseMoved(int ix, int iy)
			{
				
					if (isPainting)
					{
						float x, y;
						MapMouse(ix, iy, x, y);
						float z = 0.0;
						glm::vec3 tmp = glm::vec3(z, y, x);
						brushPath.push_back(tmp);
						//std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
						//Draw();
						CreateBrush();
						int error = glGetError();
						glBindVertexArray(vaoBrush);
						glDrawArrays(GL_POINTS, 0, brushPath.size());
						glutPostRedisplay();

					}
			}

		private:
			void MapMouse(int ix, int iy, float& x, float &y)
			{
				//windowWidth = 600;
				//windowHeight = 600;
				x = (float)ix / windowWidth; // map to 0 to 1
				x = -(2 * x - 1); // map to -1 to 1
				y = 1 - (float)iy / windowHeight; // map to 0 to 1
				y = -(1 - 2 * y); // map to 1 to -1 (flip the window coordinates)

				// 11/15
				
				//std::cout << "ix: " << ix << std::endl;
				/*
				if (ix <= 300) {
	
					DrawableProxy* brushNode = new DrawableProxy("Brush", "Brush");
					MaterialProxy* materialNode = new MaterialProxy("Brush Material", "brush_Textured", brushNode);
					TransformMatrixNodeSolution* brushTransform = new TransformMatrixNodeSolution("BrushSpace", materialNode);
					brushTransform->Scale(5, 1, 1);
					brushTransform->Translate(0, 1.0, 0);
					GroupNode* group = new GroupNode("Pedestal");
					group->AddChild(brushTransform);

				}
				*/
				
			}
				/*
				if (ix > 300) {

					DrawableBrush* brush = new DrawableBrush(30, 5);
					ModelManager::Instance()->RegisterModel("Brush", brush);
					DrawableProxy* brushNode = new DrawableProxy("Brush", "Brush");
					MaterialProxy* materialNode = new MaterialProxy("Object Material", "brush_Textured", brushNode);
					TransformMatrixNodeSolution* brushTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode);
					brushTransform->Scale(5, 1, 1);
					brushTransform->Translate(0, 1.0, 0);
					GroupNode* group = new GroupNode("Pedestal");
					group->AddChild(brushTransform);

				}
				*/
//////////////////////////////////////////////////////////////////////
			void CreateBrush(void)
			{
				//if (brushPath.size() <= 0)
				//	return;
				// Create a Vertex Array Object to organize all of the bindings.
				glGenVertexArrays(1, &vaoBrush);
				glBindVertexArray(vaoBrush);
				// Steps:
				//  1) Create buffer for vertex positions
				//  2) Set vertex positions to be sent to the shaders at slot 0.
				// Vertex positions, layout(location = 0)
				// Allocate Vertex Buffer Object (get a handle or ID)
				glGenBuffers(1, &vboBrush);
				// VBO for vertex data
				glBindBuffer(GL_ARRAY_BUFFER, vboBrush);
				// Set the model data into the VBO.
				glBufferData(GL_ARRAY_BUFFER, brushPath.size() * sizeof(glm::vec3), &brushPath[0], GL_STATIC_DRAW);
				// Define the layout of the vertex data.
				// This also set's the vertex array's location for slot 0.
				glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);
				//brushPath.clear();
/////////////////////////////////////////////////////////////////////////////////
			}
			void Draw() {
				CreateBrush();
				int error = glGetError();
				glBindVertexArray(vaoBrush);
				glDrawArrays(GL_POINTS, 0, brushPath.size());
				/*
				for (int i = 0; i < brushPath.size(); i += 3)
				{
					glDrawArrays(GL_POINTS, i, brushPath.size());
				}
				*/
				//std::cout << brushPath.size() << std::endl;
			}
		private:
			std::string name;
			ITransformNode* rootNode;
			bool isPainting = false;
			//bool display = false;
			int windowWidth, windowHeight;
			std::vector<glm::vec3> brushPath;
			unsigned int vaoBrush, vboBrush;
			unsigned int brushProgramID;
			GLuint brushtexture;
			DrawableBrush* brush;
		};
	}
}
