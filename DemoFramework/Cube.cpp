#include "OpenGL.h"

#include <string>
#include <vector>

// Includes to create the cube
#include "ISceneNode.h"
#include "ModelManager.h"
#include "DrawableProxy.h"
#include "DrawableCubeSolution.h"
#include "DrawableSphereSolution.h"
#include "DrawableFloorSolution.h"

// Material includes
#include "SolidColorMaterialSolution.h"
#include "ShadedMaterial.h"
#include "MaterialProxy.h"
#include "MaterialManager.h"
#include "ShaderConstantMaterial.h"
#include "Color.h"

#include "BlankTexture2D.h"
#include "TextureBinding.h"
#include "TextureBindManager.h"
#include "TextureBindManagerOpenGL.h"
#include "TextureDataImage.h"
#include "SamplerApplicator.h"
#include "SimpleShaderMaterial.h"
#include "TexParam2DNoMipMap.h"
#include "TexParam2DMipMap.h"

// Includes for the camera
#include "ExaminerCameraNode.h"
#include "PerspectiveTransformSolution.h"
#include "LookAtTransformSolution.h"
#include "ShaderConstantModelView.h"

// Lights
#include "PointLight.h"
#include "DirectionalLight.h"
#include "LightManager.h"
#include "ShaderConstantLights.h"

#include "RenderTargetProxy.h"
#include "RenderTarget.h"
#include "RenderManager.h"
#include "ClearFrameCommand.h"
#include "SwapCommand.h"

// Includes for walking the scene graph
#include "DebugRenderVisitor.h"
#include "PrintSceneVisitor.h"
// 1108
#include "PaintController.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "DrawableObject.h"
#include "DrawableBrush.h"
#include "DrawableObjectwireframe.h"
// Interaction
std::vector<IMouseHandler*> mouseHandlers;
std::vector<IKeyboardHandler*> keyboardHandlers;


using namespace Crawfis::Graphics;
using namespace Crawfis::Math;
using namespace std;


ISceneNode* rootSceneNode;
// 11/13
ISceneNode* rootSceneNode2;
ISceneNode* rootSceneNode3;
ISceneNode* rootSceneNode4;

IVisitor* renderVisitor;
ExaminerCameraNode* examiner;
// 11/13
ExaminerCameraNode* examiner2;
ExaminerCameraNode* examiner3;
ExaminerCameraNode* examiner4;

// 11/15
PaintController *paint;
// 11/16
// create an empty texture
ITextureDataObject* brushtexture;


int windowGUID;
int windowWidth;
int windowHeight; 

void CreateGLUTWindow(std::string windowTitle)
{
	windowWidth = 800;
	windowHeight = 600;
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	windowGUID = glutCreateWindow(windowTitle.c_str());
}

void InitializeOpenGLExtensions()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		throw "Error initializing GLEW";
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void InitializeDevices()
{
	CreateGLUTWindow("OpenGL lab2 Su");
	InitializeOpenGLExtensions();
	glDisable(GL_CULL_FACE);
}

