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
#include <fstream>
#include "PathHandler.h"
#include "LightHandler.h"
#include "Light.h"
#include "ParticleHandler.h"
#include "Particle.h"

using namespace std;
using namespace glm;

Intersection inters;
const string path="maps/";

void getNormalizedXY(int mouseX, int mouseY, int width, int height, float &x, float &y)
{
	x = 2 * (float)mouseX / width - 1;
	y = 1 - 2 * (float)mouseY / height;
}

void save(string filename, Terrain& terr, Renderer &r,PathHandler& p,LightHandler &l, ParticleHandler &part)
{
	terr.save(path,filename);
	r.saveModels(path,filename);
	p.save(path,filename);
	l.save(path,filename);
	part.save(path,filename);
}


int main(int argc, char **argv)
{
	int bindCounter=1;
	int width,height;
	Camera cam;
	int mousedx=0;
	int mousedy=0;

	//window options
	width=1280;
	height=720;
	sf::WindowSettings settings;
	settings.DepthBits         = 24;
	settings.StencilBits       = 8;
	settings.AntialiasingLevel = 1;  
	sf::Window app;
	app.Create(sf::VideoMode(width, height, 32), "Saints Edit", sf::Style::Close|sf::Style::Resize, settings);
	app.UseVerticalSync(true);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout<<"ERROR starting GLEW: "<< glewGetErrorString(err);
	}

	//Start renderer after glewinit,GLSPprog needs it (could add init method for global renderer)
	Renderer rend;
	GUI gui;

	gui.init();
	gui.drawSplashScreen();
	app.Display();
	
	//sets up the terrain
	Terrain terrain(0);
	PathHandler ph;
	LightHandler lh;
	ParticleHandler particleHandler;
	particleHandler.init();
	lh.init();
	ph.init();
	terrain.setRadius(gui.getSliderRadius());
	terrain.setOpacity(gui.getSliderOpacity());
	gui.setSurfaceTexHandles(terrain.getSurfaceTexHandles());

	//the gui needs the textures for browsing
	gui.setTerrainInfo(terrain.getTerrInfo());
	rend.updateProjMatrix(width,height);
	rend.updateViewMatrix(cam.getViewMatrix());
	terrain.updateProjMatrix(width,height);
	terrain.updateViewMatrix(cam.getViewMatrix());
	ph.updateProjectionMatrix(width,height);
	ph.updateViewMatrix(cam.getViewMatrix());
	glViewport(0,0,width,height);

	MeshHandler mh("./models/");

	for(int i=0; i<mh.getNrOfMeshes(); i++)
	{
		Model tmp;
		tmp.setMesh(mh.getMeshInfo(i));
		tmp.setBoundingBox(mh.getBoundingBox(i));
		tmp.setMeshName(mh.getMeshName(i));
		tmp.setType(mh.getMeshType(i));
		gui.addDisplayModel(tmp);
	}


	sf::Event event;

	Model m;

	while (app.IsOpened())
	{
		float normalisedx = 0;
		float normalisedy = 0;

		getNormalizedXY(app.GetInput().GetMouseX(), app.GetInput().GetMouseY(),width,height,normalisedx, normalisedy);
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
			if(event.Type==sf::Event::MouseMoved)
			{
				if(gui.getState()==GUIstate::PAINT)
				{
					vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
					terrain.setWorldXY(cam.getPos(),ray);
				}
			}


			if(event.Type == sf::Event::Resized)
			{
				height = app.GetHeight();
				width = app.GetWidth();
				glViewport(0,0,width,height);
				rend.updateProjMatrix(width,height);
				terrain.updateProjMatrix(width,height);
				ph.updateProjectionMatrix(width,height);
			}
			if(event.Type == sf::Event::MouseWheelMoved)
			{
				if(event.MouseWheel.Delta>0)
					cam.zoomIn(event.MouseWheel.Delta*2);
				else
					cam.zoomOut(-event.MouseWheel.Delta*2);
				rend.updateViewMatrix(cam.getViewMatrix());
				terrain.updateViewMatrix(cam.getViewMatrix());
				ph.updateViewMatrix(cam.getViewMatrix());
			}

			if(event.Type == sf::Event::MouseButtonPressed)
			{
				if(event.MouseButton.Button==sf::Mouse::Right)
				{
					gui.showMenu(true);
					gui.setRightClickXY(normalisedx,normalisedy);
					particleHandler.unselectAllParticleModels();
				}
			}

			if(event.Type == sf::Event::MouseButtonPressed)
			{
				if(event.MouseButton.Button==sf::Mouse::Left)
				{cout<<normalisedx<< " " << normalisedy << endl;
					gui.setLeftClick(normalisedx,normalisedy);
					terrain.setActiveTex(gui.getActiveTex());
					

					if(gui.checkDialogAnswer()=="GRID")
					{
						terrain.showHideGridMap();
						gui.resetDialogAns();
					}

					if(!gui.isSaveMapDialogUp()&&!gui.isLoadMapDialogUp()&&!gui.isNewMapDialogUp())
					{
						
						if(gui.getState()==GUIstate::PARTICLE)
						{
							if(gui.isInDrawWindow(normalisedx,normalisedy))
							{
								vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
								float x=-1;
								float z=1;
								terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
								if(gui.isPlacingParticleSystems())
								{
									if(x!=-1)
									{
										particleHandler.unselectAllParticleModels();
										Particle p;
										p=gui.getActiveParticleModel();
										p.setPos(vec3(x,gui.getActiveParticleModel().getPos().y,-z));
										particleHandler.addParticleModel(p);
										gui.setActiveParticleModel(particleHandler.getSelectedParticle());
									}
								}
								else
								{
									particleHandler.selectParticles(normalisedx,normalisedy,cam.getPos(),rend.getProjMatrix(),cam.getViewMatrix());
									gui.setActiveParticleModel(particleHandler.getSelectedParticle());
								}
							}
						}
						if(gui.getState()==GUIstate::ROAD)
						{
							if(gui.checkDialogAnswer()=="RS")
							{
								terrain.removeSelectedSurfaces();
								gui.resetDialogAns();
							}
						}
						if(gui.getState()==GUIstate::NONE)
						{
							if(gui.checkDialogAnswer()=="DEL") 
							{
								vector<Model> rm = rend.removeSelectedModels();
								lh.removeLightsBoundToModels(rm);
								vector<Model> tm =rend.getModels();
								terrain.recalcGridAroundModel(rm,tm);

								terrain.removeSelectedSurfaces();
								gui.resetDialogAns();
							}
						}
						if(gui.getState()==GUIstate::PARTICLE)
						{
							if(gui.checkDialogAnswer()=="DEL") 
							{
								particleHandler.removeSelectedParticles();
								gui.resetDialogAns();
							}
						}

						if(gui.getState()==GUIstate::PATH)
						{
							if(gui.checkDialogAnswer()=="DEL")
							{
								ph.removeSelectedPaths();
								gui.resetDialogAns();
							}
							if(gui.checkDialogAnswer()=="CRP")
							{
								ph.addPath();
								gui.resetDialogAns();
							}
							if(gui.isInDrawWindow(normalisedx,normalisedy))
							{
								if(gui.isSelectingRoad())
								{
									vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
									float x=-1;
									float z=1;
									terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
									if(x!=-1)
									{
										ph.addFlagToCurrentPath(vec3(x,0,-z));
									}
								}
								else
								{
									ph.selectPaths(normalisedx,normalisedy,cam.getPos());
								}
							}
						}
						if(gui.getState()==GUIstate::MODEL||gui.getState()==GUIstate::NONE)
						{
							if(gui.getState()==GUIstate::MODEL)
							{
								if(gui.isInDrawWindow(normalisedx,normalisedy))
								{
									vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
									float x=-1;
									float z=1;
									terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
									if(x!=-1)
									{
										if(m.getType()=="light")
										{
											m.bindId(bindCounter);
											vec3 lpos = m.getPos();
											lpos.y+=1;
											Light tmpLight;
											tmpLight.setColor(gui.getNormalizedColor());
											tmpLight.setPos(lpos);
											tmpLight.setRadius(gui.getSliderLightRadius());
											tmpLight.bindId(bindCounter);
											tmpLight.setContrast(gui.getContrast());
											tmpLight.setLightType(LightType::POINTLIGHTSHADOW);
											lh.addLight(tmpLight);
											bindCounter++;
										}
										rend.addModel(m);
										terrain.setWorldXY(cam.getPos(),ray);
										terrain.makeGridUnderModel(m);
									}
								}
							}
							if(gui.getState()==GUIstate::NONE)
							{
								if(gui.isInDrawWindow(normalisedx,normalisedy))
								{
									int index = rend.rayIntersectModelBB(normalisedx,normalisedy,cam.getPos());
									rend.selectModelAtIndex(index);

								}
							}
						}
						if(gui.getState()==GUIstate::ROAD)
						{
							vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
							terrain.setWorldXY(cam.getPos(),ray);
							terrain.selectTexSurfaces(0.5,cam.getPos(),ray);
						}
						
						if(gui.getState()==GUIstate::LIGHT)
						{
							if(gui.isInDrawWindow(normalisedx,normalisedy))
							{
								if(gui.isPlacingLightMode())
								{
									vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
									float x=-1;
									float z=1;
									terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
									Light l = gui.getActiveLight();
									lh.deselectAllLights();
									l.setPos(vec3(x,l.getPos().y,-z));
									lh.addLight(l);
								}
								else
								{
									int lightPos=lh.selectLight(normalisedx,normalisedy,cam.getPos(),rend.getProjMatrix(),cam.getViewMatrix());
									if(lightPos>=0)
									{
										Light tmpl = lh.getSelectedLight();
										gui.setSliderLightRadius(tmpl.getRadius());
										gui.setNormalizedColor(tmpl.getColor(),tmpl.getContrast());
										gui.setActiveLightModel(tmpl);
									}
								}
							}
							if(gui.checkDialogAnswer()=="DEL")
							{
								lh.removeSelectedLights();
							}
						}
					}

					if(gui.isSaveMapDialogUp())
					{
						if(gui.checkDialogAnswer()=="svOK")
						{
							save(gui.getInputText(),terrain,rend,ph,lh,particleHandler);
							gui.hideSaveMapDialog();
						}
						if(gui.checkDialogAnswer()=="svC")
						{
							gui.hideSaveMapDialog();
						}
					}
					if(gui.isNewMapDialogUp())
					{
						if(gui.checkDialogAnswer()=="nmCS")
						{
							terrain.createNewMap(0);
							rend.clear();
							ph.clear();
							lh.clear();
							particleHandler.clear();
							gui.hideNewMapDialog();
						}
						
						if(gui.checkDialogAnswer()=="nmCM")
						{
							terrain.createNewMap(1);
							rend.clear();
							ph.clear();
							lh.clear();
							particleHandler.clear();
							gui.hideNewMapDialog();
						}
						
						if(gui.checkDialogAnswer()=="nmCL")
						{
							terrain.createNewMap(2);
							rend.clear();
							ph.clear();
							lh.clear();
							particleHandler.clear();
							gui.hideNewMapDialog();
						}
						if(gui.checkDialogAnswer()=="nmOK")
						{
							terrain.createNewMap(0);
							rend.clear();
							ph.clear();
							lh.clear();
							particleHandler.clear();
							gui.hideNewMapDialog();
						}
						if(gui.checkDialogAnswer()=="nmC")
						{
							gui.hideNewMapDialog();
						}
					}
					if(gui.isLoadMapDialogUp())
					{
						if(gui.checkDialogAnswer()=="lmOK")
						{
							gui.hideLoadMapDialog();
						}
						if(gui.checkDialogAnswer()=="lmC")
						{
							gui.hideLoadMapDialog();
						}
					}
				}
			}

			if(event.Type == sf::Event::MouseButtonReleased)
			{
				if(event.MouseButton.Button==sf::Mouse::Right)
				{
					gui.showMenu(false);
					rend.unselectAllModels();
				}
			}
			//if the gui excpects text input
			if(gui.isInTextMode())
			{
				if(event.Type == sf::Event::KeyPressed)
				{
					if(int(event.Key.Code)>=97&&event.Key.Code<=122)
						gui.addChar(char(event.Key.Code));
				}
				if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Back))
				{
					gui.removeChar();
				}
			}
		}

		//realtime input
		
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Left))
		{

			if(!gui.isSaveMapDialogUp()&&!gui.isLoadMapDialogUp()&&!gui.isNewMapDialogUp())
			{
				if(gui.isInDrawWindow(normalisedx,normalisedy))
				{
					if(gui.getState()==GUIstate::PAINT)
					{
						terrain.setTerState(TerrState::PAINT);
						terrain.paint(cam.getPos(),inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos()));
					}
					if(gui.getState()==GUIstate::ROAD)
					{
						terrain.setTerState(TerrState::DRAWSURFACE);
						vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
						terrain.addSurface(cam.getPos(),ray, gui.getActiveSurfaceTexHandle());
					}
				} 
				else 
				{
					gui.moveSliders(normalisedx,normalisedy);
					if(gui.getState()==GUIstate::PAINT)
					{
						terrain.setRadius(gui.getSliderRadius());
						terrain.setOpacity(gui.getSliderOpacity());
						terrain.setDropoff(gui.getSliderDropoff());
					}
					if(gui.getState()==GUIstate::ROAD)
					{
						terrain.setRoadSpacing(gui.getRoadSliderSpacing());
						terrain.setRoadScale(gui.getRoadSliderScale());
					}
					if(gui.getState()==GUIstate::PARTICLE)
					{
						particleHandler.assignParticleNewParticle(particleHandler.getSelectedParticleIndex(),gui.getActiveParticleModel());
					}
					
					int lightPos=lh.getSelectedLightIndex();
					if(lightPos>=0)
						lh.assignLightAnotherLight(lightPos,gui.getActiveLight());
				}
			}
		}
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Right))
		{
			gui.setMouseXY(normalisedx,normalisedy);
			terrain.deselectAllSurfaceTex();
			lh.deselectAllLights();
		}


		//if the user isnt in text mode, it should be able to move
		if(!gui.isInTextMode())
		{
			if(gui.getState()==GUIstate::PAINT)
			{
					vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
					terrain.setWorldXY(cam.getPos(),ray);
			}
			if(app.GetInput().IsKeyDown(sf::Key::W))
			{
				cam.moveForeward(0.1);
			}
			if(app.GetInput().IsKeyDown(sf::Key::S))
			{
				cam.moveBackward(0.1);
			}
			if(app.GetInput().IsKeyDown(sf::Key::A))
			{
				cam.strafeLeft(0.1);
			}
			if(app.GetInput().IsKeyDown(sf::Key::D))
			{
				cam.strafeRight(0.1);
			}
			rend.updateViewMatrix(cam.getViewMatrix());
			terrain.updateViewMatrix(cam.getViewMatrix());
			ph.updateViewMatrix(cam.getViewMatrix());
			if(gui.getState()==GUIstate::MODEL)
			{
				if(app.GetInput().IsKeyDown(sf::Key::Q))
					gui.rotateActiveModelLeft(1.0f);

				if(app.GetInput().IsKeyDown(sf::Key::E))
					gui.rotateActiveModelRight(1.0f);
				if(app.GetInput().IsKeyDown(sf::Key::R))
					gui.raiseActiveModel(0.01);
				if(app.GetInput().IsKeyDown(sf::Key::F))
					gui.raiseActiveModel(-0.01);
			}
		}
		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Middle))
		{
			cam.rotateLeft(mousedx-app.GetInput().GetMouseX());
			cam.rotateUp(mousedy-app.GetInput().GetMouseY());
			rend.updateViewMatrix(cam.getViewMatrix());
			terrain.updateViewMatrix(cam.getViewMatrix());
			ph.updateViewMatrix(cam.getViewMatrix());
		}

		if(app.GetInput().IsMouseButtonDown(sf::Mouse::Right))
		{
			gui.showMenu(true);
			if(gui.getState()==GUIstate::PAINT)
			{
				terrain.showCircle();
			}
			else
				terrain.hideCircle();
		}
		//saves the position of the mouse, used for rotation
		mousedx=app.GetInput().GetMouseX();
		mousedy=app.GetInput().GetMouseY();

		glClearColor(0.75f, 0.87f, 0.85f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		terrain.draw();
		rend.draw();

		if(gui.getState()==GUIstate::PATH)
		{
			ph.drawPaths();
			if(gui.isSelectingRoad())
			{
				vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
				float x=-1;
				float z=1;
				terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
				ph.drawFlag(vec3(x,0,-z));
			}
		}
		if(gui.getState()==GUIstate::ROAD)
		{
			vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
			terrain.drawSurface(cam.getPos(),ray, gui.getActiveSurfaceTexHandle(),app.GetInput().IsMouseButtonDown(sf::Mouse::Left));
		}
		if(gui.getState()==GUIstate::LIGHT)
		{
			lh.drawLights(rend.getProjMatrix(),cam.getViewMatrix());
			
			if(gui.isPlacingLightMode())
			{
				vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
				float x=-1;
				float z=1;
				terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
				Light l = gui.getActiveLight();
				l.setPos(vec3(x,l.getPos().y,-z));
				//l.select();
				lh.drawLight(rend.getProjMatrix(),cam.getViewMatrix(),l);
			}
		}
		if(gui.getState()==GUIstate::MODEL &&gui.isInDrawWindow(normalisedx,normalisedy) )
		{
			vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
			float x=-1;
			float z=1;
			terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
			if(x!=-1)
			{
				m=gui.getActiveModel();
				m.setPos(vec3(x,gui.getActiveModel().getPos().y,-z));
				m.scaleXYZ(m.getScale());
				m.rotateY(m.getRot().y);
				rend.drawModel(m);
			}
		}
		if(gui.getState()==GUIstate::PARTICLE)
		{
			particleHandler.drawParticleModels(rend.getProjMatrix(),cam.getViewMatrix());
			
			if(gui.isPlacingParticleSystems())
			{
				vec3 ray = inters.getClickRay(app.GetInput().GetMouseX(),app.GetInput().GetMouseY(),cam.getViewMatrix(),rend.getProjMatrix(),width,height,cam.getPos());
				float x=-1;
				float z=1;
				terrain.rayIntersectTerrain(cam.getPos(), ray, x, z);
				Particle p = gui.getActiveParticleModel();
				p.setPos(vec3(x,gui.getActiveParticleModel().getPos().y,-z));
				particleHandler.drawParticleModel(rend.getProjMatrix(),cam.getViewMatrix(),p);
			}
		}
		gui.draw();

		app.Display();
	}

	lh.free();
	
	return EXIT_SUCCESS;
}
