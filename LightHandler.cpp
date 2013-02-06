#include "LightHandler.h"

LightHandler::LightHandler()
{
}

LightHandler::~LightHandler()
{
}

void LightHandler::init()
{
	this->lightShader.compileShaderFromFile("path.vsh",GLSLShader::VERTEX);
	this->lightShader.compileShaderFromFile("path.fsh",GLSLShader::FRAGMENT);
	this->lightShader.bindAttribLocation(0,"vertexPosition");
	this->lightShader.bindAttribLocation(1,"vertexNormal");
	//cout<<this->lightShader.log();
	this->lightShader.link();
	//cout<<this->lightShader.log();
	this->meshes.load("./models/lights/");
	this->spotLightMeshPos = this->meshes.getMeshByName("lightSpot");
	this->pointLightMeshPos = this->meshes.getMeshByName("lightPoint");
	this->ambientLightMeshPos = this->meshes.getMeshByName("lightAmbient");
	
	this->bBoxShader.compileShaderFromFile("bbox.vsh",GLSLShader::VERTEX);
	this->bBoxShader.compileShaderFromFile("bbox.fsh",GLSLShader::FRAGMENT);
	this->bBoxShader.bindAttribLocation(3,"bBoxCordsVertex");
	
	this->bBoxShader.link();
	
	this->radRing.init();
}

void LightHandler::free()
{
	this->radRing.free();
}


void LightHandler::addLight(Light l)
{
	Light t = l;
	if(t.getLightType() == LightType::POINTLIGHTSHADOW||t.getLightType() == LightType::POINTLIGHT)
	{
		t.setMesh(this->meshes.getMeshInfo(this->pointLightMeshPos));
		t.setBoundingBox(this->meshes.getBoundingBox(this->pointLightMeshPos));
	}
	if(t.getLightType() == LightType::SPOTLIGHT)
	{
		t.setMesh(this->meshes.getMeshInfo(this->spotLightMeshPos));
		t.setBoundingBox(this->meshes.getBoundingBox(this->spotLightMeshPos));
	}
	if(t.getLightType() == LightType::AMBIENT)
	{
		t.setMesh(this->meshes.getMeshInfo(this->ambientLightMeshPos));
		t.setBoundingBox(this->meshes.getBoundingBox(this->ambientLightMeshPos));
	}
	this->lights.push_back(t);
}
void LightHandler::drawLights(mat4 projectionMatrix,mat4 viewMatrix)
{
	this->lightShader.use();
	this->lightShader.setUniform("outAlpha",1.0f);
	mat4 mvp;
	for(unsigned int i=0;i<this->lights.size();i++)
	{
		this->lightShader.setUniform("ro",this->lights[i].getColor().x*this->lights[i].getContrast());
		this->lightShader.setUniform("go",this->lights[i].getColor().y*this->lights[i].getContrast());
		this->lightShader.setUniform("bo",this->lights[i].getColor().z*this->lights[i].getContrast());
		//set upp uniforms for rendering call
		mvp=projectionMatrix*viewMatrix*this->lights[i].getModelMatrix();
		this->lightShader.setUniform("MVP",mvp);
		mat3 normalMatrix=transpose(inverse(mat3(viewMatrix*this->lights[i].getModelMatrix())));
		this->lightShader.setUniform("normalMatrix",normalMatrix);
		
		//get a pointer to a vector with meshes info for each mesh
		for(unsigned int j=0;j<this->lights[i].getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->lights[i].getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->lights[i].getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->lights[i].getMeshInfo()->at(j).getNrOfVerts());
		}

	}
	this->bBoxShader.use();
	this->bBoxShader.setUniform("ro",0.0f);
	this->bBoxShader.setUniform("go",1.0f);
	this->bBoxShader.setUniform("bo",0.0f);
	//renders bounding box
	glLineWidth(2);
	
	for(unsigned int i=0;i<this->lights.size();i++)
	{
		if(this->lights[i].isSelected())
		{
			//set upp uniforms for rendering call
			mvp=projectionMatrix*viewMatrix*this->lights[i].getModelMatrix();
			this->bBoxShader.setUniform("MVP",mvp);
			
			glBindVertexArray(this->lights[i].getBoundingBox()->getVaoh());
			glDrawArrays(GL_LINE_STRIP,0,16);
			
			if(this->lights[i].getLightType()==LightType::POINTLIGHT || this->lights[i].getLightType()==LightType::POINTLIGHTSHADOW)
			{
				mvp=scale(mvp,vec3(this->lights[i].getRadius()));
				this->bBoxShader.setUniform("MVP",mvp);
				glBindVertexArray(this->radRing.getVaoh());
				glDrawArrays(GL_LINE_STRIP,0,this->radRing.getNrOfLines());
				
				mvp*=rotate(90.0f,glm::vec3(0.0f,0.0f,1.0f));
				this->bBoxShader.setUniform("MVP",mvp);
				this->bBoxShader.setUniform("ro",1.0f);
				this->bBoxShader.setUniform("go",0.0f);
				this->bBoxShader.setUniform("bo",0.0f);
				glDrawArrays(GL_LINE_STRIP,0,this->radRing.getNrOfLines());
				
				mvp*=rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));
				this->bBoxShader.setUniform("MVP",mvp);
				this->bBoxShader.setUniform("ro",0.0f);
				this->bBoxShader.setUniform("go",0.0f);
				this->bBoxShader.setUniform("bo",1.0f);
				glDrawArrays(GL_LINE_STRIP,0,this->radRing.getNrOfLines());
			}
		}
	}
	
	glUseProgram(0);
	glBindVertexArray(0);
}
void LightHandler::removeLightsBoundToModels(vector<Model> models)
{
	vector<int> modelIds;
	for(unsigned int i=0;i<models.size();i++)
	{
		if(models[i].getId()>0)
			modelIds.push_back(models[i].getId());
	}
	for(unsigned int i=0;i<this->lights.size();i++)
	{
		for(unsigned int j=0;j<modelIds.size();j++)
		{
			if(modelIds[j]==this->lights[i].getId())
			{
				this->lights[i]=this->lights[this->lights.size()-1];
				this->lights.pop_back();
				i--;
			}
		}
	}
}