void CreateLights()
{
	PointLight* pointLight = new PointLight("light0-pt");
	pointLight->setPosition(Vector3(-2, 5, -1));

	LightManager::Instance()->SetLight(0, pointLight);
	DirectionalLight* dirLight = new DirectionalLight("light1-dir");
	//dirLight->setColor(Colors::IndianRed);
	dirLight->setDirection(Vector3(10, 1, 1));
	LightManager::Instance()->SetLight(1, dirLight);
}
void CreateGoldMaterial()
{
	VertexRoutine* vertexRoutine = new VertexRoutine("..\\Media\\Shaders\\VertexLight.glsl");
	FragmentRoutine* fragmentRoutine = new FragmentRoutine("..\\Media\\Shaders\\SolidColorSolution.frag");
	
	IShaderProgram* shaderProgram = new ShaderProgramWithMatrices(vertexRoutine, fragmentRoutine);

	Color gold(0.8314f, 0.6863f, 0.2169f, 1.0f);
	ShadedMaterial* shinyGold = new ShadedMaterial(shaderProgram);
	shinyGold->setAmbientReflection(0.01f*gold);
	shinyGold->setDiffuseReflection(0.7f*gold);
	shinyGold->setSpecularReflection(0.25f*gold);
	shinyGold->setShininess(100.0f);

	ShaderConstantMaterial* materialConstant = new ShaderConstantMaterial("frontMaterial");
	materialConstant->setValue(shinyGold);
	ShaderConstantLights* lightConstant = new ShaderConstantLights();
	ShaderConstantCollection* shaderConstantList = new ShaderConstantCollection();
	shaderConstantList->AddConstant(materialConstant);
	shaderConstantList->AddConstant(lightConstant);
	shaderProgram->setShaderConstant(shaderConstantList);

	MaterialManager::Instance()->RegisterMaterial("ShinyGold", shinyGold);
}

