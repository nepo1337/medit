#include "PathHandler.h"

PathHandler::PathHandler()
{
}

PathHandler::~PathHandler()
{
	for(unsigned int i=0;i<this->Paths.size();i++)
	{
		this->Paths[i].freeGFX();
	}
}

void PathHandler::init()
{
	this->pathRenderer.compileShaderFromFile("path.vsh",GLSLShader::VERTEX);
	this->pathRenderer.compileShaderFromFile("path.fsh",GLSLShader::FRAGMENT);
	this->pathRenderer.bindAttribLocation(0,"vertexPosition");
	this->pathRenderer.bindAttribLocation(1,"vertexNormal");
	//cout<<this->pathRenderer.log();
	this->pathRenderer.link();
	//cout<<this->pathRenderer.log();
	
	this->lineRenderer.compileShaderFromFile("bbox.vsh",GLSLShader::VERTEX);
	this->lineRenderer.compileShaderFromFile("bbox.fsh",GLSLShader::FRAGMENT);
	this->lineRenderer.bindAttribLocation(3,"bBoxCordsVertex");
	
	//cout<<this->lineRenderer.log();
	this->lineRenderer.link();
	//cout<<this->lineRenderer.log();
	srand(time(NULL));
	
	this->meshes.load("./models/paths/");
	this->meshInfo = this->meshes.getMeshInfo(0);
	this->bb = this->meshes.getBoundingBox(0);
}

void PathHandler::setMesh(vector<MeshInfo> *meshInfo,BoundingBox * bb)
{
	this->meshInfo=meshInfo;
	this->bb=bb;
}

void PathHandler::addPath()
{
	if(this->Paths.empty())
	{
		this->Paths.push_back(Path());
		int r = rand()%256;
		int g = rand()%256;
		int b = rand()%256;
		this->Paths[this->Paths.size()-1].setColor(vec3((float)r/255,(float)g/255,(float)b/255));
	}
	else
	{
		if(this->Paths[this->Paths.size()-1].getNrOfFlags()>0)
		{
			this->Paths.push_back(Path());
			int r = rand()%256;
			int g = rand()%256;
			int b = rand()%256;
			this->Paths[this->Paths.size()-1].setColor(vec3((float)r/255,(float)g/255,(float)b/255));
		}
	}
}

void PathHandler::addFlagToCurrentPath(vec3 pos)
{
	if(this->Paths.size()>0)
	{
		this->Paths[this->Paths.size()-1].addFlag(pos);
	}
}

void PathHandler::updateProjectionMatrix(float width, float height)
{
	float nearClip = 0.5f;
	float farClip  = 1000.0f;
	float fov_deg = 45.0f;
	float aspect = (float)width/(float)height;
	this->projectionMatrix=perspective(fov_deg, aspect,nearClip,farClip);
}
void PathHandler::updateViewMatrix(mat4 view)
{
	this->viewMatrix=view;
}