int LightHandler::selectLight(float normalizedX, float normalizedY,vec3 pos, mat4 projMatrix,mat4 viewMatrix)
{
	float min=999999.0f;
	int minIndex=-1;
	for(unsigned int j=0;j<this->lights.size();j++)
	{
		this->lights[j].unSelect();
		bool found=true;
		float t1=0;
		float t2=0;
		mat4 unview = inverse(projMatrix*viewMatrix);
		vec4 near_point = unview * vec4(normalizedX, normalizedY, 0, 1);

		vec4 mm = inverse(mat4(lights[j].getModelMatrix()))*vec4(pos,1);
		

		vec3 t = vec3(near_point.x/near_point.w,near_point.y/near_point.w,near_point.z/near_point.w)-pos;
		vec3 rayd=inverse(mat3(lights[j].getModelMatrix()))*t;
		rayd = normalize(rayd);


		float sides[3] =
		{
			this->lights[j].getBoundingBox()->getBboxSide().x,
			this->lights[j].getBoundingBox()->getBboxSide().y,
			this->lights[j].getBoundingBox()->getBboxSide().z
		};
		
		vec3 normalizedSides[3] = 
		{
			vec3(1,0,0),
			vec3(0,1,0),
			vec3(0,0,1)
		};
		
		float tmin=-0.000001;
		float tmax=99999999;
		

		vec3 p = this->lights[j].getBoundingBox()->getBboxPos()-vec3(mm.x/mm.w,mm.y/mm.w,mm.z/mm.w);
		
		for(int i=0;i<3;i++)
		{
			float e = dot(normalizedSides[i],p);
			float f = dot(normalizedSides[i],rayd);
			if(abs(f)>0.000001)
			{
				t1=(e+sides[i])/f;
				t2=(e-sides[i])/f;
				if(t1>t2)
				{
					float tmp=t1;
					t1=t2;
					t2=tmp;
				}
				if(t1>tmin)
					tmin=t1;
				if(t2<tmax)
					tmax=t2;
				if(tmin>tmax)
					found=false;
				if(tmax<0)
					found=false;
			}
			else if(-e-sides[i]>0 || -e+sides[i]<0)
				found=false;
		}
		if(found)
		{
			if(tmin>0)
			{
				if(tmin<min)
				{
					min = tmin;
					minIndex=j;
				}
			}
		}
	}
	if(minIndex>=0)
	{
			this->lights[minIndex].select();
		return minIndex;
	}
	return -1;	
}