void CreateTexturedMaterial()
{
	// texture for normal map
	ITextureDataObject* camelTexture = new TextureDataImage("../Media/Textures/camel_normals.png", GL_RGB);
	camelTexture->setTextureParams(&TexParam2DMipMap::Instance);
	SamplerApplicator* uniformBinding2 = new SamplerApplicator("normaltexture");
	TextureBinding* binding2 = TextureBindManager::Instance()->CreateBinding(camelTexture, uniformBinding2);
	//binding2->Enable();
	//binding2->Disable();




/////////////////////////////////////////////////////////////////
	// texture for diffuse color
	//ITextureDataObject* diffuseTexture = new TextureDataImage("../Media/Textures/UVGrid.jpg", GL_RGB);
	//diffuseTexture->setTextureParams(&TexParam2DMipMap::Instance);
	//SamplerApplicator* uniformBinding3 = new SamplerApplicator("diffusetexture");
	//TextureBinding* binding3 = TextureBindManager::Instance()->CreateBinding(diffuseTexture, uniformBinding3);
/////////////////////////////////////////////////////////////
	// 11/16
	//ITextureDataObject* brushtexture = new BlankTexture2D(1024, 1024);
	ITextureDataObject* brushtexture = new BlankTexture2D(1024, 1024, Color(1, 1, 1, 1), GL_RGB);
	brushtexture->setTextureParams(&TexParam2DMipMap::Instance);
	SamplerApplicator* uniformBinding3 = new SamplerApplicator("diffusetexture");
	TextureBinding* binding3 = TextureBindManager::Instance()->CreateBinding(brushtexture, uniformBinding3);




//////////////////////////////////////////////////////////////////////////////////
	// 11/12 step7
	VertexRoutine* step7_vs = new VertexRoutine("..\\Media\\Shaders\\step7.vert");
	FragmentRoutine* step7_fs = new FragmentRoutine("..\\Media\\Shaders\\step7.frag");
	IShaderProgram* shaderProgram = new ShaderProgramWithMatrices(step7_vs, step7_fs);
	SimpleShaderMaterial* texturedMaterial = new SimpleShaderMaterial(shaderProgram);


	// 11/ 15
	//SimpleShaderMaterial* texturedMaterial = new SimpleShaderMaterial(shaderProgram);
	texturedMaterial->setShaderConstant(uniformBinding2);
	texturedMaterial->setShaderConstant(uniformBinding3);
	texturedMaterial->AddTexture(binding2);
	texturedMaterial->AddTexture(binding3);
///////////////////////////////////////////////////////////
	// original vs, fs, shaderprogram
	//VertexRoutine* vertexRoutine = new VertexRoutine("..\\Media\\Shaders\\ShadedTextured-vert.glsl");
	//FragmentRoutine* fragmentRoutine = new FragmentRoutine("..\\Media\\Shaders\\Textured.frag");
	//IShaderProgram* shaderProgram = new ShaderProgramWithMatrices(vertexRoutine, fragmentRoutine);
/////////////////////////////////////////////////////////////////////////	
	// step 1 for scene 2
	VertexRoutine * step1_vs = new VertexRoutine("..\\Media\\Shaders\\step1.vert");
	FragmentRoutine* step1_fs = new FragmentRoutine("..\\Media\\Shaders\\step1.frag");
	IShaderProgram* step1_shaderProgram = new ShaderProgramWithMatrices(step1_vs, step1_fs);
	SimpleShaderMaterial* texturedMaterial2 = new SimpleShaderMaterial(step1_shaderProgram);
////////////////////////////////////////////////////////////////////////
	// colorful uv plot for scene 3
	VertexRoutine* uv_vs = new VertexRoutine("..\\Media\\Shaders\\uv.vert");
	FragmentRoutine* uv_fs = new FragmentRoutine("..\\Media\\Shaders\\uv.frag");
	IShaderProgram* uv_shaderProgram = new ShaderProgramWithMatrices(uv_vs, uv_fs);
	SimpleShaderMaterial* texturedMaterial3 = new SimpleShaderMaterial(uv_shaderProgram);
////////////////////////////////////////////////////////////////////////////////////
	// for scene 4

	ITextureDataObject* imageTexture = new TextureDataImage("../Media/Textures/UVGrid.jpg", GL_RGB);
	imageTexture->setTextureParams(&TexParam2DMipMap::Instance);
	SamplerApplicator* uniformBinding = new SamplerApplicator("texture");
	TextureBinding* binding = TextureBindManager::Instance()->CreateBinding(imageTexture, uniformBinding);
	binding->Enable();
	binding->Disable();
	VertexRoutine* step3_vs = new VertexRoutine("..\\Media\\Shaders\\step3.vert");
	FragmentRoutine* step3_fs = new FragmentRoutine("..\\Media\\Shaders\\step3.frag");
	IShaderProgram* step3_shaderProgram = new ShaderProgramWithMatrices(step3_vs, step3_fs);
	SimpleShaderMaterial* texturedMaterial4 = new SimpleShaderMaterial(step3_shaderProgram);
	texturedMaterial4->setShaderConstant(uniformBinding);
	texturedMaterial4->AddTexture(binding);
/////////////////////////////////////////////////////////////////////////
	// for brush
	ITextureDataObject* redtexture = new BlankTexture2D(1024, 1024, Color(1, 0, 0, 1), GL_RGB);
	redtexture->setTextureParams(&TexParam2DMipMap::Instance);
	VertexRoutine * brush_vs = new VertexRoutine("..\\Media\\Shaders\\brush.vert");
	GeometryRoutine * brush_gs = new GeometryRoutine("..\\Media\\Shaders\\brush.geom");
	FragmentRoutine* brush_fs = new FragmentRoutine("..\\Media\\Shaders\\brush.frag");
	IShaderProgram* brush_shaderProgram = new ShaderProgramWithMatrices(brush_vs, brush_gs, brush_fs);
	SimpleShaderMaterial* texturedMaterial5 = new SimpleShaderMaterial(brush_shaderProgram);
	SamplerApplicator* uniformBinding6 = new SamplerApplicator("brushTexture");
	TextureBinding* binding6 = TextureBindManager::Instance()->CreateBinding(redtexture, uniformBinding6);
	texturedMaterial5->setShaderConstant(uniformBinding6);
	texturedMaterial5->AddTexture(binding6);
////////////////////////////////////////////////////////////////////////////////
	ShadedMaterial* white = new ShadedMaterial(shaderProgram);
	white->setAmbientReflection(0.0f*Colors::White);
	white->setDiffuseReflection(0.75f*Colors::White);
	white->setSpecularReflection(0.25f*Colors::White);
	white->setShininess(10.0f);
	ShaderConstantMaterial* materialConstant = new ShaderConstantMaterial("frontMaterial");
	materialConstant->setValue(white);
	ShaderConstantLights* lightConstant = new ShaderConstantLights();
	ShaderConstantCollection* shaderConstantList = new ShaderConstantCollection();
	shaderConstantList->AddConstant(materialConstant);
	shaderConstantList->AddConstant(lightConstant);
	// 11/15

	shaderConstantList->AddConstant(uniformBinding2);
	shaderConstantList->AddConstant(uniformBinding3);

	shaderProgram->setShaderConstant(shaderConstantList);
/////////////////////////////////////////////////////////////////////////////////////
	ShadedMaterial* white2 = new ShadedMaterial(step3_shaderProgram);
	white2->setAmbientReflection(0.0f*Colors::White);
	white2->setDiffuseReflection(0.75f*Colors::White);
	white2->setSpecularReflection(0.25f*Colors::White);
	white2->setShininess(10.0f);
	ShaderConstantMaterial* materialConstant2 = new ShaderConstantMaterial("frontMaterial");
	materialConstant2->setValue(white2);
	ShaderConstantLights* lightConstant2 = new ShaderConstantLights();
	ShaderConstantCollection* shaderConstantList2 = new ShaderConstantCollection();
	shaderConstantList2->AddConstant(materialConstant2);
	shaderConstantList2->AddConstant(lightConstant2);
	step3_shaderProgram->setShaderConstant(shaderConstantList2);
/////////////////////////////////////////////////////////////////////////////////
	MaterialManager::Instance()->RegisterMaterial("Textured", texturedMaterial);
	MaterialManager::Instance()->RegisterMaterial("step1_Textured", texturedMaterial2);
	MaterialManager::Instance()->RegisterMaterial("uv_Textured", texturedMaterial3);
	MaterialManager::Instance()->RegisterMaterial("step3_Textured", texturedMaterial4);
	// 11/16
	MaterialManager::Instance()->RegisterMaterial("brush_Textured", texturedMaterial5);

}



