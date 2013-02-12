#include "GUI.h"

GUI::GUI()
{
}
void GUI::init()
{
	this->activeSurfaceTex=2;
	this->activeParticleIndex=2;
	this->isPlacingLights=false;
	this->placingParticleSystems=false;
	this->selectRoad=false;
	this->activeModelIndex=2;
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
	this->sliderScale=Slider(vec3(0.672,-0.68,0.0));
	this->sliderColorPicker=Slider(vec3(0.8245,0.4975,0));
	this->sliderContrast = Slider(vec3(0.695f,-0.25f,0.0f));
	this->sliderContrast.setMinPos(0.54f,-0.275f);
	this->sliderContrast.setMaxPos(0.85f,-0.21f);
	this->sliderRadius = Slider(vec3(0.695f,-0.452f,0.0f));
	this->sliderRadius.setMinPos(0.54f,-0.475f);
	this->sliderRadius.setMaxPos(0.85f,-0.41f);
	this->sliderHeight= Slider(vec3(0.54,-0.67,0));
	this->sliderHeight.setMinPos(0.54,-0.7);
	this->sliderHeight.setMaxPos(0.85,-0.62);
	this->sliderRoadSpacing = Slider(vec3(0.85,-0.57,0));
	this->sliderRoadSpacing.setMinPos(0.54,-0.59);
	this->sliderRoadSpacing.setMaxPos(0.85,-0.53);
	this->sliderRoadScale = Slider(vec3(0.54, -0.87,0));
	this->sliderRoadScale.setMinPos(0.54,-0.88);
	this->sliderRoadScale.setMaxPos(0.85,-0.83);
	
	this->sliderModelHeight=Slider(vec3(0.542,-0.34,0));
	this->sliderModelHeight.setMinPos(0.542, -0.36);
	this->sliderModelHeight.setMaxPos(0.855,-0.31);
	
	this->sliderScale.setMinPos(0.56,-0.7);
	this->sliderScale.setMaxPos(0.784,-0.65f);
	this->sliderSize.setMaxPos(0.786,-0.36f);
	this->sliderSize.setMinPos(0.56,-0.42f);
	this->sliderDropoff.setMaxPos(0.786,-0.60f);
	this->sliderDropoff.setMinPos(0.56,-0.66f);
	this->sliderOpacity.setMaxPos(0.786,-0.85f);
	this->sliderOpacity.setMinPos(0.56,-0.91f);
	
	this->sliderColorPicker.setMinPos(0.731,0.325);
	this->sliderColorPicker.setMaxPos(0.918,0.67);
		
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
	
	this->modelDisplayShader.setUniform("tex1",0);
	
		
	this->modelDisplayShader.link();
	
	this->colorPlaneShader.compileShaderFromFile("colorPlane.vsh",GLSLShader::VERTEX);
	this->colorPlaneShader.compileShaderFromFile("colorPlane.fsh",GLSLShader::FRAGMENT);
	this->colorPlaneShader.bindAttribLocation(0,"vertexPosition");
	
	cout << this->colorPlaneShader.log();
	this->colorPlaneShader.link();
		cout << this->colorPlaneShader.log();
	
		

	
	//le color picker sprite
	this->colorPickerSprite.init(vec3(this->sliderColorPicker.getPosition().x,this->sliderColorPicker.getPosition().y,0),0.015,0.015,"gui/marker.png");
	
	
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
	
	//the texture for light representing the type of light used
	this->spriteAmbient.init(vec3(0.525,0.0447,0),0.04,0.08,"gui/Ambient.png");
	this->spritePointLight.init(vec3(0.626,0.0447,0),0.04,0.08,"gui/PointLight.png");
	this->spritePointLightShadow.init(vec3(0.77,0.0447,0),0.04,0.08,"gui/PointLightShadow.png");
	this->spriteSpotLight.init(vec3(0.87,0.0447,0),0.04,0.08,"gui/SpotLight.png");
	
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
	
	this->splashScreen.init(vec3(0.0f,0.0f,0.0f),1,1,"gui/splash.png");
	
	text.init("", 350, 360,0.25,1280,720,"gui/Text100.png",100,100);
	this->colorPlaneSprite.init(vec3(0.89f,0.8f,0.0f),0.05,0.08);
	
	this->activeModel.scaleXYZ(this->sliderScale.getSliderValueX()*2);
	this->colorScale.LoadFromFile("gui/colorScale.png");
	
	float xm = this->sliderColorPicker.getMaxX()-this->sliderColorPicker.getMinX();
	float ym = this->sliderColorPicker.getMaxY()-this->sliderColorPicker.getMinY();
	float xp = sliderColorPicker.getPosition().x-this->sliderColorPicker.getMinX();
	float yp = sliderColorPicker.getPosition().y-this->sliderColorPicker.getMinY();

	float colorCordX=xp*(this->colorScale.GetWidth()/xm);
	float colorCordY=this->colorScale.GetHeight()-(yp*(this->colorScale.GetHeight()/ym));
	sf::Color colorPicked = this->colorScale.GetPixel(colorCordX,colorCordY);
	this->colorPlaneShader.use();

	this->normalizedColor.x = (float)colorPicked.r/255;
	this->normalizedColor.y = (float)colorPicked.g/255;
	this->normalizedColor.z = (float)colorPicked.b/255;
	this->colorPlaneShader.setUniform("ro",normalizedColor.x);
	this->colorPlaneShader.setUniform("go",normalizedColor.y);
	this->colorPlaneShader.setUniform("bo",normalizedColor.z);
	glUseProgram(0);
	
	this->activeLight.setColor(this->normalizedColor);
	this->activeLight.setContrast(1);
	this->activeLight.setRadius(this->sliderRadius.getSliderValueX()*5);
	this->activeLight.setLightType(LightType::POINTLIGHTSHADOW);
	
	this->activeModel.setPos(vec3(this->activeModel.getPos().x,this->sliderModelHeight.getSliderValueX(),this->activeModel.getPos().z));
	this->activeParticle.setColor(this->normalizedColor);
	this->activeParticle.setParticleType(ParticleType::FIRE);
	
	Sprite p;
	particleImagesHandles.push_back(p);
	particleImagesHandles.push_back(p);
	particleImagesHandles.push_back(p);
	particleImagesHandles.push_back(p);
	particleImagesHandles.push_back(p);
	this->particleImagesHandles[0].init(vec3(0.0f),0.0,0.0,"models/particles/emit.png");
	this->particleImagesHandles[1].init(vec3(0.0f),0.0,0.0,"models/particles/fire.png");
	this->particleImagesHandles[2].init(vec3(0.57,0.50,0.0f),0.1,0.18,"models/particles/flow.png");
	this->particleImagesHandles[3].init(vec3(0.0f),0.0,0.0,"models/particles/glowring.png");
	this->particleImagesHandles[4].init(vec3(0.0f),0.0,0.0,"models/particles/smoke.png");
	
	Particle p1;
	this->particles.push_back(p1);
	this->particles.push_back(p1);
	this->particles.push_back(p1);
	this->particles.push_back(p1);
	this->particles.push_back(p1);
	this->particles[0].setParticleType(ParticleType::EMIT);
	this->particles[1].setParticleType(ParticleType::FIRE);
	this->particles[2].setParticleType(ParticleType::FLOW);
	this->particles[3].setParticleType(ParticleType::GLOWRING);
	this->particles[4].setParticleType(ParticleType::SMOKE);
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
		//glEnable(GL_DEPTH_TEST);
		//draws the models meshes
		
		//renderin models
		this->modelDisplayShader.use();
		glActiveTexture(GL_TEXTURE0);
		//draws the 2 left small models

		for(unsigned int i=0;i<2;i++)
		{
			//set upp uniforms for rendering call

			this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].setPos(vec3(0.525+i*0.1,-0.02,0));
			this->modelDisplayShader.setUniform("modelMatrix",this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getModelMatrix());
			mat3 normalMatrix=mat3(this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getModelMatrix());
			this->modelDisplayShader.setUniform("normalMatrix",normalMatrix);
			
			//get a pointer to a vector with meshes info for each mesh
			for(unsigned int j=0;j<this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->size();j++)
			{
				glBindTexture(GL_TEXTURE_2D, this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->at(j).getTexh());
				glBindVertexArray(this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->at(j).getVaoh());
				glDrawArrays(GL_TRIANGLES,0,this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->at(j).getNrOfVerts());
			}
		}
		//the two right
		for(unsigned int i=3;i<5;i++)
		{
			//set upp uniforms for rendering call
			this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].setPos(vec3(0.47+i*0.1,-0.02,0));
			this->modelDisplayShader.setUniform("modelMatrix",this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getModelMatrix());
			mat3 normalMatrix=mat3(this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getModelMatrix());
			this->modelDisplayShader.setUniform("normalMatrix",normalMatrix);
			
			//get a pointer to a vector with meshes info for each mesh
			for(unsigned int j=0;j<this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->size();j++)
			{
				glBindTexture(GL_TEXTURE_2D, this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->at(j).getTexh());
				glBindVertexArray(this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->at(j).getVaoh());
				glDrawArrays(GL_TRIANGLES,0,this->displayModels[abs(int(this->activeModelIndex+i))%this->displayModels.size()].getMeshInfo()->at(j).getNrOfVerts());
			}
		}
		
		//set upp uniforms for rendering call
		this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].setPos(vec3(0.7,0.3,0));
		this->modelDisplayShader.setUniform("modelMatrix",this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getModelMatrix());
		mat3 normalMatrix=mat3(this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getModelMatrix());
		this->modelDisplayShader.setUniform("normalMatrix",normalMatrix);
		
		//get a pointer to a vector with meshes info for each mesh
		for(unsigned int j=0;j<this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getMeshInfo()->at(j).getNrOfVerts());
		}
		//draws the main panel
		
		this->GUIshader.use();
		//glDisable(GL_DEPTH_TEST);
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->modelPanel.getTextureHandle());
		glBindVertexArray(this->modelPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",modelPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the dragers, same sprite used
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		mat4 modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderScale.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderModelHeight.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		
	}
	
	//The lights panel
	if(this->state == GUIstate::LIGHT)
	{		
		//draws the color choosed
		mat4 modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->colorPlaneSprite.getPosition());
		this->colorPlaneShader.use();
		this->colorPlaneShader.setUniform("modelMatrix",modelMatrix);
		glBindVertexArray(this->colorPlaneSprite.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		this->GUIshader.use();
		
		//draws the types of light
		//ambient
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->spriteAmbient.getPosition());
		this->GUIshader.setUniform("modelMatrix",spriteAmbient.getModelMatrix());
		glBindTexture(GL_TEXTURE_2D, this->spriteAmbient.getTextureHandle());
		glBindVertexArray(this->spriteAmbient.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		//pointlight
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->spritePointLight.getPosition());
		this->GUIshader.setUniform("modelMatrix",spritePointLight.getModelMatrix());
		glBindTexture(GL_TEXTURE_2D, this->spritePointLight.getTextureHandle());
		glBindVertexArray(this->spritePointLight.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		//pointLightShadow
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->spritePointLightShadow.getPosition());
		this->GUIshader.setUniform("modelMatrix",spritePointLightShadow.getModelMatrix());
		glBindTexture(GL_TEXTURE_2D, this->spritePointLightShadow.getTextureHandle());
		glBindVertexArray(this->spritePointLightShadow.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		//spotLight
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->spriteSpotLight.getPosition());
		this->GUIshader.setUniform("modelMatrix",spriteSpotLight.getModelMatrix());
		glBindTexture(GL_TEXTURE_2D, this->spriteSpotLight.getTextureHandle());
		glBindVertexArray(this->spriteSpotLight.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->lightPanel.getTextureHandle());
		glBindVertexArray(this->lightPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",lightPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the "x", the picker
		glBindTexture(GL_TEXTURE_2D, this->colorPickerSprite.getTextureHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderColorPicker.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glBindVertexArray(this->colorPickerSprite.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//the contrast slider
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderContrast.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		
		//draw the radius slider
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderRadius.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the height slider
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderHeight.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
	}
	
	//The particle panel
	if(this->state == GUIstate::PARTICLE)
	{
		//draws the browseble particle pictures
		glBindTexture(GL_TEXTURE_2D, this->particleImagesHandles[(this->activeParticleIndex-2)%this->particleImagesHandles.size()].getTextureHandle());
		glBindVertexArray(this->stp1.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp1.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->particleImagesHandles[(this->activeParticleIndex-1)%this->particleImagesHandles.size()].getTextureHandle());
		glBindVertexArray(this->stp2.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp2.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->particleImagesHandles[(this->activeParticleIndex)%this->particleImagesHandles.size()].getTextureHandle());
		glBindVertexArray(this->particleImagesHandles[2].getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->particleImagesHandles[2].getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->particleImagesHandles[(this->activeParticleIndex+1)%this->particleImagesHandles.size()].getTextureHandle());
		glBindVertexArray(this->stp3.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp3.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->particleImagesHandles[(this->activeParticleIndex+2)%this->particleImagesHandles.size()].getTextureHandle());
		glBindVertexArray(this->stp4.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp4.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		
		//draws the color choosed
		mat4 modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->colorPlaneSprite.getPosition());
		this->colorPlaneShader.use();
		this->colorPlaneShader.setUniform("modelMatrix",modelMatrix);
		glBindVertexArray(this->colorPlaneSprite.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		this->GUIshader.use();
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->particlePanel.getTextureHandle());
		glBindVertexArray(this->particlePanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",particlePanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the "x", the picker
		glBindTexture(GL_TEXTURE_2D, this->colorPickerSprite.getTextureHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderColorPicker.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glBindVertexArray(this->colorPickerSprite.getVaoHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//the contrast slider
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderContrast.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the height slider
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderHeight.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
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
		//displays the road textures
		glBindTexture(GL_TEXTURE_2D, surfaceTexHandles[(this->activeSurfaceTex-2)%(this->surfaceTexHandles.size())]);
		glBindVertexArray(this->stp1.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp1.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, surfaceTexHandles[(this->activeSurfaceTex-1)%(this->surfaceTexHandles.size())]);
		glBindVertexArray(this->stp2.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp2.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, surfaceTexHandles[(this->activeSurfaceTex)%(this->surfaceTexHandles.size())]);
		glBindVertexArray(this->mainTex.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->mainTex.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, surfaceTexHandles[(this->activeSurfaceTex+1)%(this->surfaceTexHandles.size())]);
		glBindVertexArray(this->stp3.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp3.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, surfaceTexHandles[(this->activeSurfaceTex+2)%(this->surfaceTexHandles.size())]);
		glBindVertexArray(this->stp4.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",this->stp4.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		//draws the panels in the right slot
		glBindTexture(GL_TEXTURE_2D, this->roadPanel.getTextureHandle());
		glBindVertexArray(this->roadPanel.getVaoHandle());
		this->GUIshader.setUniform("modelMatrix",roadPanel.getModelMatrix());
		glDrawArrays(GL_TRIANGLES,0,6);
		
		glBindTexture(GL_TEXTURE_2D, this->dragArrow.getTextureHandle());
		glBindVertexArray(this->dragArrow.getVaoHandle());
		mat4 modelMatrix;
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderRoadSpacing.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
		glDrawArrays(GL_TRIANGLES,0,6);
		modelMatrix=mat4(1.0f);
		modelMatrix*=translate(this->sliderRoadScale.getPosition());
		this->GUIshader.setUniform("modelMatrix",modelMatrix);
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
	this->activeParticle = Particle();
	this->activeParticle.setColor(this->normalizedColor);
	this->activeParticle.setParticleType(ParticleType::FIRE);
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
	this->selectRoad=false;
	this->isPlacingLights=false;
	this->placingParticleSystems=false;
}
float GUI::getSliderLightRadius()
{
	return this->sliderRadius.getSliderValueX()*5;
}
void GUI::moveSliders(float x, float y)
{
	if(this->state==GUIstate::ROAD)
	{
		if(this->sliderRoadSpacing.isInsideSliderSpace(x,y))
			this->sliderRoadSpacing.setPositionX(x);
		if(this->sliderRoadScale.isInsideSliderSpace(x,y))
			this->sliderRoadScale.setPositionX(x);
		
	}
	if(this->state==GUIstate::PAINT)
	{
		//move the sliders
		if(this->sliderSize.isInsideSliderSpace(x,y))
			this->sliderSize.setPositionX(x);

		if(this->sliderDropoff.isInsideSliderSpace(x,y))
			this->sliderDropoff.setPositionX(x);

		if(this->sliderOpacity.isInsideSliderSpace(x,y))
			this->sliderOpacity.setPositionX(x);
	}
	if(this->state==GUIstate::MODEL)
	{
		if(this->sliderScale.isInsideSliderSpace(x,y))
		{
			this->sliderScale.setPositionX(x);
		}
		if(this->sliderModelHeight.isInsideSliderSpace(x,y))
		{
			this->sliderModelHeight.setPositionX(x);
			this->activeModel.setPos(vec3(this->activeModel.getPos().x,this->sliderModelHeight.getSliderValueX(),this->activeModel.getPos().z));
		}
		
	}
	if(this->state==GUIstate::PARTICLE)
	{
		if(this->sliderContrast.isInsideSliderSpace(x,y))
		{
			this->sliderContrast.setPositionX(x);
			this->colorPlaneShader.use();
			float sliderValue= (this->sliderContrast.getSliderValueX()*2)-1;
			float r=0;
			float g=0;
			float b=0;
			if(sliderValue<0)
				r=std::max(this->normalizedColor.x+sliderValue,0.0f);
			else
				r=std::min(this->normalizedColor.x+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("ro",r);
			if(sliderValue<0)
				g=std::max(this->normalizedColor.y+sliderValue,0.0f);
			else
				g=std::min(this->normalizedColor.y+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("go",g);
			if(sliderValue<0)
				b=std::max(this->normalizedColor.z+sliderValue,0.0f);
			else
				b=std::min(this->normalizedColor.z+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("bo",b);
			glUseProgram(0);
			this->activeParticle.setContrast(1.0f);
			this->activeParticle.setColor(vec3(r,g,b));
		}
		
		if(this->sliderColorPicker.isInsideSliderSpace(x,y))
		{
			this->sliderColorPicker.setPositionX(x);
			this->sliderColorPicker.setPositionY(y);
			
			float xm = this->sliderColorPicker.getMaxX()-this->sliderColorPicker.getMinX();
			float ym = this->sliderColorPicker.getMaxY()-this->sliderColorPicker.getMinY();
			float xp = x-this->sliderColorPicker.getMinX();
			float yp = y-this->sliderColorPicker.getMinY();

			float colorCordX=xp*((this->colorScale.GetWidth()-1)/xm);
			float colorCordY=(this->colorScale.GetHeight()-1)-(yp*((this->colorScale.GetHeight()-1)/ym));
			sf::Color colorPicked = this->colorScale.GetPixel(colorCordX,colorCordY);
			
			this->normalizedColor.x = ((float)colorPicked.r/255);
			this->normalizedColor.y = ((float)colorPicked.g/255);
			this->normalizedColor.z = ((float)colorPicked.b/255);
			
			this->colorPlaneShader.use();
			float sliderValue= (this->sliderContrast.getSliderValueX()*2)-1;
			float r=0;
			float g=0;
			float b=0;
			if(sliderValue<0)
				r=std::max(this->normalizedColor.x+sliderValue,0.0f);
			else
				r=std::min(this->normalizedColor.x+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("ro",r);
			if(sliderValue<0)
				g=std::max(this->normalizedColor.y+sliderValue,0.0f);
			else
				g=std::min(this->normalizedColor.y+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("go",g);
			if(sliderValue<0)
				b=std::max(this->normalizedColor.z+sliderValue,0.0f);
			else
				b=std::min(this->normalizedColor.z+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("bo",b);
			glUseProgram(0);
			this->activeParticle.setColor(vec3(r,g,b));
		}
		if(this->sliderHeight.isInsideSliderSpace(x,y))
		{
			this->sliderHeight.setPositionX(x);
			this->activeParticle.setPos(vec3(this->activeParticle.getPos().x,this->sliderHeight.getSliderValueX()*10,this->activeParticle.getPos().z));
		}
	}
	if(this->state==GUIstate::LIGHT)
	{
		if(this->sliderContrast.isInsideSliderSpace(x,y))
		{
			this->sliderContrast.setPositionX(x);
			this->colorPlaneShader.use();
			float sliderValue= (this->sliderContrast.getSliderValueX()*2)-1;
			float r=0;
			float g=0;
			float b=0;
			if(sliderValue<0)
				r=std::max(this->normalizedColor.x+sliderValue,0.0f);
			else
				r=std::min(this->normalizedColor.x+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("ro",r);
			if(sliderValue<0)
				g=std::max(this->normalizedColor.y+sliderValue,0.0f);
			else
				g=std::min(this->normalizedColor.y+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("go",g);
			if(sliderValue<0)
				b=std::max(this->normalizedColor.z+sliderValue,0.0f);
			else
				b=std::min(this->normalizedColor.z+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("bo",b);
			glUseProgram(0);
			this->activeLight.setContrast(1.0f);
			this->activeLight.setColor(vec3(r,g,b));
		}
		if(this->sliderHeight.isInsideSliderSpace(x,y))
		{
			this->sliderHeight.setPositionX(x);
			this->activeLight.setPos(vec3(0,this->sliderHeight.getSliderValueX()*10,0));
		}
		if(this->sliderRadius.isInsideSliderSpace(x,y))
		{
			this->sliderRadius.setPositionX(x);
			this->activeLight.setRadius(this->sliderRadius.getSliderValueX()*5);
		}
			
		if(this->sliderColorPicker.isInsideSliderSpace(x,y))
		{
			this->sliderColorPicker.setPositionX(x);
			this->sliderColorPicker.setPositionY(y);
			
			float xm = this->sliderColorPicker.getMaxX()-this->sliderColorPicker.getMinX();
			float ym = this->sliderColorPicker.getMaxY()-this->sliderColorPicker.getMinY();
			float xp = x-this->sliderColorPicker.getMinX();
			float yp = y-this->sliderColorPicker.getMinY();

			float colorCordX=xp*((this->colorScale.GetWidth()-1)/xm);
			float colorCordY=(this->colorScale.GetHeight()-1)-(yp*((this->colorScale.GetHeight()-1)/ym));
			sf::Color colorPicked = this->colorScale.GetPixel(colorCordX,colorCordY);
			
			this->normalizedColor.x = ((float)colorPicked.r/255);
			this->normalizedColor.y = ((float)colorPicked.g/255);
			this->normalizedColor.z = ((float)colorPicked.b/255);
			
			this->colorPlaneShader.use();
			float sliderValue= (this->sliderContrast.getSliderValueX()*2)-1;
			float r=0;
			float g=0;
			float b=0;
			if(sliderValue<0)
				r=std::max(this->normalizedColor.x+sliderValue,0.0f);
			else
				r=std::min(this->normalizedColor.x+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("ro",r);
			if(sliderValue<0)
				g=std::max(this->normalizedColor.y+sliderValue,0.0f);
			else
				g=std::min(this->normalizedColor.y+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("go",g);
			if(sliderValue<0)
				b=std::max(this->normalizedColor.z+sliderValue,0.0f);
			else
				b=std::min(this->normalizedColor.z+sliderValue,1.0f);
			this->colorPlaneShader.setUniform("bo",b);
			glUseProgram(0);
			this->activeLight.setContrast(1.0f);
			this->activeLight.setColor(vec3(r,g,b));
		}
	}
}

void GUI::setLeftClick(float x, float y)
{	
	this->ans="";
	//when clicking on menus
	if(this->state==GUIstate::PAINT)
	{
		//for browsing textures, could be replaced with a button class
		if(this->inCircle(x,y, 0.46,0.04,0.03))
			this->incActiveTex();
		if(this->inCircle(x,y, 0.93,0.04,0.03))
			this->decActiveTex();
			
		//the most left tex plane
		if(this->inCircle(x,y, 0.523,0.05,0.045))
			this->activeTex=(this->activeTex+6)%8;
		//left
		if(this->inCircle(x,y, 0.625,0.05,0.045))
			this->activeTex=(this->activeTex+7)%8;
			
		//right
		if(this->inCircle(x,y, 0.77,0.05,0.045))
			this->activeTex=(this->activeTex+1)%8;
		//most right
		if(this->inCircle(x,y, 0.87,0.05,0.045))
			this->activeTex=(this->activeTex+2)%8;
			
	
	}
	if(this->state==GUIstate::MODEL)
	{
		//resets the height slider for the model, if the left arrow is pressed
		if(this->inCircle(x,y,0.518,-0.28,0.02))
		{
			this->sliderModelHeight.setPos(vec3(this->sliderModelHeight.getMinX(),this->sliderModelHeight.getPosition().y,this->sliderModelHeight.getPosition().z));
		}
		
		//if switching model (left)
		if(this->inCircle(x,y, 0.46,0.04,0.03))
		{
			this->activeModelIndex++;
			if(this->activeModelIndex>this->displayModels.size()-1)
				this->activeModelIndex=0;
			this->activeModel=this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()];
		}	
		//switching model (right)
		if(this->inCircle(x,y, 0.93,0.04,0.03))
		{
			this->activeModelIndex--;
			if(this->activeModelIndex<0)
				this->activeModelIndex=this->displayModels.size()-1;
			this->activeModel=this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()];
		}
		if(this->inCircle(x,y, 0.523,0.05,0.045))
		{
			this->activeModel=this->displayModels[abs(this->activeModelIndex)%this->displayModels.size()];
			this->activeModelIndex--;
			if(this->activeModelIndex<0)
				this->activeModelIndex=this->displayModels.size()-1;
			this->activeModelIndex--;
			if(this->activeModelIndex<0)
				this->activeModelIndex=this->displayModels.size()-1;
		}
		if(this->inCircle(x,y, 0.625,0.05,0.045))
		{
			this->activeModel=this->displayModels[abs(this->activeModelIndex+1)%this->displayModels.size()];
			this->activeModelIndex--;
			if(this->activeModelIndex<0)
				this->activeModelIndex=this->displayModels.size()-1;

		}
		
		if(this->inCircle(x,y, 0.77,0.05,0.045))
		{
			this->activeModel=this->displayModels[abs(this->activeModelIndex+3)%this->displayModels.size()];
			this->activeModelIndex++;
			if(this->activeModelIndex>this->displayModels.size()-1)
				this->activeModelIndex=0;
		}
		if(this->inCircle(x,y, 0.87,0.05,0.045))
		{
			this->activeModel=this->displayModels[abs(this->activeModelIndex+4)%this->displayModels.size()];
			this->activeModelIndex++;
			if(this->activeModelIndex>this->displayModels.size()-1)
				this->activeModelIndex=0;
			this->activeModelIndex++;
			if(this->activeModelIndex>this->displayModels.size()-1)
				this->activeModelIndex=0;
		}
		
		
		if(this->inCircle(x,y,0.84,-0.53,0.05))
		{
			this->sliderScale.resetPosition();
		}
		float rotAmount=22.5f;
		//checks for rotation x right
		if(this->inCircle(x,y,0.546,-0.863,0.02))
		{
			this->activeModel.rotateX(this->activeModel.getRot().x+rotAmount);
		}
		//left
		if(this->inCircle(x,y,0.562,-0.816,0.02))
		{
			this->activeModel.rotateX(this->activeModel.getRot().x-rotAmount);
		}
		//y right
		if(this->inCircle(x,y,0.695,-0.863,0.02))
		{
			this->activeModel.rotateY(this->activeModel.getRot().y+rotAmount);
		}
		//y left
		if(this->inCircle(x,y,0.714,-0.816,0.02))
		{
			this->activeModel.rotateY(this->activeModel.getRot().y-rotAmount);
		}
		//z right
		if(this->inCircle(x,y,0.839,-0.863,0.02))
		{
			this->activeModel.rotateZ(this->activeModel.getRot().z+rotAmount);
		}
		//z left
		if(this->inCircle(x,y,0.857,-0.816,0.02))
		{
			this->activeModel.rotateZ(this->activeModel.getRot().z-rotAmount);
		}
		//all the code below just scales the display window models
		float scaleX=this->displayModels[abs(this->activeModelIndex+3)%this->displayModels.size()].getBoundingBox()->getBboxSide().x;
		float scaleY=this->displayModels[abs(this->activeModelIndex+3)%this->displayModels.size()].getBoundingBox()->getBboxSide().y;
		float scaleZ=this->displayModels[abs(this->activeModelIndex+3)%this->displayModels.size()].getBoundingBox()->getBboxSide().z;
		float scale=scaleY;
		if(scaleX>scaleY)
			scale=scaleX;
		if(scaleZ>scaleX)
			scale=scaleZ;
		this->displayModels[abs(this->activeModelIndex+3)%this->displayModels.size()].scaleXYZ(0.04/(scale));
		
		
		scaleX=this->displayModels[abs(this->activeModelIndex+1)%this->displayModels.size()].getBoundingBox()->getBboxSide().x;
		scaleY=this->displayModels[abs(this->activeModelIndex+1)%this->displayModels.size()].getBoundingBox()->getBboxSide().y;
		scaleZ=this->displayModels[abs(this->activeModelIndex+1)%this->displayModels.size()].getBoundingBox()->getBboxSide().z;
		scale=scaleY;
		if(scaleX>scaleY)
			scale=scaleX;
		if(scaleZ>scaleX)
			scale=scaleZ;
		this->displayModels[abs(this->activeModelIndex+1)%this->displayModels.size()].scaleXYZ(0.04/(scale));
		
		
		scaleX=this->displayModels[abs(this->activeModelIndex)%this->displayModels.size()].getBoundingBox()->getBboxSide().x;
		scaleY=this->displayModels[abs(this->activeModelIndex)%this->displayModels.size()].getBoundingBox()->getBboxSide().y;
		scaleZ=this->displayModels[abs(this->activeModelIndex)%this->displayModels.size()].getBoundingBox()->getBboxSide().z;
		scale=scaleY;
		if(scaleX>scaleY)
			scale=scaleX;
		if(scaleZ>scaleX)
			scale=scaleZ;
		this->displayModels[abs(this->activeModelIndex)%this->displayModels.size()].scaleXYZ(0.04/(scale));
		
		scaleX=this->displayModels[abs(this->activeModelIndex+4)%this->displayModels.size()].getBoundingBox()->getBboxSide().x;
		scaleY=this->displayModels[abs(this->activeModelIndex+4)%this->displayModels.size()].getBoundingBox()->getBboxSide().y;
		scaleZ=this->displayModels[abs(this->activeModelIndex+4)%this->displayModels.size()].getBoundingBox()->getBboxSide().z;
		scale=scaleY;
		if(scaleX>scaleY)
			scale=scaleX;
		if(scaleZ>scaleX)
			scale=scaleZ;
		this->displayModels[abs(this->activeModelIndex+4)%this->displayModels.size()].scaleXYZ(0.04/(scale));
		
		
		
		scaleX=this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getBoundingBox()->getBboxSide().x;
		scaleY=this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getBoundingBox()->getBboxSide().y;
		scaleZ=this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].getBoundingBox()->getBboxSide().z;
		scale=scaleY;
		if(scaleX>scaleY)
			scale=scaleX;
		if(scaleZ>scaleX)
			scale=scaleZ;
		this->displayModels[abs(this->activeModelIndex+2)%this->displayModels.size()].scaleXYZ(0.16/(scale));
		this->activeModel.setPos(vec3(this->activeModel.getPos().x,this->sliderModelHeight.getSliderValueX(),this->activeModel.getPos().z));
	}
	if(this->state == GUIstate::PATH)
	{
		if(y>0.388&&y<0.472)
		{
			if(x>0.546&&x<0.687)
			{
				//create path
				this->ans="CRP";
				this->selectRoad=true;
			}
			if(x>0.712&&x<0.85)
			{
				//remove path
				this->ans="RMP";
				this->ans="DEL";
			}
		}
	}
	if(this->state == GUIstate::LIGHT)
	{
		if(this->inCircle(x,y,0.525,0.044,0.06))
		{
			this->activeLight.setLightType(LightType::AMBIENT);
			this->isPlacingLights=true;
			this->activeLight.rotateX(0);
			this->activeLight.rotateY(0);
			this->activeLight.rotateZ(0);
		}
		if(this->inCircle(x,y,0.628,0.044,0.06))
		{
			this->activeLight.setLightType(LightType::POINTLIGHT);
			this->isPlacingLights=true;
			this->activeLight.rotateX(0);
			this->activeLight.rotateY(0);
			this->activeLight.rotateZ(0);
		}

		if(this->inCircle(x,y,0.77,0.044,0.06))
		{
			this->activeLight.setLightType(LightType::POINTLIGHTSHADOW);
			this->isPlacingLights=true;
			this->activeLight.rotateX(0);
			this->activeLight.rotateY(0);
			this->activeLight.rotateZ(0);
		}

		if(this->inCircle(x,y,0.87,0.044,0.08))
		{
			this->activeLight.setLightType(LightType::SPOTLIGHT);
			this->isPlacingLights=true;
			this->activeLight.rotateX(0);
			this->activeLight.rotateY(0);
			this->activeLight.rotateZ(0);
		}
		float rotAmount=10;
		if(this->inCircle(x,y,0.54,-0.91,0.02))
		{
			this->activeLight.rotateX(this->activeLight.getRot().x+rotAmount);
		}
		if(this->inCircle(x,y,0.562,-0.872,0.02))
		{
			this->activeLight.rotateX(this->activeLight.getRot().x-rotAmount);
		}
		if(this->inCircle(x,y,0.695,-0.916,0.02))
		{
			this->activeLight.rotateY(this->activeLight.getRot().y+rotAmount);
		}
		if(this->inCircle(x,y,0.712,-0.872,0.02))
		{
			this->activeLight.rotateY(this->activeLight.getRot().y-rotAmount);
		}
		if(this->inCircle(x,y,0.837,-0.916,0.02))
		{
			this->activeLight.rotateZ(this->activeLight.getRot().z+rotAmount);
		}
		if(this->inCircle(x,y,0.853,-0.872,0.02))
		{
			this->activeLight.rotateZ(this->activeLight.getRot().z-rotAmount);
		}
	}
	if(this->state==GUIstate::PARTICLE)
	{
		float rotAmount=10;
		if(this->inCircle(x,y,0.678,-0.36,0.04))
			this->placingParticleSystems=true;
		if(this->inCircle(x,y,0.54,-0.91,0.02))
		{
			this->activeParticle.rotateX(this->activeParticle.getRot().x+rotAmount);
		}
		if(this->inCircle(x,y,0.562,-0.872,0.02))
		{
			this->activeParticle.rotateX(this->activeParticle.getRot().x-rotAmount);
		}
		if(this->inCircle(x,y,0.695,-0.916,0.02))
		{
			this->activeParticle.rotateY(this->activeParticle.getRot().y+rotAmount);
		}
		if(this->inCircle(x,y,0.712,-0.872,0.02))
		{
			this->activeParticle.rotateY(this->activeParticle.getRot().y-rotAmount);
		}
		if(this->inCircle(x,y,0.837,-0.916,0.02))
		{
			this->activeParticle.rotateZ(this->activeParticle.getRot().z+rotAmount);
		}
		if(this->inCircle(x,y,0.853,-0.872,0.02))
		{
			this->activeParticle.rotateZ(this->activeParticle.getRot().z-rotAmount);
		}
	}
	//when clicking on the top bar
	if(y>0.9)
	{
		//the position of the grid le button
		if(x>-0.75&&x<-0.7)
		{
			this->ans="GRID";
		}
		//exit
		if(x>-0.685&&x<-0.64)
		{
			cout<<"EXIT"<<endl;
		}
		//DELETE OBJECT
		if(x>-0.62&&x<-0.44)
		{
			this->ans = "DEL";
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
			//this->showNewMapSprite=false;
		}
		//if cancel
		if(x>-0.181&&x<-0.02&&y>-0.16&&y<-0.09)
		{
			this->ans="nmC";
			//this->showNewMapSprite=false;
		}
		if(this->inCircle(x,y,-0.31,0.01,0.07))
		{
			this->ans="nmCS";
		}
		if(this->inCircle(x,y,-0.196,0.01,0.07))
		{
			this->ans="nmCM";
		}
		if(this->inCircle(x,y,-0.082,0.01,0.07))
		{
			this->ans="nmCL";
		}
	}
	
	//if show load sprite is up
	if(showLoadMapSprite)
	{
		if(x>-0.39&&x<-0.232&&y>-0.088&&y<-0.033)
		{
			this->ans="lmOK";
			//this->showLoadMapSprite=false;
			this->textMode=false;
		}
		//if cancel
		if(x>-0.2&&x<-0.045&&y>-0.09&&y<-0.02)
		{
			this->ans="lmC";
			//this->showLoadMapSprite=false;
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
	
	if(this->state==GUIstate::PARTICLE)
	{
		if(this->inCircle(x,y, 0.57,0.76,0.03))
		{
			this->activeParticleIndex+=this->particleImagesHandles.size()-1;
		}
		if(this->inCircle(x,y, 0.57,0.236,0.03))
			this->activeParticleIndex++;
			
		//the most left tex plane
		if(this->inCircle(x,y, 0.523,0.05,0.045))
			this->activeParticleIndex+=this->particleImagesHandles.size()-2;
		//left
		if(this->inCircle(x,y, 0.625,0.05,0.045))
			this->activeParticleIndex+=this->particleImagesHandles.size()-1;
			
		//right
		if(this->inCircle(x,y, 0.77,0.05,0.045))
			this->activeParticleIndex=this->activeParticleIndex+1;
		//most right
		if(this->inCircle(x,y, 0.87,0.05,0.045))
			this->activeParticleIndex=this->activeParticleIndex+2;
		
		//so the activeSurfaceTex wont overflow the storage of int
		if(this->activeParticleIndex>60000)
			this->activeParticleIndex=2;
	}

	if(this->state==GUIstate::ROAD)
	{
		if(this->inCircle(x,y, 0.46,0.04,0.03))
		{
			this->activeSurfaceTex+=this->surfaceTexHandles.size()-1;
		}
		if(this->inCircle(x,y, 0.93,0.04,0.03))
			this->activeSurfaceTex++;
			
		//the most left tex plane
		if(this->inCircle(x,y, 0.523,0.05,0.045))
			this->activeSurfaceTex+=this->surfaceTexHandles.size()-2;
		//left
		if(this->inCircle(x,y, 0.625,0.05,0.045))
			this->activeSurfaceTex+=this->surfaceTexHandles.size()-1;
			
		//right
		if(this->inCircle(x,y, 0.77,0.05,0.045))
			this->activeSurfaceTex=this->activeSurfaceTex+1;
		//most right
		if(this->inCircle(x,y, 0.87,0.05,0.045))
			this->activeSurfaceTex=this->activeSurfaceTex+2;
		
		//so the activeSurfaceTex wont overflow the storage of int
		if(this->activeSurfaceTex>60000)
			this->activeSurfaceTex=2;
			
		if(x>0.63 && x < 0.77 && y>-0.25&&y<-0.177)
		{
			if(this->inCircle(x,y, 0.93,0.04,0.03))
				this->decActiveTex();
			
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
		if(this->inCircle(ax,ay,-0.134,0,0.0666))
		{
			this->state=GUIstate::LIGHT;
		}
		if(this->inCircle(ax,ay,+0.134,0.07,0.07))
		{
			this->state=GUIstate::MODEL;
			//resets the rotation for the active model when model panel is brought up
			this->activeModel.rotateX(0);
			this->activeModel.rotateY(0);
			this->activeModel.rotateZ(0);
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
void GUI::hideLoadMapDialog()
{
	this->showLoadMapSprite=false;
}
void GUI::hideSaveMapDialog()
{
	showSaveMapSprite=false;
}
bool GUI::isSaveMapDialogUp()
{
	return showSaveMapSprite;
}
void GUI::hideNewMapDialog()
{
	showNewMapSprite=false;
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
	if(this->displayModels.size()==5)
	{
		scaleX=this->displayModels[4].getBoundingBox()->getBboxSide().x;
		scaleY=this->displayModels[4].getBoundingBox()->getBboxSide().y;
		scaleZ=this->displayModels[4].getBoundingBox()->getBboxSide().z;
		scale=scaleY;
		if(scaleX>scaleY)
			scale=scaleX;
		if(scaleZ>scaleX)
			scale=scaleZ;
		this->displayModels[4].scaleXYZ(0.16/(scale));
		{
			this->activeModel=this->displayModels[4];
		}
	}

}
void GUI::setSurfaceTexHandles(vector<GLuint> th)
{
	this->surfaceTexHandles = th;
}
void GUI::resetDialogAns()
{
	this->ans="";
}
Model GUI::getActiveModel()
{
	this->activeModel.scaleXYZ(this->sliderScale.getSliderValueX()*2);
	return this->activeModel;
}
void GUI::rotateActiveModelLeft(float f)
{
	this->activeModel.rotateY(this->activeModel.getRot().y+f);
}
void GUI::rotateActiveModelRight(float f)
{
	this->activeModel.rotateY(this->activeModel.getRot().y-f);
}

bool GUI::isSelectingRoad()
{
	return this->selectRoad;
}
vec3 GUI::getNormalizedColor()
{
	return this->normalizedColor;
}

float GUI::getContrast()
{
	return this->sliderContrast.getSliderValueX();
}

void GUI::setSliderLightRadius(float s)
{
	this->sliderRadius.setPos(vec3(this->sliderRadius.getMinX()+(this->sliderRadius.getMaxX()-this->sliderRadius.getMinX())*(s/5),this->sliderRadius.getPosition().y,0));
}

void GUI::setNormalizedColor(vec3 col,float contrast)
{
	this->sliderContrast.setPos(vec3(this->sliderContrast.getMinX()+(this->sliderContrast.getMaxX()-this->sliderContrast.getMinX())*contrast,this->sliderContrast.getPosition().y,0));
	this->normalizedColor=col;
	this->colorPlaneShader.use();
	this->colorPlaneShader.setUniform("ro",col.x*contrast);
	this->colorPlaneShader.setUniform("go",col.y*contrast);
	this->colorPlaneShader.setUniform("bo",col.z*contrast);
}
Light GUI::getActiveLight()
{
	return this->activeLight;
}
void GUI::setActiveLightModel(Light l)
{
	this->activeLight=l;
	this->sliderHeight.setPos(vec3(this->sliderHeight.getMinX()+(this->sliderHeight.getMaxX()-this->sliderHeight.getMinX())*(l.getPos().y/10),this->sliderHeight.getPosition().y,0));
}
bool GUI::isPlacingLightMode()
{
	return this->isPlacingLights;
}
void GUI::raiseActiveModel(float f)
{
	this->activeModel.setPos(vec3(this->activeModel.getPos().x,this->activeModel.getPos().y+f,this->activeModel.getPos().z));
}
void GUI::lowerActiveModel(float f)
{
	this->activeModel.setPos(vec3(this->activeModel.getPos().x,this->activeModel.getPos().y-f,this->activeModel.getPos().z));
}
void GUI::drawSplashScreen()
{
	glDisable(GL_DEPTH_TEST);
	this->GUIshader.use();

	glBindTexture(GL_TEXTURE_2D, this->splashScreen.getTextureHandle());
	glBindVertexArray(this->splashScreen.getVaoHandle());
	this->GUIshader.setUniform("modelMatrix",splashScreen.getModelMatrix());
	glDrawArrays(GL_TRIANGLES,0,6);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glBindVertexArray(0);
}
int GUI::getActiveSurfaceTexHandle()
{
	return (this->activeSurfaceTex)%(this->surfaceTexHandles.size());
}

float GUI::getRoadSliderSpacing()
{
	return this->sliderRoadSpacing.getSliderValueX();
}
float GUI::getRoadSliderScale()
{
	return 1+(this->sliderRoadScale.getSliderValueX()*5);
}
Particle GUI::getActiveParticleModel()
{
	this->activeParticle.setParticleType(this->particles[(this->activeParticleIndex)%this->particleImagesHandles.size()].getParticleType());
	return this->activeParticle;
}
bool GUI::isPlacingParticleSystems()
{
	return this->placingParticleSystems;
}
void GUI::setActiveParticleModel(Particle p)
{
	this->activeParticle=p;
	this->activeParticleIndex = p.getParticleType()+this->particleImagesHandles.size();

	this->sliderHeight.setPos(vec3(this->sliderHeight.getMinX()+(this->sliderHeight.getMaxX()-this->sliderHeight.getMinX())*(p.getPos().y/10),this->sliderHeight.getPosition().y,0));
	
	//this->sliderContrast.setPos(vec3(this->sliderContrast.getMinX()+(this->sliderContrast.getMaxX()-this->sliderContrast.getMinX())*p.getContrast(),this->sliderContrast.getPosition().y,0));
	this->normalizedColor=p.getColor();
	this->colorPlaneShader.use();
	this->colorPlaneShader.setUniform("ro",p.getColor().x);
	this->colorPlaneShader.setUniform("go",p.getColor().y);
	this->colorPlaneShader.setUniform("bo",p.getColor().z);
	glUseProgram(0);
}