void LightHandler::removeSelectedLights()
{
	for(unsigned int i=0;i<this->lights.size();i++)
	{
		if(this->lights[i].isSelected())
		{
			this->lights[i]=this->lights[this->lights.size()-1];
			this->lights.pop_back();
			i--;
		}
	}
}
void LightHandler::deselectAllLights()
{
	for(unsigned int i=0;i<this->lights.size();i++)
	{
		this->lights[i].unSelect();
	}
}

Light LightHandler::getSelectedLight()
{
	bool found=false;
	Light tmp;
	for(unsigned int i=0;i<this->lights.size()&&!found;i++)
	{
		if(this->lights[i].isSelected())
		{
			found=true;
			tmp = this->lights[i];
		}
	}
	return tmp;
}
int LightHandler::getSelectedLightIndex()
{
	bool found=false;
	int index=-1;
	for(unsigned int i=0;i<this->lights.size()&&!found;i++)
	{
		if(this->lights[i].isSelected())
		{
			found=true;
			index=i;
		}
	}
	return index;
}

void LightHandler::save(string path, string filename)
{
	fstream file;
	string p = path+filename+".txt";
	file.open(p.c_str(),fstream::out|fstream::app);
	file << "LIGHTS: name, pos x,y,z, rot x,y,z, color, radius"<<endl;
	for(unsigned int i = 0; i< this->lights.size();i++)
	{
		if(this->lights[i].getLightType() == LightType::POINTLIGHTSHADOW)
		{
			file << "PLS"<<" " << this->lights[i].getPos().x << " " << this->lights[i].getPos().y << " " << this->lights[i].getPos().z 
			<< " " <<this->lights[i].getRot().x << " " << this->lights[i].getRot().y <<" " << this->lights[i].getRot().z<< " "
			<< this->lights[i].getColor().x << " " <<this->lights[i].getColor().y << " " << this->lights[i].getColor().z << " " <<
			this->lights[i].getRadius()<<endl;
		}
		if(this->lights[i].getLightType() == LightType::POINTLIGHT)
		{
			file << "PL"<<" " << this->lights[i].getPos().x << " " << this->lights[i].getPos().y << " " << this->lights[i].getPos().z 
			<< " " <<this->lights[i].getRot().x << " " << this->lights[i].getRot().y <<" " << this->lights[i].getRot().z<< " "
			<< this->lights[i].getColor().x << " " <<this->lights[i].getColor().y << " " << this->lights[i].getColor().z << " " <<
			this->lights[i].getRadius()<<endl;
		}
		if(this->lights[i].getLightType() == LightType::AMBIENT)
		{
			vec3 dir=vec3(0.0,-1.0,0.0);
			mat4 rot=mat4(1.0f);
			rot*=rotate(this->lights[i].getRot().x,vec3(1.0,0.0,0.0));
			rot*=rotate(this->lights[i].getRot().y,vec3(0.0,1.0,0.0));
			rot*=rotate(this->lights[i].getRot().z,vec3(0.0,0.0,1.0));
			dir=mat3(rot)*dir;

			file << "AM"<<" " << " " <<dir.x << " " << dir.y <<" " << dir.z<< " "
			<< this->lights[i].getColor().x << " " <<this->lights[i].getColor().y << " " << this->lights[i].getColor().z<<endl;
		}
		if(this->lights[i].getLightType() == LightType::SPOTLIGHT)
		{
			vec3 dir=vec3(0.0,-1.0,0.0);
			mat4 rot=mat4(1.0f);
			rot*=rotate(this->lights[i].getRot().x,vec3(1.0,0.0,0.0));
			rot*=rotate(this->lights[i].getRot().y,vec3(0.0,1.0,0.0));
			rot*=rotate(this->lights[i].getRot().z,vec3(0.0,0.0,1.0));
			dir=mat3(rot)*dir;
			
			file << "SL " << this->lights[i].getPos().x << " " << this->lights[i].getPos().y << " " << this->lights[i].getPos().z <<
			" " <<dir.x << " " << dir.y <<" " << dir.z<< " "
			<< this->lights[i].getColor().x << " " <<this->lights[i].getColor().y << " " << this->lights[i].getColor().z<<endl;
		}
	}
	file << "end"<<endl;
	file.close();
}

