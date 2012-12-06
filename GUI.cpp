#include "GUI.h"

GUI::GUI()
{
	this->menuPosMM=mat4(1.0f);
	this->menuUp=false;
	this->state = GUIstate::NONE;
	this->vaoHmain=0;
	this->vaoHpanel=0;
	this->vaoHmainTex=0;
	this->vaohsmalltp1=0;
	this->vaohsmalltp2=0;
	this->vaohsmalltp3=0;
	this->vaohsmalltp4=0;
	this->vaohrcp=0;
	for(int i=0;i<9;i++)
		this->vbohs[i]=0;
	
	this->nrOfTex=15;
	for(int i=0;i<this->nrOfTex;i++)
		this->textureH[i]=0;
		
	this->activeTex=0;
	
	//defines the plane for drawing the main GUI (clipspace)
	//all positions are hardcoded :/:/:/
	
	//uv cords
	float mainGUIUV[]=
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f
	};
	
	//the front GUI
	float mainGUI[]=
	{
		-1.0f,-1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		-1.0f,1.0f,0.0f
	};
	
	//the panel to the right
	float panelBackground[]=
	{
		0.41f,-1.0f,0.0f,
		0.41f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.41f,1.0f,0.0f
	};
	
	//main texture plane
	float mainTexPlane[]=
	{
		0.53f,0.23f,0.0f,
		0.53f,0.83f,0.0f,
		0.875f, 0.23f,0.0f,
		0.875f, 0.83f,0.0f,
		0.875f, 0.23f,0.0f,
		0.53f,0.83f,0.0f
	};
	
	//the small texplane (1)
	float stp1[]=
	{
		0.48f,-0.03f,0.0f,
		0.48f, 0.11f,0.0f,
		0.56f,-0.03f,0.0f,
		0.56f, 0.11f,0.0f,
		0.56f,-0.03f,0.0f,
		0.48f, 0.11f,0.0f
	};
	
	//the small texplane (2)
	float stp2[]=
	{
		0.58f,-0.03f,0.0f,
		0.58f, 0.11f,0.0f,
		0.67f,-0.03f,0.0f,
		0.67f, 0.11f,0.0f,
		0.67f,-0.03f,0.0f,
		0.58f, 0.11f,0.0f
	};
	
	//the small texplane (3)
	float stp3[]=
	{
		0.73f,-0.03f,0.0f,
		0.73f, 0.11f,0.0f,
		0.825f,-0.03f,0.0f,
		0.825f, 0.11f,0.0f,
		0.825f,-0.03f,0.0f,
		0.73f, 0.11f,0.0f
	};
	
	//the small texplane (4)
	float stp4[]=
	{
		0.84f,-0.03f,0.0f,
		0.84f, 0.11f,0.0f,
		0.93f,-0.03f,0.0f,
		0.93f, 0.11f,0.0f,
		0.93f,-0.03f,0.0f,
		0.84f, 0.11f,0.0f
	};
	
	//the right click plane, defined in the middle so model matrix can easily be applied
	float rcp[]=
	{
		-0.2f,-0.29f,0.0f,
		-0.2f, 0.29f,0.0f,
		0.2f, -0.29f,0.0f,
		0.2f, 0.29f,0.0f,
		0.2f,-0.29f,0.0f,
		-0.2f, 0.29f,0.0f
	};
	
	glGenVertexArrays(1,&vaoHmain);
	glGenVertexArrays(1,&vaoHpanel);
	glGenVertexArrays(1,&vaoHmainTex);
	glGenVertexArrays(1,&vaohsmalltp1);
	glGenVertexArrays(1,&vaohsmalltp2);
	glGenVertexArrays(1,&vaohsmalltp3);
	glGenVertexArrays(1,&vaohsmalltp4);
	glGenVertexArrays(1,&vaohrcp);
	glGenBuffers(9,vbohs);
	
	//front
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mainGUI),mainGUI,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mainGUIUV), mainGUIUV, GL_STATIC_DRAW);
	
	//right panel
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(panelBackground),panelBackground,GL_STATIC_DRAW);
	
	//main tex plane
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mainTexPlane),mainTexPlane,GL_STATIC_DRAW);
	
	//small texplane 1
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stp1),stp1,GL_STATIC_DRAW);
	
	//small texplane 2
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stp2),stp2,GL_STATIC_DRAW);
	
	//small texplane 3
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stp3),stp3,GL_STATIC_DRAW);
	
	//small texplane 4
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stp4),stp4,GL_STATIC_DRAW);
	
	//the panel when rightclicking
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rcp),rcp,GL_STATIC_DRAW);
	
	//front vao
	glBindVertexArray(vaoHmain);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//panel vao
	glBindVertexArray(vaoHpanel);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//main tex vao
	glBindVertexArray(vaoHmainTex);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[3]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	
	//small texplane 1
	glBindVertexArray(vaohsmalltp1);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[4]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//small teplane 2
	glBindVertexArray(vaohsmalltp2);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[5]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//small teplane 3
	glBindVertexArray(vaohsmalltp3);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[6]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//small teplane 3
	glBindVertexArray(vaohsmalltp4);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[7]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//the right click plane
	glBindVertexArray(this->vaohrcp);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[8]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	//loads all gui textures in to textureslot 0
	glActiveTexture(GL_TEXTURE0);
	this->textureH[0] = SOIL_load_OGL_texture("gui/GUI-Front.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[1] = SOIL_load_OGL_texture("gui/GUI-Back.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[2] = SOIL_load_OGL_texture("gui/GUI-Draw.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[3] = SOIL_load_OGL_texture("gui/GUI-Model.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[4] = SOIL_load_OGL_texture("gui/GUI-Light.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[5] = SOIL_load_OGL_texture("gui/GUI-Particle.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[6] = SOIL_load_OGL_texture("gui/GUI-Path.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[7] = SOIL_load_OGL_texture("gui/GUI-Quest.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[8] = SOIL_load_OGL_texture("gui/C-off.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[9] = SOIL_load_OGL_texture("gui/C-Draw.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[10] = SOIL_load_OGL_texture("gui/C-Light.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[11] = SOIL_load_OGL_texture("gui/C-Model.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[12] = SOIL_load_OGL_texture("gui/C-Particle.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[13] = SOIL_load_OGL_texture("gui/C-Path.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	this->textureH[14] = SOIL_load_OGL_texture("gui/C-Quest.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

	//the standard static gui
	this->GUIshader.compileShaderFromFile("gui.vsh",GLSLShader::VERTEX);
	this->GUIshader.compileShaderFromFile("gui.fsh",GLSLShader::FRAGMENT);
	this->GUIshader.bindAttribLocation(0,"vertexPosition");
	this->GUIshader.bindAttribLocation(1,"vertexUv");
	this->GUIshader.link();
	this->GUIshader.setUniform("guiTex", 0);
	this->GUIshader.setUniform("modelMatrix", this->menuPosMM);
	
	//moveable gui parts
	this->GUImoveable.compileShaderFromFile("guiMoveable.vsh",GLSLShader::VERTEX);
	this->GUImoveable.compileShaderFromFile("guiMoveable.fsh",GLSLShader::FRAGMENT);
	this->GUImoveable.bindAttribLocation(0,"vertexPosition");
	this->GUImoveable.bindAttribLocation(1,"vertexUv");
	this->GUImoveable.link();
	this->GUImoveable.setUniform("guiTex", 0);
	this->GUImoveable.setUniform("modelMatrix", this->menuPosMM);
}

GUI::~GUI()
{
	glDeleteTextures(this->nrOfTex,textureH);
	glDeleteBuffers(9,this->vbohs);
	glDeleteVertexArrays(1, &this->vaoHmain);
	glDeleteVertexArrays(1, &this->vaoHpanel);
}

void GUI::draw()
{
	//disables depthtest so the gui will not overlap the terrain
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->GUIshader.use();
	
	//OBS! need to take the depth test disabled in mind and draw in the correct order
	//draws the empty panel
	glBindTexture(GL_TEXTURE_2D, this->textureH[1]);
	glBindVertexArray(this->vaoHpanel);
	glDrawArrays(GL_TRIANGLES,0,6);
	
	//if you are not using any tools
	if(this->state == GUIstate::NONE)
	{

	}
	
	this->GUIshader.use();
	//if you are painting textures
	if(this->state == GUIstate::PAINT)
	{
		//draws the texture planes first
		//the texture planes
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+6)%8]);
		glBindVertexArray(this->vaohsmalltp1);
		glDrawArrays(GL_TRIANGLES,0,6);
		this->menuPosMM=mat4(1.0f);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+7)%8]);
		glBindVertexArray(this->vaohsmalltp2);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex)%8]);
		glBindVertexArray(this->vaoHmainTex);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+1)%8]);
		glBindVertexArray(this->vaohsmalltp3);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+2)%8]);
		glBindVertexArray(this->vaohsmalltp4);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->textureH[2]);
		glBindVertexArray(this->vaoHpanel);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//for placing and editing models
	if(this->state == GUIstate::MODEL)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->textureH[3]);
		glBindVertexArray(this->vaoHpanel);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The lights panel
	if(this->state == GUIstate::LIGHT)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->textureH[4]);
		glBindVertexArray(this->vaoHpanel);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The particle panel
	if(this->state == GUIstate::PARTICLE)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->textureH[5]);
		glBindVertexArray(this->vaoHpanel);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The path panel
	if(this->state == GUIstate::PATH)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->textureH[6]);
		glBindVertexArray(this->vaoHpanel);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The quest panel
	if(this->state == GUIstate::QUEST)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->textureH[7]);
		glBindVertexArray(this->vaoHpanel);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//draws the front
	glBindTexture(GL_TEXTURE_2D, this->textureH[0]);
	glBindVertexArray(this->vaoHmain);
	glDrawArrays(GL_TRIANGLES,0,6);
	
	//draws  the front above everything
	if(this->menuUp)
	{
		this->GUImoveable.use();
		if(this->state == GUIstate::NONE)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[8]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
			this->menuPosMM=mat4(1.0f);
		}
		if(this->state==GUIstate::PAINT)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[9]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::MODEL)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[11]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::LIGHT)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[10]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::PARTICLE)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[12]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::PATH)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[13]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::QUEST)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->textureH[14]);
			glBindVertexArray(this->vaohrcp);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
	}
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glUseProgram(0);
	glBindVertexArray(0);
}

