#ifndef GUI_H
#define GUI_H
#include "SOIL/SOIL.h"
#include <glew.h>
#include "TA.h"
#include "GLSLProgram.h"
#include <iostream>

using namespace std;

namespace GUIstate
{
  enum GUIstates
  {
    PAINT,MODEL,LIGHT,PATH,PARTICLE,QUEST
  };
};

class GUI
{
private:
	GUIstate::GUIstates state;
	GLuint vaoHmain, vaoHpanel, vaoHmainTex, vaohsmalltp1,vaohsmalltp2,vaohsmalltp3,vaohsmalltp4;
	GLuint vbohs[8];
	//textures to GUI releated textures
	GLuint textureH[3];
	int nrOfTex;
	GLSLProgram GUIshader;
	//textur handles to paintable textures
	GLuint texHandels[8];
	int activeTex;
public:
	GUI();
	~GUI();
	void draw();
	void setTerrainInfo(TerrainInfo *t);
	void incActiveTex();
	void decActiveTex();
	int getActiveTex();

};

#endif // GUI_H
