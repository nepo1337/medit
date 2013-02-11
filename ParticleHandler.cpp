#include "ParticleHandler.h"

ParticleHandler::ParticleHandler()
{
	this->selectedParticleIndex=-1;
}

ParticleHandler::~ParticleHandler()
{
}

void ParticleHandler::init()
{
	this->particleShader.compileShaderFromFile("path.vsh",GLSLShader::VERTEX);
	this->particleShader.compileShaderFromFile("path.fsh",GLSLShader::FRAGMENT);
	this->particleShader.bindAttribLocation(0,"vertexPosition");
	this->particleShader.bindAttribLocation(1,"vertexNormal");

	this->particleShader.link();

	this->meshes.load("./models/particles/");
	
	this->bBoxShader.compileShaderFromFile("bbox.vsh",GLSLShader::VERTEX);
	this->bBoxShader.compileShaderFromFile("bbox.fsh",GLSLShader::FRAGMENT);
	this->bBoxShader.bindAttribLocation(3,"bBoxCordsVertex");
	
	this->bBoxShader.link();
}

void ParticleHandler::drawParticleModel(mat4 projectionMatrix, mat4 viewMatrix,Particle p)
{
	Particle particle=p;
	
	particle.setMesh(this->meshes.getMeshInfo(0));
	particle.setBoundingBox(this->meshes.getBoundingBox(0));
	
	this->particleShader.use();
	this->particleShader.setUniform("outAlpha",0.7f);
	mat4 mvp;
	
	this->particleShader.setUniform("ro",particle.getColor().x*particle.getContrast());
	this->particleShader.setUniform("go",particle.getColor().y*particle.getContrast());
	this->particleShader.setUniform("bo",particle.getColor().z*particle.getContrast());
	//set upp uniforms for rendering call
	mvp=projectionMatrix*viewMatrix*particle.getModelMatrix();
	this->particleShader.setUniform("MVP",mvp);
	mat3 normalMatrix=transpose(inverse(mat3(viewMatrix*particle.getModelMatrix())));
	this->particleShader.setUniform("normalMatrix",normalMatrix);
	
	//get a pointer to a vector with meshes info for each mesh
	for(unsigned int j=0;j<particle.getMeshInfo()->size();j++)
	{
		glBindTexture(GL_TEXTURE_2D, particle.getMeshInfo()->at(j).getTexh());
		glBindVertexArray(particle.getMeshInfo()->at(j).getVaoh());
		glDrawArrays(GL_TRIANGLES,0,particle.getMeshInfo()->at(j).getNrOfVerts());
	}
		
	glUseProgram(0);
	glBindVertexArray(0);
}

void ParticleHandler::drawParticleModels(mat4 projectionMatrix, mat4 viewMatrix)
{
	this->particleShader.use();
	this->particleShader.setUniform("outAlpha",1.0f);
	mat4 mvp;
	for(unsigned int i=0;i<this->particleModels.size();i++)
	{
		this->particleShader.setUniform("ro",this->particleModels[i].getColor().x*this->particleModels[i].getContrast());
		this->particleShader.setUniform("go",this->particleModels[i].getColor().y*this->particleModels[i].getContrast());
		this->particleShader.setUniform("bo",this->particleModels[i].getColor().z*this->particleModels[i].getContrast());
		//set upp uniforms for rendering call
		mvp=projectionMatrix*viewMatrix*this->particleModels[i].getModelMatrix();
		this->particleShader.setUniform("MVP",mvp);
		mat3 normalMatrix=transpose(inverse(mat3(viewMatrix*this->particleModels[i].getModelMatrix())));
		this->particleShader.setUniform("normalMatrix",normalMatrix);
		
		//get a pointer to a vector with meshes info for each mesh
		for(unsigned int j=0;j<this->particleModels[i].getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->particleModels[i].getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->particleModels[i].getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->particleModels[i].getMeshInfo()->at(j).getNrOfVerts());
		}

	}
	this->bBoxShader.use();
	this->bBoxShader.setUniform("ro",0.0f);
	this->bBoxShader.setUniform("go",1.0f);
	this->bBoxShader.setUniform("bo",0.0f);
	//renders bounding box
	glLineWidth(2);
	
	for(unsigned int i=0;i<this->particleModels.size();i++)
	{
		if(this->particleModels[i].isSelected())
		{
			//set upp uniforms for rendering call
			mvp=projectionMatrix*viewMatrix*this->particleModels[i].getModelMatrix();
			this->bBoxShader.setUniform("MVP",mvp);
			
			glBindVertexArray(this->particleModels[i].getBoundingBox()->getVaoh());
			glDrawArrays(GL_LINE_STRIP,0,16);
		}
	}
	
	glUseProgram(0);
	glBindVertexArray(0);
}
void ParticleHandler::unselectAllParticleModels()
{
	for(unsigned int i = 0;i<this->particleModels.size();i++)
	{
		this->particleModels[i].unSelect();
	}
	this->selectedParticleIndex=-1;
}
void ParticleHandler::addParticleModel(Particle p)
{
	this->particleModels.push_back(p);
	this->particleModels[this->particleModels.size()-1].setMesh(this->meshes.getMeshInfo(0));
	this->particleModels[this->particleModels.size()-1].setBoundingBox(this->meshes.getBoundingBox(0));
	this->particleModels[this->particleModels.size()-1].select();
	this->selectedParticle=this->particleModels[this->particleModels.size()-1];
	this->selectedParticleIndex=this->particleModels.size()-1;
}
void ParticleHandler::removeSelectedParticles()
{	
	/*
	if(selectedParticleIndex>=0&&selectedParticleIndex<this->particleModels.size())
	{
		this->particleModels[selectedParticleIndex]=this->particleModels[this->particleModels.size()-1];
		this->particleModels.pop_back();
	}*/
	for(int i = 0;i<this->particleModels.size();i++)
	{
		if(this->particleModels[i].isSelected())
		{
			cout << i << endl;
			this->particleModels[i]=this->particleModels[this->particleModels.size()-1];
			this->particleModels.pop_back();
			i--;
		}
	}
}