void GUI::setTerrainInfo(TerrainInfo *t)
{
	this->texHandels[0]=t->texH[0];
	this->texHandels[1]=t->texH[1];
	this->texHandels[2]=t->texH[2];
	this->texHandels[3]=t->texH[3];
	this->texHandels[4]=t->tex2H[0];
	this->texHandels[5]=t->tex2H[1];
	this->texHandels[6]=t->tex2H[2];
	this->texHandels[7]=t->tex2H[3];
}

int GUI::getActiveTex()
{
	return this->activeTex;
}
void GUI::incActiveTex()
{
	this->activeTex++;
	if(this->activeTex>7)
		this->activeTex=0;
}
void GUI::decActiveTex()
{
	this->activeTex--;
	if(this->activeTex<0)
		this->activeTex=7;
}

void GUI::showMenu(bool ans)
{
	this->menuUp=ans;
}

void GUI::setGuiState(GUIstate::GUIstates state)
{
	this->state=state;
}

void GUI::setRightClickXY(float x, float y)
{
	this->rightClickX=x;
	this->rightClickY=y;
	this->menuPosMM=mat4(1.0f);
	this->menuPosMM*=translate(vec3(x,y,0));
	this->GUImoveable.use();
	this->GUImoveable.setUniform("modelMatrix", this->menuPosMM);
}

