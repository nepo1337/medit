#include <iostream>
#define GLEW_STATIC
#include <glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <MeshHandler.h>
#include "glm/glm.hpp"
#include "Model.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Camera.h"
#include "Intersection.h"

using namespace std;
using namespace glm;

Intersection inters;

int main(int argc, char **argv)
{
	int width,height;
	Camera cam;

	//window options
	width=1280;
	height=720;
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
	Terrain terrain(75.0f,75.0f);
	rend.setTerrainInfo(terrain.getTerrInfo());
	rend.updateProjMatrix(width,height);
	rend.updateViewMatrix(cam.getViewMatrix());
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
			if(event.Type == sf::Event::MouseWheelMoved)
			{
				if(event.MouseWheel.Delta>0)
					cam.zoomIn(0.4);
				else
					cam.zoomOut(0.4);
				rend.updateViewMatrix(cam.getViewMatrix());
			}
			
		}
		
		//realtime input
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Left))
		{
			terrain.paint(0,0,0,cam.getPos(),inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos()));
		}
		if(app.GetInput().IsKeyDown(sf::Key::W))
		{
			cam.moveForeward(0.1);
			rend.updateViewMatrix(cam.getViewMatrix());
		}
		if(app.GetInput().IsKeyDown(sf::Key::S))
		{
			cam.moveBackward(0.1);
			rend.updateViewMatrix(cam.getViewMatrix());
		}
		if(app.GetInput().IsKeyDown(sf::Key::A))
		{
			cam.strafeLeft(0.1);
			rend.updateViewMatrix(cam.getViewMatrix());
		}
		if(app.GetInput().IsKeyDown(sf::Key::D))
		{
			cam.strafeRight(0.1);
			rend.updateViewMatrix(cam.getViewMatrix());
		}
		glClearColor(0.75f, 0.87f, 0.85f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rend.draw();
		//terrain.paint(0,0,0,0,0);
		app.Display();
	}

	return EXIT_SUCCESS;
}