#include <iostream>
#define GLEW_STATIC
#include <glew.h>
#include <SFML/Window.hpp>
#include <MeshHandler.h>
#include "glm/glm.hpp"
#include "Model.h"
#include "Renderer.h"
#include "Terrain.h"

using namespace std;
using namespace glm;

int main(int argc, char **argv)
{
	int width,height;
	mat4 projectionMatrix;

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

	//Start renderer after glewinit,GLSPprog needs it (could add init method for global renderer)
	Renderer rend;
	Terrain terrain(1.0f,1.0f,1);
	rend.setTerrainInfo(terrain.getTerrInfo());
	rend.updateProjMatrix(width,height);
	rend.updateCamera(vec3(1.0f,1.0f,2.0f));
	glViewport(0,0,width,height);

	MeshHandler mh("./models/");
	Model m;
	
	m.setMesh(mh.getMeshInfo(0));
	rend.addModel(&m);
	m.rotateX(-90);
	
	m.setPos(vec3(0.8f,0.0f,0.0f));
	m.scaleXYZ(0.02);
	 
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
		
		//realtime input
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Left))
		{
			terrain.paint(0,0,0,vec3(1.0f,1.0f,2.0f),rend.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY()));
		}
		glClearColor(0.75f, 0.87f, 0.85f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rend.draw();
		//terrain.paint(0,0,0,0,0);
		app.Display();
	}

	return EXIT_SUCCESS;
}