void GUI::setLeftClick(float x, float y)
{
	this->mouseX=x;
	this->mouseY=y;
	
	//when clicking on menus
	if(this->state==GUIstate::PAINT)
	{
		if(this->inCircle(this->mouseX,this->mouseY, 0.5,0.52,0.03))
			this->incActiveTex();
		if(this->inCircle(this->mouseX,this->mouseY, 0.9,0.52,0.03))
			this->decActiveTex();
	}
}

void GUI::setMouseXY(float x, float y)
{
	this->mouseX=x;
	this->mouseY=y;
	
	//if using the menu, updates what is hovered
	if(this->menuUp)
	{
		//calculates the differences where the mouse where right clicked
		float ax,ay;
		ax=this->mouseX-this->rightClickX;
		ay=this->mouseY-this->rightClickY;
		//so we can count on that 0,0 is in the middle
		
		//ax,ay is the difference from the click, the other parameters are the middle point of the circle
		//where the choice(for ex paint) is located
		
		if(this->inCircle(ax,ay,0,0,0.07))
		{
			this->state=GUIstate::NONE;
		}
		if(this->inCircle(ax,ay,-0.125,0,0.07))
		{
			this->state=GUIstate::LIGHT;
		}
		if(this->inCircle(ax,ay,+0.125,0,0.07))
		{
			this->state=GUIstate::MODEL;
		}
		if(this->inCircle(ax,ay,-0.0625,0.185,0.07))
		{
			this->state=GUIstate::PATH;
		}
		if(this->inCircle(ax,ay,0.0625,0.185,0.07))
		{
			this->state=GUIstate::QUEST;
		}
		if(this->inCircle(ax,ay,-0.0625,-0.185,0.07))
		{
			this->state=GUIstate::PAINT;
		}
		if(this->inCircle(ax,ay,0.0625,-0.185,0.07))
		{
			this->state=GUIstate::PARTICLE;
		}
	}
}

bool GUI::inCircle(float cx, float cy, float x, float y,float rad)
{
	float dist;
	dist = (cx-x)*(cx-x)+(cy-y)*(cy-y);
	return dist<rad*rad;
}
GUIstate::GUIstates GUI::getState()
{
	return this->state;
}

bool GUI::isInDrawWindow(float x, float y)
{
	return (x>-0.94&&x<0.4&&y>-0.9&&y<0.9);
}