ISceneNode* CreateSceneGraph()
{
	// Create a simple scene
	// Perspective
	// LookAt camera
	// Drawable Cube
	//
	// First, create the models and register them.
	//DrawableSphereSolution* sphere = new DrawableSphereSolution(10000);
	//sphere->setLevel(6);
	//ModelManager::Instance()->RegisterModel("Sphere", sphere);
	//DrawableSphereSolution* smoothSphere = new DrawableSphereSolution(100000);
	//ModelManager::Instance()->RegisterModel("SmoothSphere", smoothSphere);
	//IDrawable* cube = new DrawableCubeSolution();
	//ModelManager::Instance()->RegisterModel("Cube", cube);
	//DrawableFloor* floor = new DrawableFloor(10,10);
	//ModelManager::Instance()->RegisterModel("Floor", floor);

	DrawableObject * object= new DrawableObject("..\\Media\\camel_decim.obj");
	ModelManager::Instance()->RegisterModel("Object", object);
	DrawableBrush  * brush = new DrawableBrush(5,5);
	ModelManager::Instance()->RegisterModel("Brush", brush);

	//DrawableProxy* sphereNode = new DrawableProxy("Sphere", "SmoothSphere");
	//DrawableProxy* sphereNode2 = new DrawableProxy("Sphere", "Sphere");
	//DrawableProxy* cubeNode = new DrawableProxy("Cube", "Cube"); // It is okay if they have the same name.
	//DrawableProxy* floorNode = new DrawableProxy("Floor", "Floor"); 	// It is okay if they have the same name.
	DrawableProxy* objectNode = new DrawableProxy("Object", "Object");
	DrawableProxy* brushNode = new DrawableProxy("Brush", "Brush");

	// Add a material
	//SolidColorMaterialSolution* scarlet = new SolidColorMaterialSolution(Colors::White);
	//MaterialManager::Instance()->RegisterMaterial("Scarlet", scarlet);

	CreateLights();
	CreateGoldMaterial();
	CreateTexturedMaterial();



	//MaterialProxy* materialNode2 = new MaterialProxy("Floor Material", "step3_Textured", floorNode);
	//TransformMatrixNodeSolution* floorTransform = new TransformMatrixNodeSolution("CubeSpace", materialNode2);
	//floorTransform->Scale(10, 1, 10);
	//floorTransform->Rotate(1.57f, Vector3(1, 0, 0));
	//floorTransform->Translate(1.0, -1.1, 0);

	// object
	MaterialProxy* materialNode = new MaterialProxy("Object Material", "Textured", objectNode);
	TransformMatrixNodeSolution* objectTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode);
	objectTransform->Scale(3, 3, 3);
	//objectTransform->Rotate(1.57f, Vector3(1, 0, 0));
	objectTransform->Translate(0, 1.0, 0);
	// brush
	MaterialProxy* materialNode3 = new MaterialProxy("Brush Material", "step3_Textured", brushNode);
	TransformMatrixNodeSolution* brushTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode3);
	brushTransform->Scale(3, 3, 3);
	//brushTransform->Rotate(1.57f, Vector3(1, 0, 0));
	brushTransform->Translate(0, -1.0, 0);



	//MaterialProxy* cubeMaterialNode = new MaterialProxy("Cube Material", "Scarlet", cubeNode);
	//TransformMatrixNodeSolution* cubeTransform = new TransformMatrixNodeSolution("CubeSpace", cubeMaterialNode);
	//cubeTransform->Scale(1, 2, 1);
	//cubeTransform->Translate(0, 1.0, 0);
	//MaterialProxy* sphereMaterialNode = new MaterialProxy("Sphere Material", "ShinyGold", sphereNode);
	//TransformMatrixNodeSolution* sphereTransform = new TransformMatrixNodeSolution("SphereSpace", sphereMaterialNode);
	//sphereTransform->Scale(1, 1, 1);
	//sphereTransform->Translate(0, 3.9, 0);
	GroupNode* group = new GroupNode("Pedestal");
	//group->AddChild(sphereTransform);
	//group->AddChild(cubeTransform);
	//group->AddChild(floorTransform);
	group->AddChild(objectTransform);
	group->AddChild(brushTransform);
	//Vector3 eyePosition = Vector3(-5.0,43.0,-4.998);
	//Vector3 centerOfInterest = Vector3(0,13,0);
	//Vector3 viewUp = Vector3(0,1,0);
	examiner = new ExaminerCameraNode("Examiner", group);
	examiner->setWidth(windowWidth);
	examiner->setHeight(windowHeight);
