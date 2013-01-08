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
#include "GUI.h"

using namespace std;
using namespace glm;

Intersection inters;


int main(int argc, char **argv)
{
	int width,height;
	Camera cam;
	int mousedx=0;
	int mousedy=0;
	int test=0;

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
	GUI gui;
	//sets up the terrain
	Terrain terrain(0);
	terrain.setRadius(gui.getSliderRadius());
	terrain.setOpacity(gui.getSliderOpacity());
	
	//sends texture handels etc to the renderer
	rend.setTerrainInfo(terrain.getTerrInfo());
	//also the gui needs the textures for browsing
	gui.setTerrainInfo(terrain.getTerrInfo());
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
			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::E))
			{
				gui.decActiveTex();
				terrain.setActiveTex(gui.getActiveTex());
			}
			
			if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::R))
			{
				gui.incActiveTex();
				terrain.setActiveTex(gui.getActiveTex());
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
					cam.zoomIn(event.MouseWheel.Delta*0.5);
				else
					cam.zoomOut(-event.MouseWheel.Delta*0.5);
				rend.updateViewMatrix(cam.getViewMatrix());
			}
			
			if(event.Type == sf::Event::MouseButtonPressed)
			{
				if(event.MouseButton.Button==sf::Mouse::Right)
				{
					gui.showMenu(true);
					float normalisedx = 2 * (float)app.GetInput().GetMouseX() / width - 1;
					float normalisedy = 1 - 2 * (float)app.GetInput().GetMouseY() / height;
					gui.setRightClickXY(normalisedx,normalisedy);
				}
			}
			
			if(event.Type == sf::Event::MouseButtonPressed)
			{
				if(event.MouseButton.Button==sf::Mouse::Left)
				{
					float normalisedx = 2 * (float)app.GetInput().GetMouseX() / width - 1;
					float normalisedy = 1 - 2 * (float)app.GetInput().GetMouseY() / height;
					gui.setLeftClick(normalisedx,normalisedy);
					terrain.setActiveTex(gui.getActiveTex());
				}
			}

			if(event.Type == sf::Event::MouseButtonReleased)
			{
				if(event.MouseButton.Button==sf::Mouse::Right)
				{
					gui.showMenu(false);
				}
			}
		}
		
		//realtime input
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Left))
		{
			float normalisedx = 2 * (float)app.GetInput().GetMouseX() / width - 1;
			float normalisedy = 1 - 2 * (float)app.GetInput().GetMouseY() / height;
			//cout << normalisedx <<" " << normalisedy<<endl;
			if(gui.isInDrawWindow(normalisedx,normalisedy))
			{
				if(gui.getState()==GUIstate::PAINT)
					terrain.paint(cam.getPos(),inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos()));
			}
			else
			{
				gui.moveSliders(normalisedx,normalisedy);
				{
					terrain.setRadius(gui.getSliderRadius());
					terrain.setOpacity(gui.getSliderOpacity());
					terrain.setDropoff(gui.getSliderDropoff());
				}
			}
		}
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Right))
		{
				float normalisedx = 2 * (float)app.GetInput().GetMouseX() / width - 1;
				float normalisedy = 1 - 2 * (float)app.GetInput().GetMouseY() / height;
				gui.setMouseXY(normalisedx,normalisedy);
				//cout << normalisedx <<" " << normalisedy<<endl;
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
		
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Middle))
		{
			cam.rotateLeft(mousedx-app.GetInput().GetMouseX());
			cam.rotateUp(mousedy-app.GetInput().GetMouseY());
			rend.updateViewMatrix(cam.getViewMatrix());
		}
		
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Right))
		{
			gui.showMenu(true);
		}
		//saves the position of the mouse, used for rotation
		mousedx=app.GetInput().GetMouseX();
		mousedy=app.GetInput().GetMouseY();

		glClearColor(0.75f, 0.87f, 0.85f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rend.draw();
		gui.draw();

		app.Display();
	}

	return EXIT_SUCCESS;
}