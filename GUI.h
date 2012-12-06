#ifndef GUI_H
#define GUI_H
#include "SOIL/SOIL.h"
#include <glew.h>
#include "TA.h"
#include "GLSLProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>

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
	GLuint vaoHmain, vaoHpanel, vaoHmainTex, vaohsmalltp1,vaohsmalltp2,vaohsmalltp3,vaohsmalltp4,vaohrcp;
	GLuint vbohs[9];
	//textures to GUI releated textures
	GLuint textureH[15];
	int nrOfTex;
	GLSLProgram GUIshader,GUImoveable;
	//textur handles to paintable textures
	GLuint texHandels[8];
	int activeTex;
	bool menuUp;
	float rightClickX, rightClickY, mouseX,mouseY;
	mat4 menuPosMM;
	bool inCircle(float cx, float cy, float x, float y,float rad);
public:
	GUI();
	~GUI();
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
	
};

#endif // GUI_H
