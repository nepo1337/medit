#include "GUI.h"

GUI::GUI()
{
}
void GUI::init()
{
	this->activeModel=2;
	this->ans="";
	this->menuUp=false;
	this->showNewMapSprite=false;
	this->showSaveMapSprite=false;
	this->showLoadMapSprite=false;
	this->textMode=false;
	this->state = GUIstate::NONE;
	this->sliderSize=Slider(vec3(0.675f,-0.39f,0.0f));
	this->sliderDropoff=Slider(vec3(0.675f,-0.635f,0.0f));
	this->sliderOpacity=Slider(vec3(0.675f,-0.88f,0.0f));
	
	this->sliderSize.setMaxPos(0.786,-0.36f);
	this->sliderSize.setMinPos(0.56,-0.42f);
	this->sliderDropoff.setMaxPos(0.786,-0.60f);
	this->sliderDropoff.setMinPos(0.56,-0.66f);
	this->sliderOpacity.setMaxPos(0.786,-0.85f);
	this->sliderOpacity.setMinPos(0.56,-0.91f);
		
	this->activeTex=0;	
	//the gui
	this->GUIshader.compileShaderFromFile("gui.vsh",GLSLShader::VERTEX);
	this->GUIshader.compileShaderFromFile("gui.fsh",GLSLShader::FRAGMENT);
	this->GUIshader.bindAttribLocation(0,"vertexPosition");
	this->GUIshader.bindAttribLocation(1,"vertexUv");
	this->GUIshader.link();
	this->GUIshader.setUniform("guiTex", 0);
	
	
	this->modelDisplayShader.compileShaderFromFile("modelDisplay.vsh",GLSLShader::VERTEX);
	this->modelDisplayShader.compileShaderFromFile("modelDisplay.fsh",GLSLShader::FRAGMENT);
	this->modelDisplayShader.bindAttribLocation(0,"vertexPosition");
	this->modelDisplayShader.bindAttribLocation(1,"vertexNormal");
	this->modelDisplayShader.bindAttribLocation(2,"vertexUv");
	
		
	this->modelDisplayShader.link();
		
	this->modelDisplayShader.setUniform("tex1",0);
		
	glUseProgram(0);
	
	//the panels to the right side
	this->frontPanel.init(vec3(0.0f),1.0f,1.0f,"gui/GUI-Front.png");
	this->backPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Back.png");
	this->drawPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Draw.png");
	this->pathPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Path.png");
	this->particlePanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Particle.png");
	this->lightPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Light.png");
	this->questPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Quest.png");
	this->modelPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Model.png");
	this->roadPanel.init(vec3(0.7f,0.0f,0.0f),0.28f,1.0f,"gui/GUI-Road.png");
	
	//the right click menu
	this->menuOff.init(vec3(0.0f),0.21,0.29,"gui/C-off.png");
	this->menuDraw.init(vec3(0.0f),0.21,0.29,"gui/C-Draw.png");
	this->menuLight.init(vec3(0.0f),0.21,0.29,"gui/C-Light.png");
	this->menuModel.init(vec3(0.0f),0.21,0.29,"gui/C-Model.png");
	this->menuParticle.init(vec3(0.0f),0.21,0.29,"gui/C-Particle.png");
	this->menuPath.init(vec3(0.0f),0.21,0.29,"gui/C-Path.png");
	this->menuQuest.init(vec3(0.0f),0.21,0.29,"gui/C-Quest.png");
	this->menuRoad.init(vec3(0.0f),0.21,0.29,"gui/C-Road.png");
	
	//the textures you can browse thorugh
	this->mainTex.init(vec3(0.7f,0.53f,0),0.15,0.27);
	this->stp1.init(vec3(0.521,0.043,0),0.04,0.07);
	this->stp2.init(vec3(0.624,0.043,0),0.04,0.07);
	this->stp3.init(vec3(0.771,0.043,0),0.04,0.07);
	this->stp4.init(vec3(0.871,0.043,0),0.04,0.07);
	
	//the dragesr
	this->dragArrow.init(vec3(0.0f),0.015,0.02,"gui/GUI-Arrow.png");
	
	//le new map panel
	this->newMap.init(vec3(-0.2f,0.0f,0.0f),0.4,0.45,"gui/GUI-New.png");
	
	//saveMap panel
	this->saveMap.init(vec3(-0.2f,0.0f,0.0f),0.4,0.45,"gui/GUI-Save.png");
	//loadMap panel
	
	this->loadMap.init(vec3(-0.2f,0.0f,0.0f),0.4,0.45,"gui/GUI-Load.png");
	text.init("", 350, 360,0.25,1280,720,"gui/Text100.png",100,100);

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
		modelMatrix*=translate(this->sliderSize.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderDropoff.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderOpacity.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//for placing and editing models
	if(this->state == GUIstate::MODEL)
	{
		glEnable(GL_DEPTH_TEST);
		//draws the models meshes
		
		//renderin models
		this->modelDisplayShader.use();
		glActiveTexture(GL_TEXTURE0);
		//draws the 2 left small models

		for(unsigned int i=0;i<2;i++)
		{
			//set upp uniforms for rendering call

			this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].setPos(vec3(0.525+i*0.1,-0.02,0));
			this->modelDisplayShader.setUniform("modelMatrix",this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getModelMatrix());
			mat3 normalMatrix=mat3(this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getModelMatrix());
			this->modelDisplayShader.setUniform("normalMatrix",normalMatrix);
			
			//get a pointer to a vector with meshes info for each mesh
			for(unsigned int j=0;j<this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->size();j++)
			{
				glBindTexture(GL_TEXTURE_2D, this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->at(j).getTexh());
				glBindVertexArray(this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->at(j).getVaoh());
				glDrawArrays(GL_TRIANGLES,0,this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->at(j).getNrOfVerts());
			}
		}
		//the two right
		for(unsigned int i=3;i<5;i++)
		{
			//set upp uniforms for rendering call
			this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].setPos(vec3(0.47+i*0.1,-0.02,0));
			this->modelDisplayShader.setUniform("modelMatrix",this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getModelMatrix());
			mat3 normalMatrix=mat3(this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getModelMatrix());
			this->modelDisplayShader.setUniform("normalMatrix",normalMatrix);
			
			//get a pointer to a vector with meshes info for each mesh
			for(unsigned int j=0;j<this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->size();j++)
			{
				glBindTexture(GL_TEXTURE_2D, this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->at(j).getTexh());
				glBindVertexArray(this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->at(j).getVaoh());
				glDrawArrays(GL_TRIANGLES,0,this->displayModels[abs(int(this->activeModel+i))%this->displayModels.size()].getMeshInfo()->at(j).getNrOfVerts());
			}
		}
		
		//set upp uniforms for rendering call
		this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].setPos(vec3(0.7,0.25,0));
		this->modelDisplayShader.setUniform("modelMatrix",this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].getModelMatrix());
		mat3 normalMatrix=mat3(this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].getModelMatrix());
		this->modelDisplayShader.setUniform("normalMatrix",normalMatrix);
		
		//get a pointer to a vector with meshes info for each mesh
		for(unsigned int j=0;j<this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->displayModels[abs(this->activeModel+2)%this->displayModels.size()].getMeshInfo()->at(j).getNrOfVerts());
		}
		//draws the main panel
		
		this->GUIshader.use();
		glDisable(GL_DEPTH_TEST);
		
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
	
	if(this->state == GUIstate::ROAD)
	{
		glBindTexture(GL_TEXTURE_2D, this->surfaceTexHandles[0]);
		glBindVertexArray(this->mainTex.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->mainTex.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->roadPanel.getTextureHandle());
		glBindVertexArray(this->roadPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",roadPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	
	//draws the front
	this->GUIshader.setUniform("modelMatrix",frontPanel.getModelMatrix());
	glBindTexture(GL_TEXTURE_2D, this->frontPanel.getTextureHandle());
	glBindVertexArray(this->frontPanel.getVaoHandle());
	glDrawArrays(GL_TRIANGLES,0,6);
	
	//if you press new map, show sprite
	if(this->showNewMapSprite)
	{
		glBindTexture(GL_TEXTURE_2D, this->newMap.getTextureHandle());
		glBindVertexArray(this->newMap.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",newMap.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//if you want to load a map  show sprite
	if(this->showLoadMapSprite)
	{
		glBindTexture(GL_TEXTURE_2D, this->loadMap.getTextureHandle());
		glBindVertexArray(this->loadMap.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",loadMap.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	//if you want to save show sprite
	if(this->showSaveMapSprite)
	{
		glBindTexture(GL_TEXTURE_2D, this->saveMap.getTextureHandle());
		glBindVertexArray(this->saveMap.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",saveMap.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	if(this->textMode)
	{
		glBindTexture(GL_TEXTURE_2D, text.getTexHandle());
		glBindVertexArray(text.getVaoHandle());
		mat4 mm=mat4(1.0f);
		this->GUIshader.setUniform("modelMatrix",mm);
		glDrawArrays(GL_TRIANGLES,0,this->text.getNrOfVerts());
	}
	
	
	//draws  the menu above everything
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
		if(this->state==GUIstate::ROAD)
		{
			//draws the right click panel
			glBindTexture(GL_TEXTURE_2D, this->menuRoad.getTextureHandle());
			glBindVertexArray(this->menuRoad.getVaoHandle());
			this->GUIshader.setUniform("modelMatrix",this->menuRoad.getModelMatrix());
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
	this->menuRoad.setPosition(vec3(x,y,0));
}

void GUI::moveSliders(float x, float y)
{
		//move the sliders
		if(this->sliderSize.isInsideSliderSpace(x,y))
			this->sliderSize.setPositionX(x);

		if(this->sliderDropoff.isInsideSliderSpace(x,y))
			this->sliderDropoff.setPositionX(x);

		if(this->sliderOpacity.isInsideSliderSpace(x,y))
			this->sliderOpacity.setPositionX(x);
}

void GUI::setLeftClick(float x, float y)
{	
	this->ans="";
	//when clicking on menus
	if(this->state==GUIstate::PAINT)
	{
		//for browsing textures, could be replaced with a button class
		if(this->inCircle(x,y, 0.5,0.52,0.03))
			this->incActiveTex();
		if(this->inCircle(x,y, 0.9,0.52,0.03))
			this->decActiveTex();
	}
	if(this->state==GUIstate::MODEL)
	{
		if(this->inCircle(x,y, 0.5,0.52,0.03))
		{
			this->activeModel++;
			if(this->activeModel>this->displayModels.size()-1)
				this->activeModel=0;
		}	
		
		if(this->inCircle(x,y, 0.9,0.52,0.03))
		{
			this->activeModel--;
			if(this->activeModel<0)
				this->activeModel=this->displayModels.size()-1;
		}
		
	}
	//when clicking on the top bar
	if(y>0.9)
	{
		//the position of the grid le button
		if(x>-0.75&&x<-0.7)
		{
			cout<<"GRID"<<endl;
		}
		//exit
		if(x>-0.685&&x<-0.64)
		{
			cout<<"EXIT"<<endl;
		}
		//new
		if(x>-0.94&&x<-0.9)
		{
			this->showNewMapSprite=true;
			this->showLoadMapSprite=false;
			this->showSaveMapSprite=false;
		}
		//load
		if(x>-0.881&&x<-0.835)
		{
			this->showLoadMapSprite=true;
			this->showSaveMapSprite=false;
			this->showNewMapSprite=false;
			this->textMode=true;
		}
		//save
		if(x>-0.817&&x<-0.77)
		{
			this->showSaveMapSprite=true;
			this->showLoadMapSprite=false;
			this->showNewMapSprite=false;
			this->textMode=true;
		}
	}
	//if new map sprite is up
	if(showNewMapSprite)
	{
		//if ok
		if(x>-0.367&&x<-0.212&&y>-0.16&&y<-0.09)
		{
			this->ans="nmOK";
			this->showNewMapSprite=false;
		}
		//if cancel
		if(x>-0.181&&x<-0.02&&y>-0.16&&y<-0.09)
		{
			this->ans="nmC";
			this->showNewMapSprite=false;
		}
	}
	
	//if show load sprite is up
	if(showLoadMapSprite)
	{
		if(x>-0.39&&x<-0.232&&y>-0.088&&y<-0.033)
		{
			this->ans="lmOK";
			this->showLoadMapSprite=false;
			this->textMode=false;
		}
		//if cancel
		if(x>-0.2&&x<-0.045&&y>-0.09&&y<-0.02)
		{
			this->ans="loC";
			this->showLoadMapSprite=false;
			this->textMode=false;
		}
	}
	
	//if save sprite is up
	if(showSaveMapSprite)
	{
		if(x>-0.39&&x<-0.232&&y>-0.088&&y<-0.033)
		{
			this->ans="svOK";
			//this->showSaveMapSprite=false;
			this->textMode=false;
		}
		//if cancel
		if(x>-0.2&&x<-0.045&&y>-0.09&&y<-0.02)
		{
			this->ans="svC";
			//this->showSaveMapSprite=false;
			this->textMode=false;
		}
	}

	if(this->state==GUIstate::ROAD)
	{
		if(x>0.63 && x < 0.77 && y>-0.25&&y<-0.177)
		{
			this->ans="RS";
		}
	}
}
string GUI::checkDialogAnswer()
{
	return this->ans;
}

void GUI::setMouseXY(float x, float y)
{	
	//if using the menu, updates what is hovered
	if(this->menuUp)
	{
		//calculates the differences where the mouse where right clicked
		float ax,ay;
		ax=x-this->rightClickX;
		ay=y-this->rightClickY;
		//so we can count on that 0,0 is in the middle

		//ax,ay is the difference from the click, the other parameters are the middle point of the circle
		//where the choice(for ex paint) is located
		
		if(this->inCircle(ax,ay,0,0,0.07))
		{
			this->state=GUIstate::NONE;
		}
		if(this->inCircle(ax,ay,-0.135,0,0.06))
		{
			this->state=GUIstate::LIGHT;
		}
		if(this->inCircle(ax,ay,+0.134,0.07,0.07))
		{
			this->state=GUIstate::MODEL;
		}
		if(this->inCircle(ax,ay,-0.0546,0.211,0.07))
		{
			this->state=GUIstate::PATH;
		}
		if(this->inCircle(ax,ay,0.0578,0.214,0.07))
		{
			this->state=GUIstate::QUEST;
		}
		if(this->inCircle(ax,ay,0.001,-0.195,0.07))
		{
			this->state=GUIstate::PAINT;
		}
		if(this->inCircle(ax,ay,0.1125,-0.108,0.07))
		{
			this->state=GUIstate::PARTICLE;
		}
		if(this->inCircle(ax,ay,-0.104,-0.11,0.07))
		{
			this->state=GUIstate::ROAD;
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

float GUI::getSliderRadius()
{
	return this->sliderSize.getSliderValueX();
}
float GUI::getSliderOpacity()
{
	return this->sliderOpacity.getSliderValueX();
}
float GUI::getSliderDropoff()
{
	return this->sliderDropoff.getSliderValueX();
}

bool GUI::addChar(char c)
{
	if(textMode)
	{
		
		{
			string tmp="";
			tmp+=c;
			this->text.addText(tmp);
			

			if(showLoadMapSprite)
			{

			}
			if(showSaveMapSprite)
			{

			}
		}
	}
	return textMode;
}
bool GUI::removeChar()
{
	this->text.removeLastChar();
	return this->textMode;
}
bool GUI::isInTextMode()
{
	return this->textMode;
}

string GUI::getInputText()
{
	return this->text.getText();
}
void GUI::hideSaveMapDialog()
{
	showSaveMapSprite=false;
}
bool GUI::isSaveMapDialogUp()
{
	return showSaveMapSprite;
}
bool GUI::isLoadMapDialogUp()
{
	return showLoadMapSprite;
}

bool GUI::isNewMapDialogUp()
{
	return this->showNewMapSprite;
}

void GUI::addDisplayModel(Model m)
{
	Model tmp = m;
	float scaleX=tmp.getBoundingBox()->getBboxSide().x;
	float scaleY=tmp.getBoundingBox()->getBboxSide().y;
	float scaleZ=tmp.getBoundingBox()->getBboxSide().z;
	float scale=scaleY;
	if(scaleX>scaleY)
		scale=scaleX;
	if(scaleZ>scaleX)
		scale=scaleZ;
	m.scaleXYZ(0.04/(scale));
	this->displayModels.push_back(m);
}
void GUI::setSurfaceTexHandles(vector<GLuint> th)
{
	this->surfaceTexHandles = th;
}
void GUI::resetDialogAns()
{
	this->ans="";
}
int GUI::getActiveModelIndex()
{
	return abs(this->activeModel+2)%this->displayModels.size();
}