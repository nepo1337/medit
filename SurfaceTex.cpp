#include "SurfaceTex.h"

SurfaceTex::SurfaceTex()
{
}

SurfaceTex::~SurfaceTex()
{
	glDeleteTextures(1,&this->texHandle);
	glDeleteBuffers(4,this->vbohs);
	glDeleteVertexArrays(1, &this->vaoh);
}

void SurfaceTex::init(string path, string texName)
{
	this->name=texName;
	float planeVerts[]=
	{
		-0.5f,0.0f,-0.5f,
		-0.5f,0.0f,0.5f,
		0.5f,0.0f,-0.5f,
		0.5f,0.0f,0.5f,
		0.5f,0.0f,-0.5f,
		-0.5f,0.0f,0.5f
	};

	float planeNorms[]=
	{
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};
	
	float planeUvs[]=
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f
	};
	
	float bBox[]=
	{
		-0.5f,0.0f,-0.5f,
		-0.5f,0.0f,0.5f,
		0.5f,0.0f,0.5f,
		0.5f,0.0f,-0.5f,
		-0.5f,0.0f,-0.5f,
	};
	glActiveTexture(GL_TEXTURE0);
	string fullPath=path+texName;
	this->texHandle = SOIL_load_OGL_texture(fullPath.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	
	if(texHandle==0)
				cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
	//create buffers
	glGenVertexArrays(1,&vaoh);
	glGenBuffers(4,vbohs);
	//vertex points
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts),planeVerts,GL_STATIC_DRAW);
	
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeNorms), planeNorms, GL_STATIC_DRAW);
	
	//uvs
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeUvs), planeUvs, GL_STATIC_DRAW);
	
	//bbox
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bBox), bBox, GL_STATIC_DRAW);
	
	
	//SETTING UP VAO
	glBindVertexArray(vaoh);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	//vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	//uv
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,NULL);
	//bbox
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[3]);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);
	
}
vector<float>* SurfaceTex::getRotations()
{
	return &this->rotations;
}

vector<vec3>* SurfaceTex::getPositions()
{
	return &this->positions;
}
vector<bool>* SurfaceTex::getDrawBbox()
{
	return &this->showBbox;
}

GLuint SurfaceTex::getTexHandle()
{
	return this->texHandle;
}
GLuint SurfaceTex::getVaoH()
{
	return this->vaoh;
}
void SurfaceTex::addSurface(float rot, vec3 pos)
{
	this->rotations.push_back(rot);
	this->positions.push_back(vec3(pos.x,0,pos.z));
	this->showBbox.push_back(false);
}

void SurfaceTex::select(int index)
{
	this->showBbox[index]=true;
}
void SurfaceTex::deSelect(int index)
{
	this->showBbox[index]=false;
}
string SurfaceTex::getName()
{
	return this->name;
}

bool SurfaceTex::isSelected(int i)
{
	return this->showBbox[i];
}
void SurfaceTex::remove(int i)
{
	this->positions.erase(positions.begin()+i);
	this->rotations.erase(rotations.begin()+i);
	this->showBbox.erase(showBbox.begin()+i);
}