/////////////////////////////////////////////////
	// for brush
	// 11/10
	//paint = new PaintController("Painter", group);
	//ISceneNode* lookAt = new LookAtTransformSolution("LookAt", materialNode, eyePosition, centerOfInterest, viewUp);
	//ISceneNode* rootNode = new PerspectiveTransformSolution("Perspective", lookAt, 30, windowWidth, windowHeight, 0.1, 100);
	return examiner;
}
///////////////////////////////////////////////////

ISceneNode* CreateSceneGraph2()
{
	DrawableObjectwireframe * object = new DrawableObjectwireframe("..\\Media\\camel_decim.obj");
	ModelManager::Instance()->RegisterModel("WObject", object);
	DrawableProxy* objectNode = new DrawableProxy("WObject", "WObject");
	CreateLights();
	CreateGoldMaterial();
	CreateTexturedMaterial();
	MaterialProxy* materialNode = new MaterialProxy("Object Material", "step1_Textured", objectNode);
	TransformMatrixNodeSolution* objectTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode);
	objectTransform->Scale(3, 3, 3);
	objectTransform->Translate(0, 1.0, 0);
	GroupNode* group = new GroupNode("Pedestal");
	group->AddChild(objectTransform);
	examiner2 = new ExaminerCameraNode("Examiner", group);
	examiner2->setWidth(windowWidth);
	examiner2->setHeight(windowHeight);
	return examiner2;
}



