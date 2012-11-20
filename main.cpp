#include <iostream>
#include <glew.h>
#include <SFML/Window.hpp>
#include <MeshHandler.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <Model.h>

using namespace std;
using namespace glm;

int width,height;
mat4 projectionMatrix;
//s
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
	
	glViewport(0,0,width,height);
	float nearClip = 0.5f;
	float farClip  = 100.0f;
	float fov_deg = 45.0f;
	float aspect = (float)width/(float)height;
	projectionMatrix = perspective(fov_deg, aspect,nearClip,farClip);
	
	MeshHandler mh("./models/");
	Model m;
	m.setMesh(mh.getMeshInfo(0));
	
	sf::Event event;

	while (app.IsOpened())
	{
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
			}
		}
		glClearColor(0.75f, 0.87f, 0.85f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app.Display();
	}

	return EXIT_SUCCESS;
}
