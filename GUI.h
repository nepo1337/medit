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


using namespace std;
using namespace glm;

namespace GUIstate
{
  enum GUIstates
  {
    NONE,PAINT,MODEL,LIGHT,PATH,PARTICLE,QUEST
  };
};

class GUI
{
private:
	GUIstate::GUIstates state;
	Spritetext text;
	GLuint k;
	Sprite frontPanel,backPanel,drawPanel,modelPanel,lightPanel,
	questPanel,particlePanel,pathPanel,menuOff,menuDraw,menuLight,
	menuModel,menuParticle,menuPath,menuQuest,newMap,loadMap,saveMap;
	//sprites for browsing the textures in paint mode, stp = smalltextureplane
	Sprite mainTex,stp1,stp2,stp3,stp4;
	
	//sprites/data drag thiny
	Sprite dragArrow;
	Slider sliderOpacity,sliderSize,sliderDropoff;
	
	GLSLProgram GUIshader;
	//textur handles to paintable textures
	GLuint texHandels[8];
	int activeTex;
	bool menuUp;
	float rightClickX, rightClickY;
	bool inCircle(float cx, float cy, float x, float y,float rad);
	bool showNewMapSprite,showLoadMapSprite,showSaveMapSprite;
	bool ans;
	bool textMode;
public:
	GUI();
	~GUI();
	void init();
	void draw();
	void setTerrainInfo(TerrainInfo *t);
	void incActiveTex();
	void decActiveTex();
	int getActiveTex();
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
	bool checkDialogAnswer();
	bool addChar(char c);
	bool removeChar();
	bool isInTextMode();
	
};

#endif // GUI_H