ISceneNode* CreateSceneGraph3()
{
	DrawableObject * object = new DrawableObject("..\\Media\\camel_decim.obj");
	ModelManager::Instance()->RegisterModel("Object", object);
	DrawableProxy* objectNode = new DrawableProxy("Object", "Object");
	CreateLights();
	CreateGoldMaterial();
	CreateTexturedMaterial();
	MaterialProxy* materialNode = new MaterialProxy("Object Material", "uv_Textured", objectNode);
	TransformMatrixNodeSolution* objectTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode);
	objectTransform->Scale(3, 3, 3);
	objectTransform->Translate(0, 1.0, 0);
	GroupNode* group = new GroupNode("Pedestal");
	group->AddChild(objectTransform);
	examiner3 = new ExaminerCameraNode("Examiner", group);
	examiner3->setWidth(windowWidth);
	examiner3->setHeight(windowHeight);
	return examiner3;
}

/*
ISceneNode* BrushFrameBuffer(Crawfis::Graphics::ISceneNode * scene)
{
	IRenderTarget* brushscreen = new RenderTarget();
	RenderManager::Instance()->RegisterRenderTarget("BrushScreen", brushscreen);
	brushscreen->setEnableCommand(new ClearFrameCommand(Colors::IndianRed));
	brushscreen->setDisableCommand(new SwapCommand(true));
	RenderTargetProxy* brushframeBuffer = new RenderTargetProxy("Brush Screen Display", "BrushScreen", scene);
	return brushframeBuffer;
}
*/

ISceneNode* CreateSceneGraph4()
{
	/*
	DrawableBrush *brush = new DrawableBrush(5, 5);
	ModelManager::Instance()->RegisterModel("Brush", brush);
	DrawableProxy* objectNode = new DrawableProxy("Object", "Object");
	DrawableProxy* brushNode = new DrawableProxy("Brush", "Brush");
	MaterialProxy* materialNode5 = new MaterialProxy("Object Material", "step3_Textured", objectNode);
	TransformMatrixNodeSolution* objectTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode5);
	objectTransform->Scale(3, 3, 3);
	objectTransform->Translate(0, 1.0, 0);
	MaterialProxy* materialNode3 = new MaterialProxy("Brush Material", "step3_Textured", brushNode);
	TransformMatrixNodeSolution* brushTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode3);
	brushTransform->Scale(1, 1, 1);
	brushTransform->Translate(0, -1.0, 0);
	GroupNode* group = new GroupNode("Pedestal");
	paint = new PaintController("Painter", group);
	group->AddChild(objectTransform);
	group->AddChild(brushTransform);
	examiner4 = new ExaminerCameraNode("Examiner", group);
	examiner4->setWidth(windowWidth);
	examiner4->setHeight(windowHeight);
	
	
	
	return examiner4;
	*/

	// 3. Write a shader that colors the object according to its
	// texture coordinates.
	// 4. add a frambuffer object support to your system 
	// such that you can render to a texture
	/*
	// Create the brush texture
	const int size = 256;
	glGenTextures(1, &brushTexture);
	glBindTexture(GL_TEXTURE_2D, brushTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create a FBO
	unsigned int fboName;
	glGenFramebuffers(1, &fboName);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboName);
	glViewport(0, 0, size, size);

	// Attach the Texture to the Framebuffer.
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brushTexture, 0);
	// Check for FBO completeness.
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
		throw "Problem creating the FBO";
	// Render a quad into the FBO.
	//glUseProgram(gaussianProgramID);

	// Allocate Vertex Buffer Object (get a handle or ID)
	// No VAO, since we are only doing this once.
	unsigned int vertexBufferID;
	glGenBuffers(1, &vertexBufferID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	// Set the model data into the VBO.
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
	// Define the layout of the vertex data.
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Draw! But draw into the FBO.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Clean up
	// Unbind and delete the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertexBufferID);

	//Unbind the Program (could have created it and deleted it here as well).
	glUseProgram(0);
	// Un-bind the FBO and delete it. Note: the texture still remains.
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fboName);

	// Generate the mip-maps for the texture.
	glBindTexture(GL_TEXTURE_2D, brushTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Alternatively, could tell the system to not use mipmaps with the call below.
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Reset Graphics state.
	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0, 0, width, height);

	*/

	
	DrawableObject * object = new DrawableObject("..\\Media\\camel_decim.obj");
	ModelManager::Instance()->RegisterModel("Object", object);
	DrawableProxy* objectNode = new DrawableProxy("Object", "Object");
	CreateLights();
	CreateGoldMaterial();
	CreateTexturedMaterial();
	MaterialProxy* materialNode = new MaterialProxy("Object Material", "step3_Textured", objectNode);
	TransformMatrixNodeSolution* objectTransform = new TransformMatrixNodeSolution("ObjectSpace", materialNode);
	objectTransform->Scale(3, 3, 3);
	objectTransform->Translate(0, 1.0, 0);
	GroupNode* group = new GroupNode("Pedestal");
	paint = new PaintController("Painter", group);
	group->AddChild(objectTransform);
	examiner4 = new ExaminerCameraNode("Examiner", group);
	examiner4->setWidth(windowWidth);
	examiner4->setHeight(windowHeight);
	return examiner4;
	
	// 11/15

}