void LightHandler::drawLight(mat4 projectionMatrix, mat4 viewMatrix,Light l)
{
	glEnable(GL_BLEND);
	Light light=l;
	
	int bBoxpos=0;
	
	if(light.getLightType()==LightType::POINTLIGHTSHADOW||light.getLightType()==LightType::POINTLIGHT)
	{
		bBoxpos=this->pointLightMeshPos;
		light.setMesh(this->meshes.getMeshInfo(this->pointLightMeshPos));
	}
		
	if(light.getLightType()==LightType::SPOTLIGHT)
	{
		bBoxpos=this->spotLightMeshPos;
		light.setMesh(this->meshes.getMeshInfo(this->spotLightMeshPos));
	}
	
	if(light.getLightType()==LightType::AMBIENT)
	{
		bBoxpos=this->ambientLightMeshPos;
		light.setMesh(this->meshes.getMeshInfo(this->ambientLightMeshPos));
	}
	
	this->lightShader.use();
	this->lightShader.setUniform("outAlpha",0.5f);
	mat4 mvp;
	
	this->lightShader.setUniform("ro",light.getColor().x*light.getContrast());
	this->lightShader.setUniform("go",light.getColor().y*light.getContrast());
	this->lightShader.setUniform("bo",light.getColor().z*light.getContrast());
	//set upp uniforms for rendering call
	mvp=projectionMatrix*viewMatrix*light.getModelMatrix();
	this->lightShader.setUniform("MVP",mvp);
	mat3 normalMatrix=transpose(inverse(mat3(viewMatrix*light.getModelMatrix())));
	this->lightShader.setUniform("normalMatrix",normalMatrix);
	
	//get a pointer to a vector with meshes info for each mesh
	for(unsigned int j=0;j<light.getMeshInfo()->size();j++)
	{
		glBindTexture(GL_TEXTURE_2D, light.getMeshInfo()->at(j).getTexh());
		glBindVertexArray(light.getMeshInfo()->at(j).getVaoh());
		glDrawArrays(GL_TRIANGLES,0,light.getMeshInfo()->at(j).getNrOfVerts());
	}

	this->lightShader.setUniform("outAlpha",1.0f);
	

		this->bBoxShader.use();
		this->bBoxShader.setUniform("ro",0.0f);
		this->bBoxShader.setUniform("go",1.0f);
		this->bBoxShader.setUniform("bo",0.0f);
		//renders bounding box
		glLineWidth(2);
		
		if(light.isSelected())
		{
			//set upp uniforms for rendering call
			mvp=projectionMatrix*viewMatrix*light.getModelMatrix();
			this->bBoxShader.setUniform("MVP",mvp);
			
			glBindVertexArray(this->meshes.getBoundingBox(bBoxpos)->getVaoh());
			glDrawArrays(GL_LINE_STRIP,0,16);
			
			if(light.getLightType()==LightType::POINTLIGHTSHADOW||light.getLightType()==LightType::POINTLIGHT)
			{
				mvp=scale(mvp,vec3(light.getRadius()));
				this->bBoxShader.setUniform("MVP",mvp);
				glBindVertexArray(this->radRing.getVaoh());
				glDrawArrays(GL_LINE_STRIP,0,this->radRing.getNrOfLines());
				
				mvp*=rotate(90.0f,glm::vec3(0.0f,0.0f,1.0f));
				this->bBoxShader.setUniform("MVP",mvp);
				this->bBoxShader.setUniform("ro",1.0f);
				this->bBoxShader.setUniform("go",0.0f);
				this->bBoxShader.setUniform("bo",0.0f);
				glDrawArrays(GL_LINE_STRIP,0,this->radRing.getNrOfLines());
				
				mvp*=rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));
				this->bBoxShader.setUniform("MVP",mvp);
				this->bBoxShader.setUniform("ro",0.0f);
				this->bBoxShader.setUniform("go",0.0f);
				this->bBoxShader.setUniform("bo",1.0f);
				glDrawArrays(GL_LINE_STRIP,0,this->radRing.getNrOfLines());
			}
		}
	
	glUseProgram(0);
	glBindVertexArray(0);
	
	glDisable(GL_BLEND);
}

void LightHandler::assignLightAnotherLight(int pos, Light l)
{
	if(pos>=0&&pos<this->lights.size())
	{
		this->lights[pos].setColor(l.getColor());
		this->lights[pos].setContrast(l.getContrast());
		this->lights[pos].setRadius(l.getRadius());
		this->lights[pos].setPos(vec3(this->lights[pos].getPos().x,l.getPos().y,this->lights[pos].getPos().z));
		this->lights[pos].rotateX(l.getRot().x);
		this->lights[pos].rotateY(l.getRot().y);
		this->lights[pos].rotateZ(l.getRot().z);
	}
}
