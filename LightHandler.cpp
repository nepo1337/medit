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
	this->spotLightMeshPos = this->meshes.getMeshByName("lightPoint");
	
	this->bBoxShader.compileShaderFromFile("bbox.vsh",GLSLShader::VERTEX);
	this->bBoxShader.compileShaderFromFile("bbox.fsh",GLSLShader::FRAGMENT);
	this->bBoxShader.bindAttribLocation(3,"bBoxCordsVertex");
	
	this->bBoxShader.link();
}

void LightHandler::addPointLight(vec3 pos, vec3 color, float radius,int id)
{
	Light l;
	l.bindId(id);
	l.setPos(pos);
	l.setColor(color);
	l.setRadius(radius);
	l.setMesh(this->meshes.getMeshInfo(this->spotLightMeshPos));
	this->pointLights.push_back(l);
}
void LightHandler::drawLights(mat4 projectionMatrix,mat4 viewMatrix)
{
	this->lightShader.use();
	mat4 mvp;
	for(unsigned int i=0;i<this->pointLights.size();i++)
	{
		this->lightShader.setUniform("ro",this->pointLights[i].getColor().x);
		this->lightShader.setUniform("go",this->pointLights[i].getColor().y);
		this->lightShader.setUniform("bo",this->pointLights[i].getColor().z);
		//set upp uniforms for rendering call
		mvp=projectionMatrix*viewMatrix*this->pointLights[i].getModelMatrix();
		this->lightShader.setUniform("MVP",mvp);
		mat3 normalMatrix=transpose(inverse(mat3(viewMatrix*this->pointLights[i].getModelMatrix())));
		this->lightShader.setUniform("normalMatrix",normalMatrix);
		
		//get a pointer to a vector with meshes info for each mesh
		for(unsigned int j=0;j<this->pointLights[i].getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->pointLights[i].getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->pointLights[i].getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->pointLights[i].getMeshInfo()->at(j).getNrOfVerts());
		}

	}
	this->bBoxShader.use();
	this->bBoxShader.setUniform("ro",0.0f);
	this->bBoxShader.setUniform("go",1.0f);
	this->bBoxShader.setUniform("bo",0.0f);
	//renders bounding box
	glLineWidth(2);
	
	for(unsigned int i=0;i<this->pointLights.size();i++)
	{
		if(this->pointLights[i].isSelected())
		{
			//set upp uniforms for rendering call
			mvp=projectionMatrix*viewMatrix*this->pointLights[i].getModelMatrix();
			this->lightShader.setUniform("MVP",mvp);
			
			glBindVertexArray(this->meshes.getBoundingBox(spotLightMeshPos)->getVaoh());
			glDrawArrays(GL_LINE_STRIP,0,16);
			
		}
	}
	
	glUseProgram(0);
	glBindVertexArray(0);
}
void LightHandler::removeLightsBoundToModels(vector<Model> models)
{
	vector<int> modelIds;
	for(int i=0;i<models.size();i++)
	{
		if(models[i].getId()>0)
			modelIds.push_back(models[i].getId());
	}
	for(int i=0;i<this->pointLights.size();i++)
	{
		for(int j=0;j<modelIds.size();j++)
		{
			if(modelIds[j]==this->pointLights[i].getId())
			{
				this->pointLights[i]=this->pointLights[this->pointLights.size()-1];
				this->pointLights.pop_back();
				i--;
			}
		}
	}
}

int LightHandler::selectLight(float normalizedX, float normalizedY,vec3 pos, mat4 projMatrix,mat4 viewMatrix)
{
	float min=999999.0f;
	int minIndex=-1;
	for(unsigned int j=0;j<this->pointLights.size();j++)
	{
		this->pointLights[j].unSelect();
		bool found=true;
		float t1=0;
		float t2=0;
		mat4 unview = inverse(projMatrix*viewMatrix);
		vec4 near_point = unview * vec4(normalizedX, normalizedY, 0, 1);

		vec4 mm = inverse(mat4(pointLights[j].getModelMatrix()))*vec4(pos,1);
		

		vec3 t = vec3(near_point.x/near_point.w,near_point.y/near_point.w,near_point.z/near_point.w)-pos;
		vec3 rayd=inverse(mat3(pointLights[j].getModelMatrix()))*t;
		rayd = normalize(rayd);


		float sides[3] =
		{
			this->meshes.getBoundingBox(spotLightMeshPos)->getBboxSide().x,
			this->meshes.getBoundingBox(spotLightMeshPos)->getBboxSide().y,
			this->meshes.getBoundingBox(spotLightMeshPos)->getBboxSide().z
		};
		
		vec3 normalizedSides[3] = 
		{
			vec3(1,0,0),
			vec3(0,1,0),
			vec3(0,0,1)
		};
		
		float tmin=-0.000001;
		float tmax=99999999;
		

		vec3 p = this->meshes.getBoundingBox(spotLightMeshPos)->getBboxPos()-vec3(mm.x/mm.w,mm.y/mm.w,mm.z/mm.w);
		
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
			this->pointLights[minIndex].select();
		return minIndex;
	}
	return -1;	
}

void LightHandler::removeSelectedLights()
{
	for(int i=0;i<this->pointLights.size();i++)
	{
		if(this->pointLights[i].isSelected())
		{
			this->pointLights[i]=this->pointLights[this->pointLights.size()-1];
			this->pointLights.pop_back();
			i--;
		}
	}
}