void PathHandler::drawFlag(vec3 pos)
{
	mat4 mvp=mat4(1.0f);
	mat4 mm = mat4(1.0f);
	
	this->pathRenderer.use();
	this->pathRenderer.setUniform("ro",1.0f);
	this->pathRenderer.setUniform("go",1.0f);
	this->pathRenderer.setUniform("bo",1.0f);
	this->pathRenderer.setUniform("outAlpha",1);
		
	mm*=translate(pos);
	mvp=this->projectionMatrix*this->viewMatrix*mm;
	this->pathRenderer.setUniform("MVP",mvp);
	mat3 normalMatrix=mat3(mm);
	this->pathRenderer.setUniform("normalMatrix",normalMatrix);
	glBindVertexArray(this->meshInfo->at(0).getVaoh());
	glDrawArrays(GL_TRIANGLES,0,this->meshInfo->at(0).getNrOfVerts());
	
	glUseProgram(0);
	glBindVertexArray(0);
	
}
void PathHandler::drawPaths()
{
	this->pathRenderer.use();
	mat4 mvp=mat4(1.0f);
	
	this->pathRenderer.setUniform("outAlpha",1);
	for(unsigned int i=0;i<this->Paths.size();i++)
	{
		this->pathRenderer.setUniform("ro",this->Paths[i].getColor().x);
		this->pathRenderer.setUniform("go",this->Paths[i].getColor().y);
		this->pathRenderer.setUniform("bo",this->Paths[i].getColor().z);
		
		for(int j=0;j<this->Paths[i].getNrOfFlags();j++)
		{
			mat4 mm = mat4(1.0f);
			mm*=translate(this->Paths[i].getFlagPosition(j));
			mvp=this->projectionMatrix*this->viewMatrix*mm;
			this->pathRenderer.setUniform("MVP",mvp);
			mat3 normalMatrix=mat3(mm);
			this->pathRenderer.setUniform("normalMatrix",normalMatrix);

			glBindVertexArray(this->meshInfo->at(0).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->meshInfo->at(0).getNrOfVerts());

		}
	}
	this->lineRenderer.use();
	//draw the lines between paths
	mat4 mm = mat4(1.0f);
	glLineWidth(3);
	glDisable(GL_DEPTH_TEST);
	for(unsigned int i=0;i<this->Paths.size();i++)
	{
		this->lineRenderer.setUniform("ro",this->Paths[i].getColor().x);
		this->lineRenderer.setUniform("go",this->Paths[i].getColor().y);
		this->lineRenderer.setUniform("bo",this->Paths[i].getColor().z);
		
		mvp=this->projectionMatrix*this->viewMatrix*mm;
		this->lineRenderer.setUniform("MVP",mvp);
			
		glBindVertexArray(this->Paths[i].getVaohToLines());
		glDrawArrays(GL_LINE_STRIP,0,this->Paths[i].getNrOfFlags());
	}
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1);
	for(unsigned int i=0;i<this->Paths.size();i++)
	{
		if(this->Paths[i].isSelected())
		{
			this->lineRenderer.setUniform("ro",this->Paths[i].getColor().x);
			this->lineRenderer.setUniform("go",this->Paths[i].getColor().y);
			this->lineRenderer.setUniform("bo",this->Paths[i].getColor().z);
			
			for(int j=0;j<this->Paths[i].getNrOfFlags();j++)
			{
				mm = mat4(1.0f);
				mm*=translate(this->Paths[i].getFlagPosition(j));
					
				mvp=this->projectionMatrix*this->viewMatrix*mm;
				this->lineRenderer.setUniform("MVP",mvp);
					
				glBindVertexArray(this->bb->getVaoh());
				glDrawArrays(GL_LINE_STRIP,0,16);
			}
		}
	}
	glUseProgram(0);
	glBindVertexArray(0);
}

void PathHandler::selectPaths(float normalizedX, float normalizedY,vec3 pos)
{
	float min=999999.0f;
	int minIndex=-1;
	bool found=true;
	for(int k=0;k<this->Paths.size();k++)
	{
		this->Paths[k].unselect();
		for(unsigned int j=0;j<this->Paths[k].getNrOfFlags();j++)
		{
			found=true;
			float t1=0;
			float t2=0;
			mat4 unview = inverse(this->projectionMatrix*this->viewMatrix);
			vec4 near_point = unview * vec4(normalizedX, normalizedY, 0, 1);
			
			mat4 modelMatrix = mat4(1.0f);
			modelMatrix*=translate(this->Paths[k].getFlagPosition(j));
			
			vec4 mm = inverse(mat4(modelMatrix))*vec4(pos,1);
			

			vec3 t = vec3(near_point.x/near_point.w,near_point.y/near_point.w,near_point.z/near_point.w)-pos;
			vec3 rayd=inverse(mat3(modelMatrix))*t;
			rayd = normalize(rayd);


			float sides[3] =
			{
				this->bb->getBboxSide().x,
				this->bb->getBboxSide().y,
				this->bb->getBboxSide().z
			};
			
			vec3 normalizedSides[3] = 
			{
				vec3(1,0,0),
				vec3(0,1,0),
				vec3(0,0,1)
			};
			
			float tmin=-0.000001;
			float tmax=99999999;
			

			vec3 p = this->bb->getBboxPos()-vec3(mm.x/mm.w,mm.y/mm.w,mm.z/mm.w);
			
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
						minIndex=k;
					}
				}
			}
		}
	}
	if(minIndex>=0)
		this->Paths[minIndex].select();
}

void PathHandler::removeSelectedPaths()
{
	for(int i=0;i<this->Paths.size();i++)
	{
		if(this->Paths[i].isSelected())
		{
			this->Paths[i].freeGFX();
			this->Paths[i]=this->Paths[this->Paths.size()-1];
			this->Paths.pop_back();
			i--;
		}
	}
}

void PathHandler::save(string path, string filename)
{
	fstream file;
	string p = path+filename+".txt";
	file.open(p.c_str(),fstream::out|fstream::app);
	for(int i=0;i<this->Paths.size();i++)
	{
		file << "path"<<endl;
		for(int j=0;j<this->Paths[i].getNrOfFlags();j++)
		{
			file << this->Paths[i].getFlagPosition(j).x << " " <<this->Paths[i].getFlagPosition(j).z << endl;
		}
		file << "end"<<endl;
	}
	file.close();
}

void PathHandler::clear()
{
	this->Paths.clear();
}

int PathHandler::getNrOfPaths()
{
	return this->Paths.size();
}