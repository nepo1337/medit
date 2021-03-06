#ifndef GUI_H
#define GUI_H
#define GLEW_STATIC
#include "Spritetext.h"
#include <glew.h>
#include "TA.h"
#include "GLSLProgram.h"
#include <iostream>
#include "Sprite.h"
#include "Slider.h"
#include "SOIL/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "Model.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Light.h"
#include "Particle.h"

class Renderer;

using namespace std;
using namespace glm;

namespace GUIstate
{
  enum GUIstates
  {
    NONE,PAINT,MODEL,LIGHT,PATH,PARTICLE,QUEST,ROAD
  };
};

class GUI
{
private:
	GUIstate::GUIstates state;
	Spritetext text;
	GLuint k;
	Sprite frontPanel,backPanel,drawPanel,modelPanel,lightPanel,splashScreen,
	questPanel,particlePanel,pathPanel,menuOff,menuDraw,menuLight,
	menuModel,menuParticle,menuPath,menuQuest,newMap,loadMap,saveMap,menuRoad,roadPanel,colorPickerSprite,colorPlaneSprite, spriteAmbient, spritePointLight,spritePointLightShadow,spriteSpotLight;
	//sprites for browsing the textures in paint mode, stp = smalltextureplane
	Sprite mainTex,stp1,stp2,stp3,stp4;
	vector<GLuint> surfaceTexHandles;
	vector<Sprite> particleImagesHandles;
	vector<Particle> particles;
	
	//sprites/data drag thiny
	Sprite dragArrow;
	Slider sliderOpacity,sliderSize,sliderDropoff,sliderScale,sliderColorPicker,sliderContrast,sliderRadius,sliderHeight,sliderModelHeight,sliderRoadSpacing,sliderRoadScale;
	
	GLSLProgram GUIshader,modelDisplayShader,colorPlaneShader;
	//textur handles to paintable textures
	GLuint texHandels[8];
	
	int activeParticleIndex;
	int activeTex;
	bool menuUp;
	float rightClickX, rightClickY;
	bool inCircle(float cx, float cy, float x, float y,float rad);
	bool showNewMapSprite,showLoadMapSprite,showSaveMapSprite;
	string ans;
	bool textMode;
	vector<Model> displayModels;
	int activeModelIndex;
	void incActiveTex();
	void decActiveTex();
	Model activeModel;
	bool selectRoad;
	sf::Image colorScale;
	vec3 normalizedColor;
	Light activeLight;
	bool isPlacingLights;
	int activeSurfaceTex;
	Particle activeParticle;
	bool placingParticleSystems;
public:
	GUI();
	~GUI();
	void init();
	void draw();
	void setTerrainInfo(TerrainInfo *t);
	int getActiveTex();
	int getActiveSurfaceTexHandle();
	void setGuiState(GUIstate::GUIstates state);
	void showMenu(bool ans);
	//maybe there should be some inputclass so the gui would be realeased from checking coordinates
	void setRightClickXY(float x, float y);
	void setMouseXY(float x, float y);
	void setLeftClick(float x, float y);
	GUIstate::GUIstates getState();
	bool isInDrawWindow(float x, float y);
	float getSliderRadius();
	float getSliderOpacity();
	float getSliderDropoff();
	void moveSliders(float x, float y);
	string checkDialogAnswer();
	bool addChar(char c);
	bool removeChar();
	bool isInTextMode();
	string getInputText();
	void hideNewMapDialog();
	void hideSaveMapDialog();
	void hideLoadMapDialog();
	bool isSaveMapDialogUp();
	bool isLoadMapDialogUp();
	bool isNewMapDialogUp();
	void addDisplayModel(Model m);
	void setSurfaceTexHandles(vector<GLuint> th);
	void resetDialogAns();
	Model getActiveModel();
	void rotateActiveModelLeft(float f);
	void rotateActiveModelRight(float f);
	void raiseActiveModel(float f);
	void lowerActiveModel(float f);
	bool isSelectingRoad();
	vec3 getNormalizedColor();
	float getContrast();
	void setContrast(float con);
	float getSliderLightRadius();
	void setSliderLightRadius(float s);
	void setNormalizedColor(vec3 col,float contrast);
	Light getActiveLight();
	void setActiveLightModel(Light l);
	bool isPlacingLightMode();
	void drawSplashScreen();
	float getRoadSliderSpacing();
	float getRoadSliderScale();
	Particle getActiveParticleModel();
	bool isPlacingParticleSystems();
	void setActiveParticleModel(Particle p);
};

#endif // GUI_H
