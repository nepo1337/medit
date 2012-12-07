#include "GUI.h"

GUI::GUI()
{
	this->menuUp=false;
	this->state = GUIstate::NONE;
	this->dragSize=vec3(0.675f,-0.39f,0.0f);
	this->dragDo=vec3(0.675f,-0.635f,0.0f);
	this->dragOp=vec3(0.675f,-0.88f,0.0f);
		
	this->activeTex=0;	

	//the gui
	this->GUIshader.compileShaderFromFile("gui.vsh",GLSLShader::VERTEX);
	this->GUIshader.compileShaderFromFile("gui.fsh",GLSLShader::FRAGMENT);
	this->GUIshader.bindAttribLocation(0,"vertexPosition");
	this->GUIshader.bindAttribLocation(1,"vertexUv");
	this->GUIshader.link();
	this->GUIshader.setUniform("guiTex", 0);
	
	//the panels to the right side
	this->frontPanel.init(vec3(0.0f),1.0f,1.0f,"gui/GUI-Front.png");
	this->backPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Back.png");
	this->drawPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Draw.png");
	this->pathPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Path.png");
	this->particlePanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Particle.png");
	this->lightPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Light.png");
	this->questPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Quest.png");
	this->modelPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Model.png");
	
	//the right click menu
	this->menuOff.init(vec3(0.0f),0.21,0.29,"gui/C-off.png");
	this->menuDraw.init(vec3(0.0f),0.21,0.29,"gui/C-Draw.png");
	this->menuLight.init(vec3(0.0f),0.21,0.29,"gui/C-Light.png");
	this->menuModel.init(vec3(0.0f),0.21,0.29,"gui/C-Model.png");
	this->menuParticle.init(vec3(0.0f),0.21,0.29,"gui/C-Particle.png");
	this->menuPath.init(vec3(0.0f),0.21,0.29,"gui/C-Path.png");
	this->menuQuest.init(vec3(0.0f),0.21,0.29,"gui/C-Quest.png");
	
	//the textures you can browse thorugh
	this->mainTex.init(vec3(0.7f,0.53f,0),0.15,0.27);
	this->stp1.init(vec3(0.521,0.043,0),0.04,0.07);
	this->stp2.init(vec3(0.621,0.043,0),0.04,0.07);
	this->stp3.init(vec3(0.771,0.043,0),0.04,0.07);
	this->stp4.init(vec3(0.871,0.043,0),0.04,0.07);
	
	//the dragesr
	this->dragArrow.init(vec3(0.0f),0.015,0.02,"gui/GUI-Arrow.png");
}

GUI::~GUI()
{

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
	glBindTexture(GL_TEXTURE_2D, this->backPanel.getTextureHandle());
	glBindVertexArray(this->backPanel.getVaoHandle());
	this->GUIshader.setUniform("modelMatrix",backPanel.getModelMatrix());
	glDrawArrays(GL_TRIANGLES,0,6);
	
	//if you are not using any tools
	if(this->state == GUIstate::NONE)
	{

	}
	
	//if you are painting textures
	if(this->state == GUIstate::PAINT)
	{
		//draws the texture planes first
		//the texture planes
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+6)%8]);
		glBindVertexArray(this->stp1.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp1.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+7)%8]);
		glBindVertexArray(this->stp2.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp2.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex)%8]);
		glBindVertexArray(this->mainTex.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->mainTex.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+1)%8]);
		glBindVertexArray(this->stp3.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp3.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->texHandels[(this->activeTex+2)%8]);
		glBindVertexArray(this->stp4.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp4.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->drawPanel.getTextureHandle());
		glBindVertexArray(this->drawPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",drawPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the dragers, same sprite used
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		mat4 modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->dragSize);
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->dragDo);
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->dragOp);
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//for placing and editing models
	if(this->state == GUIstate::MODEL)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->modelPanel.getTextureHandle());
		glBindVertexArray(this->modelPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",modelPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The lights panel
	if(this->state == GUIstate::LIGHT)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->lightPanel.getTextureHandle());
		glBindVertexArray(this->lightPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",lightPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The particle panel
	if(this->state == GUIstate::PARTICLE)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->particlePanel.getTextureHandle());
		glBindVertexArray(this->particlePanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",particlePanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The path panel
	if(this->state == GUIstate::PATH)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->pathPanel.getTextureHandle());
		glBindVertexArray(this->pathPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",pathPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//The quest panel
	if(this->state == GUIstate::QUEST)
	{
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->questPanel.getTextureHandle());
		glBindVertexArray(this->questPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",questPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//draws the front
	this->GUIshader.setUniform("modelMatrix",frontPanel.getModelMatrix());
	glBindTexture(GL_TEXTURE_2D, this->frontPanel.getTextureHandle());
	glBindVertexArray(this->frontPanel.getVaoHandle());
	glDrawArrays(GL_TRIANGLES,0,6);
	
	//draws  the front above everything
	if(this->menuUp)
	{
		if(this->state == GUIstate::NONE)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuOff.getTextureHandle());
			glBindVertexArray(this->menuOff.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuOff.getModelMatrix());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::PAINT)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuDraw.getTextureHandle());
			glBindVertexArray(this->menuDraw.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuDraw.getModelMatrix());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::MODEL)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuModel.getTextureHandle());
			glBindVertexArray(this->menuModel.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuModel.getModelMatrix());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::LIGHT)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuLight.getTextureHandle());
			glBindVertexArray(this->menuLight.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuLight.getModelMatrix());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::PARTICLE)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuParticle.getTextureHandle());
			glBindVertexArray(this->menuParticle.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuParticle.getModelMatrix());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::PATH)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuPath.getTextureHandle());
			glBindVertexArray(this->menuPath.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuPath.getModelMatrix());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		if(this->state==GUIstate::QUEST)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuQuest.getTextureHandle());
			glBindVertexArray(this->menuQuest.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuQuest.getModelMatrix());
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
	//this->menuPosMM=mat4(1.0f);
	//this->menuPosMM*=translate(vec3(x,y,0));
	this->menuOff.setPosition(vec3(x,y,0));
	this->menuDraw.setPosition(vec3(x,y,0));
	this->menuLight.setPosition(vec3(x,y,0));
	this->menuModel.setPosition(vec3(x,y,0));
	this->menuParticle.setPosition(vec3(x,y,0));
	this->menuPath.setPosition(vec3(x,y,0));
	this->menuQuest.setPosition(vec3(x,y,0));
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