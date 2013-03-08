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
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace glm;

Intersection inters;
const string path="maps/";
int bindCounter=1;
	

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

void load(string filename, Terrain &terr, Renderer &r, PathHandler& p, LightHandler &l, ParticleHandler &part,MeshHandler &m)
{
	ifstream stream;
	string fullPath = path+filename+".txt";
	stream.open(fullPath.c_str());
	if(stream.is_open())
	{
		r.clear();
		l.clear();
		p.clear();
		part.clear();
		int mapsize=0;
		
		int highBind=0;
		string bmp1="";
		string bmp2="";
		int width=0;
		while(!stream.eof())
		{
			char buf[1024];
			char key[1024];
			stream.getline(buf, 1024);
			sscanf(buf, "%s", key);

			if(strcmp(key, "bmp1:") == 0)
			{
				char file[100];
				sscanf(buf, "bmp1: %s", &file);
				bmp1= string(file);
			}
			else if(strcmp(key, "bmp2:") == 0)
			{
				char file[100];
				sscanf(buf, "bmp2: %s", &file);
				bmp2= string(file);
				
				if(width == 128)
					mapsize=1;
				if(width==256)
					mapsize=2;
				terr.createNewMap(mapsize);
				terr.loadMaps(path+bmp1,path+bmp2,path+filename+"gm.png");
			}
			else if(strcmp(key, "width:") == 0)
			{
				sscanf(buf, "width: %d", &width);
			}		
			else if(strcmp(key, "Surfaceplanes:") == 0)
			{
				string texture;
				int surfCounter=-1;
				bool done = false;
				while(!done)
				{
					stream.getline(buf, 1024);
					sscanf(buf, "%s", key);
					
					if(strcmp(key, "SF:") == 0)
					{
						char in[100];
						sscanf(buf, "SF: %s", &in);
						texture = string(in);
						surfCounter++;
					}
					else if(strcmp(key, "end") == 0)
					{
						done = true;
					}
					else // Else its an actual road piece (at least we hope so because else we are screwed)
					{
						float rot,scale;
						float x, z;
						sscanf(buf, "%f %f %f %f", &rot, &x, &z,&scale);
						terr.addSurface(vec3(x,0,z),rot,surfCounter,scale);
						

						//m_roads.push_back(g_graphicsEngine->createRoad(texture, FLOAT3(x, 0.0f, -z), rot));
					}
				}
			}
			else if(strcmp(key, "MODELS:") == 0)
			{
				string s;
				bool done = false;
				while(!done)
				{
					stream.getline(buf, 1024);
					sscanf(buf, "%s", key);
					
					if(strcmp(key, "end") == 0)
					{
						done = true;
					}
					else
					{
						char in[100];
						vec3 position;
						vec3 rotation;
						float scale;
						int id = 0;
						sscanf(buf, "%s %f %f %f %f %f %f %f %f", &in, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &scale,&id);
						Model model;
						string modelName = string(in);
						int meshIndex = m.getMeshByName(modelName);
						model.setMesh(m.getMeshInfo(meshIndex));
						model.setBoundingBox(m.getBoundingBox(meshIndex));
						model.setMeshName(m.getMeshName(meshIndex));
						model.setPos(position);
						model.rotateX(rotation.x);
						model.rotateY(rotation.y);
						model.rotateZ(rotation.z);
						model.scaleXYZ(scale);
						model.bindId(id);
						r.addModel(model);
						if(id>bindCounter)
							bindCounter=id;
					}
				}
			}
			else if(strcmp(key, "LIGHTS:") == 0)
			{
				string s;
				bool done = false;
				while(!done)
				{
					stream.getline(buf, 1024);
					sscanf(buf, "%s", key);
					
					if(strcmp(key, "end") == 0)
					{
						done = true;
					}
					else
					{
						char in[100];
						sscanf(buf, "%s", &in);

						if(strcmp(key, "AM") == 0)
						{
							vec3 direction;
							vec3 color;
							vec3 rot;
							vec3 pos;

							sscanf(buf, "AM %f %f %f %f %f %f %f %f %f %f %f %f", &direction.x, &direction.y, &direction.z, &color.x, &color.y, &color.z,&rot.x,&rot.y,&rot.z,&pos.x,&pos.y,&pos.z);
							
							Light tmpLight;
							tmpLight.setColor(color);
							tmpLight.setContrast(1.0f);
							tmpLight.setPos(pos);
							tmpLight.setRadius(0);
							tmpLight.setLightType(LightType::AMBIENT);
							tmpLight.rotateX(rot.x);
							tmpLight.rotateY(rot.y);
							tmpLight.rotateZ(rot.z);
							l.addLight(tmpLight);
						}
						else if(strcmp(key, "PLS") == 0)
						{
							vec3 position;
							vec3 rotation;
							vec3 color;
							float radius;
							int id=0;
							sscanf(buf, "PLS %f %f %f %f %f %f %f %f %f %f %f", &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &color.x, &color.y, &color.z, &radius,&id);
							
							Light tmpLight;
							tmpLight.setColor(color);
							tmpLight.setContrast(1.0f);
							tmpLight.setPos(position);
							tmpLight.setRadius(radius);
							tmpLight.setLightType(LightType::POINTLIGHTSHADOW);
							tmpLight.rotateX(rotation.x);
							tmpLight.rotateY(rotation.y);
							tmpLight.rotateZ(rotation.z);
							tmpLight.bindId(id);
							l.addLight(tmpLight);

						}
						else if(strcmp(key, "PL") == 0)
						{
							vec3 position;
							vec3 rotation;
							vec3 color;
							float radius;
							int id=0;
							sscanf(buf, "PL %f %f %f %f %f %f %f %f %f %f %f", &position.x, &position.y, &position.z, &rotation.y, &rotation.x, &rotation.z, &color.x, &color.y, &color.z, &radius,&id);
							
							Light tmpLight;
							tmpLight.setColor(color);
							tmpLight.setContrast(1.0f);
							tmpLight.setPos(position);
							tmpLight.setRadius(radius);
							tmpLight.setLightType(LightType::POINTLIGHT);
							tmpLight.rotateX(rotation.x);
							tmpLight.rotateY(rotation.y);
							tmpLight.rotateZ(rotation.z);
							tmpLight.bindId(id);
							l.addLight(tmpLight);
						}
						else if(strcmp(key, "SL") == 0)
						{
							vec3 position;
							vec3 direction;
							vec3 color;
							vec3 rot;
							sscanf(buf, "SL %f %f %f %f %f %f %f %f %f %f %f %f", &position.x, &position.y, &position.z, &direction.x, &direction.y, &direction.z, &color.x, &color.y, &color.z,&rot.x, &rot.y, &rot.z);
							
							Light tmpLight;
							tmpLight.setColor(color);
							tmpLight.setContrast(1.0f);
							tmpLight.setPos(position);
							tmpLight.setRadius(0);
							tmpLight.setLightType(LightType::SPOTLIGHT);
							tmpLight.rotateX(rot.x);
							tmpLight.rotateY(rot.y);
							tmpLight.rotateZ(rot.z);
							l.addLight(tmpLight);
						}
					}
				}
			}
			else if(strcmp(key, "path") == 0)
			{
				p.addPath();
				stream.getline(buf, 1024);
				sscanf(buf, "%s", key);

				int nrOfPoints = 0;
				vec3 points[100];
				while(strcmp(key, "end") != 0)
				{
					float notInvertZ;
					points[nrOfPoints]=vec3(0.0f);
					sscanf(buf, "%f %f", &points[nrOfPoints].x, &points[nrOfPoints].z);
					nrOfPoints++;
					stream.getline(buf, 1024);
					sscanf(buf, "%s", key);
				}

				for(int i = 0; i < nrOfPoints; i++)
				{
					p.addFlagToCurrentPath(points[i]);
				}

			}
			else if(strcmp(key, "PARTICLESYSTEMS:") == 0)
			{
				string s;
				bool done = false;
				while(!done)
				{
					stream.getline(buf, 1024);
					sscanf(buf, "%s", key);
					
					if(strcmp(key, "end") == 0)
					{
						done = true;
					}
					else
					{
						vec3 position;
						vec3 rotation;
						vec3 color;

						sscanf(buf, "%s %f %f %f %f %f %f %f %f %f", &key, &position.x, &position.y, &position.z, &rotation.x, &rotation.y, &rotation.z, &color.x, &color.y, &color.z);
						
						Particle particle;
						particle.setPos(position);
						particle.rotateX(rotation.x);
						particle.rotateY(rotation.y);
						particle.rotateZ(rotation.z);
						particle.setColor(color);
						string particleType=key;
						if(particleType=="GLOWRING")
							particle.setParticleType(ParticleType::GLOWRING);
						if(particleType=="FIRE")
							particle.setParticleType(ParticleType::FIRE);
						if(particleType=="EMIT")
							particle.setParticleType(ParticleType::EMIT);
						if(particleType=="FLOW")
							particle.setParticleType(ParticleType::FLOW);
						if(particleType=="SMOKE")
							particle.setParticleType(ParticleType::SMOKE);
						part.addParticleModel(particle);

						//Create particle system
						int lol = 0;
					}
				}
			}
			sscanf("bugfix", "%s", key);
		}	
		bindCounter++;
		stream.close();
	}
}