//////////////////////////////////////////////////////////////////
void DisplayFrame()
{
	//rootSceneNode->Accept(renderVisitor);
	glEnable(GL_SCISSOR_TEST);
	//examiner->setWidth(windowHeight - 20);
	//examiner->setHeight(windowHeight - 20);

	// left main scene
	glViewport(0, 0, 600, 600);
	glScissor(0, 0, 600, 600);
	rootSceneNode->Accept(renderVisitor);
	

	int viewportSize = 180;
	int sideViewSize = 10;
	// right up scene
	glViewport(windowWidth - viewportSize - sideViewSize + 10, windowHeight - 1 * viewportSize - 1 * sideViewSize, viewportSize, viewportSize);
	glScissor(windowWidth - viewportSize - sideViewSize + 10, windowHeight - 1 * viewportSize - 1 * sideViewSize, viewportSize, viewportSize);
	rootSceneNode2->Accept(renderVisitor);

	// right medium scene
	glViewport(windowWidth - viewportSize - sideViewSize + 10, windowHeight - 2 * viewportSize - 2 * sideViewSize, viewportSize, viewportSize);
	glScissor(windowWidth - viewportSize - sideViewSize + 10, windowHeight - 2 * viewportSize - 2 * sideViewSize, viewportSize, viewportSize);
	rootSceneNode3->Accept(renderVisitor);
	// right down scene
	glViewport(windowWidth - viewportSize - sideViewSize + 10, windowHeight - 3 * viewportSize - 3 * sideViewSize, viewportSize, viewportSize);
	glScissor(windowWidth - viewportSize - sideViewSize + 10, windowHeight - 3 * viewportSize - 3 * sideViewSize, viewportSize, viewportSize);
	rootSceneNode4->Accept(renderVisitor);


}