int ParticleHandler::selectParticles(float normalizedX, float normalizedY,vec3 pos, mat4 projMatrix,mat4 viewMatrix)
{
	float min=999999.0f;
	int minIndex=-1;
	for(unsigned int j=0;j<this->particleModels.size();j++)
	{
		this->particleModels[j].unSelect();
		bool found=true;
		float t1=0;
		float t2=0;
		mat4 unview = inverse(projMatrix*viewMatrix);
		vec4 near_point = unview * vec4(normalizedX, normalizedY, 0, 1);

		vec4 mm = inverse(mat4(particleModels[j].getModelMatrix()))*vec4(pos,1);
		

		vec3 t = vec3(near_point.x/near_point.w,near_point.y/near_point.w,near_point.z/near_point.w)-pos;
		vec3 rayd=inverse(mat3(particleModels[j].getModelMatrix()))*t;
		rayd = normalize(rayd);


		float sides[3] =
		{
			this->particleModels[j].getBoundingBox()->getBboxSide().x,
			this->particleModels[j].getBoundingBox()->getBboxSide().y,
			this->particleModels[j].getBoundingBox()->getBboxSide().z
		};
		
		vec3 normalizedSides[3] = 
		{
			vec3(1,0,0),
			vec3(0,1,0),
			vec3(0,0,1)
		};
		
		float tmin=-0.000001;
		float tmax=99999999;
		

		vec3 p = this->particleModels[j].getBoundingBox()->getBboxPos()-vec3(mm.x/mm.w,mm.y/mm.w,mm.z/mm.w);
		
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
		this->particleModels[minIndex].select();
		this->selectedParticle = this->particleModels[minIndex];
		this->selectedParticleIndex=minIndex;
		return minIndex;
	}
	return -1;
}
Particle ParticleHandler::getSelectedParticle()
{
	return this->selectedParticle;
}
void ParticleHandler::assignParticleNewParticle(int index, Particle p)
{
	if(index>=0&&index<this->particleModels.size())
	{
		this->particleModels[index]=p;
		this->particleModels[index].setMesh(this->meshes.getMeshInfo(0));
		this->particleModels[index].setBoundingBox(this->meshes.getBoundingBox(0));
	}
	
}

int ParticleHandler::getSelectedParticleIndex()
{
	int index =-1;
	for(unsigned int i=0;i<this->particleModels.size();i++)
	{
		if(this->particleModels[i].isSelected())
			index=i;
	}
	return index;
}

void ParticleHandler::save(string path, string filename)
{
	fstream file;
	string p = path+filename+".txt";
	file.open(p.c_str(),fstream::out|fstream::app);
	file << "PARTICLESYSTEMS: type, pos x,y,z, rot x,y,z, color r,g,b"<<endl;
	
	for(int i=0;i<this->particleModels.size();i++)
	{
		if(this->particleModels[i].getParticleType()==ParticleType::GLOWRING)
			file<<"GLOWRING ";
		if(this->particleModels[i].getParticleType()==ParticleType::FIRE)
			file<<"FIRE ";
		if(this->particleModels[i].getParticleType()==ParticleType::EMIT)
			file<<"EMIT ";
		if(this->particleModels[i].getParticleType()==ParticleType::SMOKE)
			file<<"SMOKE ";
		if(this->particleModels[i].getParticleType()==ParticleType::FLOW)
			file<<"FLOW ";
		file << this->particleModels[i].getPos().x << " " << this->particleModels[i].getPos().y << " " << this->particleModels[i].getPos().z << " " 
		<< this->particleModels[i].getRot().x << " " << this->particleModels[i].getRot().y << " " << this->particleModels[i].getRot().z << " " 
		<< this->particleModels[i].getColor().x << " " << this->particleModels[i].getColor().y << " " << this->particleModels[i].getColor().z << endl;
	}
	file << "end"<<endl;
	file.close();
}

void ParticleHandler::clear()
{
	this->particleModels.clear();
}