int main(int argc, char **argv)
{
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
				{
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
									rend.selectModel(normalisedx,normalisedy,cam.getPos());

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
							load(gui.getInputText(),terrain,rend,ph,lh,particleHandler,mh);
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
						particleHandler.assignParticleNewParticle(gui.getActiveParticleModel());
					}
					
					if(gui.getState()==GUIstate::LIGHT)
						lh.assignLightAnotherLight(gui.getActiveLight());
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
			if(app.GetInput().IsKeyDown(sf::Key::Delete))
			{
				if(gui.getState()==GUIstate::MODEL||gui.getState()==GUIstate::NONE)
				{
					vector<Model> rm = rend.removeSelectedModels();
					lh.removeLightsBoundToModels(rm);
					vector<Model> tm =rend.getModels();
					terrain.recalcGridAroundModel(rm,tm);
				}
				if(gui.getState()==GUIstate::PATH)
				{
					ph.removeSelectedPaths();
				}
				if(gui.getState()==GUIstate::PARTICLE)
				{
					particleHandler.removeSelectedParticles();
				}
				if(gui.getState()==GUIstate::ROAD)
				{
					terrain.removeSelectedSurfaces();
				}
				if(gui.getState()==GUIstate::LIGHT)
				{
					lh.removeSelectedLights();
				}
				
			}
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