void ReshapeWindow(int newWidth, int newHeight)
{
	windowWidth = newWidth;
	windowHeight = newHeight;
	examiner->setWidth(windowWidth);
	examiner->setHeight(windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glutPostRedisplay();
}

ISceneNode* CreateFrameBuffer(Crawfis::Graphics::ISceneNode * scene)
{
	IRenderTarget* screen = new RenderTarget();
	RenderManager::Instance()->RegisterRenderTarget("Screen", screen);
	screen->setEnableCommand(new ClearFrameCommand(Colors::Scarlet));
	screen->setDisableCommand(new SwapCommand(true));
	RenderTargetProxy* frameBuffer = new RenderTargetProxy("Screen Display", "Screen", scene);
	return frameBuffer;
}


void KeyboardController(unsigned char key, int x, int y)
{
	printf("Key Pressed: %c\n", key);
	std::vector<IKeyboardHandler*>::iterator handlerIterator;
	for (handlerIterator = keyboardHandlers.begin(); handlerIterator != keyboardHandlers.end(); handlerIterator++)
	{
		(*handlerIterator)->KeyPress(key, x, y);
	}
	glutPostRedisplay();
}

void NumPadController(int key, int x, int y)
{
	std::vector<IKeyboardHandler*>::iterator handlerIterator;
	for (handlerIterator = keyboardHandlers.begin(); handlerIterator != keyboardHandlers.end(); handlerIterator++)
	{
		(*handlerIterator)->NumPadPress(key, x, y);
	}
	glutPostRedisplay();
}

void MousePressController(int button, int state, int ix, int iy)
{
	std::vector<IMouseHandler*>::iterator handlerIterator;
	for (handlerIterator = mouseHandlers.begin(); handlerIterator != mouseHandlers.end(); handlerIterator++)
	{
		(*handlerIterator)->MouseEvent(button, state, ix, iy);
	}
	glutPostRedisplay();
}

void MouseMotionController(int ix, int iy)
{
	std::vector<IMouseHandler*>::iterator handlerIterator;
	for (handlerIterator = mouseHandlers.begin(); handlerIterator != mouseHandlers.end(); handlerIterator++)
	{
		(*handlerIterator)->MouseMoved(ix, iy);
	}
	glutPostRedisplay();
}

void IdleCallback()
{
}
void InitializeDevIL()
{
	::ilInit();
	::iluInit();
	::ilutInit();
	//::ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	//::ilEnable(IL_ORIGIN_SET);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	InitializeDevices();
	InitializeDevIL();
	TextureBindManagerOpenGL::Init();

	MatrixStack::Init();
	// 11/13
	ISceneNode* scene = CreateSceneGraph();
	rootSceneNode = CreateFrameBuffer(scene);
///////////////////////////////////////////////////////

	// 11/13
	ISceneNode * scene2 = CreateSceneGraph2();
	rootSceneNode2 = CreateFrameBuffer(scene2);

/////////////////////////////////////////////////////////////
	ISceneNode * scene3 = CreateSceneGraph3();
	rootSceneNode3 = CreateFrameBuffer(scene3);
///////////////////////////////////////////////////
	// 11/16
	ISceneNode * scene4 = CreateSceneGraph4();
	rootSceneNode4 = CreateFrameBuffer(scene4);

	//ISceneNode * scene4 = CreateSceneGraph4();
	//rootSceneNode4 = CreateFrameBuffer(scene4);

//////////////////////////////////////////////////
	renderVisitor = new DebugRenderVisitor();
	PrintSceneVisitor* printScene = new PrintSceneVisitor();
	rootSceneNode->Accept(printScene);
	rootSceneNode2->Accept(printScene);
	rootSceneNode3->Accept(printScene);
	rootSceneNode4->Accept(printScene);

	keyboardHandlers.push_back(paint);
	mouseHandlers.push_back(paint);
	keyboardHandlers.push_back(examiner);
	mouseHandlers.push_back(examiner);


///////////////////////////////////////////////////////////////////
	// 11/13
	keyboardHandlers.push_back(examiner3);
	mouseHandlers.push_back(examiner3);
	keyboardHandlers.push_back(examiner4);
	mouseHandlers.push_back(examiner4);
	







	glutDisplayFunc(DisplayFrame);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(KeyboardController);
	glutSpecialFunc(NumPadController);
	glutMouseFunc(MousePressController);
	glutMotionFunc(MouseMotionController);

	glutMainLoop();

	return 0;
}