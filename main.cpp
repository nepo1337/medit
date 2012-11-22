#include <iostream>
#include <glew.h>
#include <SFML/Window.hpp>
#include <MeshHandler.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "Model.h"
#include "Renderer.h"

using namespace std;
using namespace glm;

int width,height;
mat4 projectionMatrix;
float rot=0;
int main(int argc, char **argv)
{
	//window options
	width=800;
	height=600;
	sf::WindowSettings settings;
	settings.DepthBits         = 24; // Request a 24 bits depth buffer
	settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
	settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::Window app;
	app.Create(sf::VideoMode(width, height, 32), "R-EDIT", sf::Style::Close|sf::Style::Resize, settings);
	app.UseVerticalSync(true);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout<<"ERROR starting GLEW: "<< glewGetErrorString(err);
	}
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // set depth-testing function type
	//glEnable(GL_CULL_FACE); // enable culling of back-faces
	//glCullFace(GL_BACK); // enable culling of back-faces
	//glFrontFace(GL_CCW);
	
	//info
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslver = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major,minor;
	glGetIntegerv(GL_MAJOR_VERSION,&major);
	glGetIntegerv(GL_MINOR_VERSION,&minor);
	
	cout <<"vendor: "<<vendor<<endl;
	cout <<"renderer: "<<renderer<<endl;
	cout <<"openGL version: "<<version<<endl;
	cout <<"GLSL version: "<<glslver<<endl;
	cout <<"GL version, maj min: "<<major<<minor<<endl;
	
	//Start renderer after glewinit,GLSPprog needs it (could add init method for global renderer)
	Renderer rend;
	rend.updateProjMatrix(width,height);
	rend.updateCamera(vec3(50.0f,0.0f,0.0f));
	glViewport(0,0,width,height);
	
	MeshHandler mh("./models/");
	Model m;
	Model f;
	m.setMesh(mh.getMeshInfo(0));
	f.setMesh(mh.getMeshInfo(1));
	rend.addModel(&m);
	rend.addModel(&f);
	m.setPos(vec3(0.0f,10.0f,0.0f));
	
	sf::Event event;
	while (app.IsOpened())
	{
		//fps
		float framerate = 1.f / app.GetFrameTime();
		
		//events
		while(app.GetEvent(event))
		{
			if(event.Type==sf::Event::Closed)
			{
				app.Close();
			}
			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
			{
				app.Close();
			}
			if(event.Type == sf::Event::Resized)
			{
				height = app.GetHeight();
				width = app.GetWidth();
				glViewport(0,0,width,height);
				rend.updateProjMatrix(width,height);
			}
		}
		glClearColor(0.75f, 0.87f, 0.85f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rend.draw();
		
		f.rotateX(rot);
		f.rotateY(rot);
		m.rotateZ(rot);
		rot+=0.5;
		app.Display();
	}

	return EXIT_SUCCESS;
}
