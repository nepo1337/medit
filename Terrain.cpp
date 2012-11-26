#include "Terrain.h"

Terrain::Terrain(float width, float height, int texScale)
{
	this->width=width;
	this->height=height;
	this->texScale=texScale;
	this->debug=true;
	
	float planeVerts[]=
	{
		0.0f,0.0f,0.0f,
		this->width,0.0f,0.0f,
		this->width,0.0f,this->height,
		this->width,0.0f,this->height,
		0.0f,0.0f,this->height,
		0.0f,0.0f,0.0f,
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
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f
	};
	

	this->terrInf.texB = SOIL_load_OGL_texture("terrain/grass.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	//upload text

	this->terrInf.texH = SOIL_load_OGL_texture("terrain/medalpha.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	if(debug)
	{
		if(texH==0)
			cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
	}

	//create buffers
	glGenVertexArrays(1,&vaoh);
	glGenBuffers(3,vbohs);
	//vertex points
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts),planeVerts,GL_STATIC_DRAW);
	
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeNorms), planeNorms, GL_STATIC_DRAW);
	
	//uvs
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeUvs), planeUvs, GL_STATIC_DRAW);
	
	//SETTING UP VAO
	glBindVertexArray(vaoh);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	//vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	//uv
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	this->terrInf.vaoh=this->vaoh;
	this->terrInf.vbohs[0]=this->vbohs[0];
	this->terrInf.vbohs[1]=this->vbohs[1];
	this->terrInf.vbohs[2]=this->vbohs[2];
}

Terrain::~Terrain()
{
	glDeleteBuffers(3,&this->vbohs[3]);
	glDeleteVertexArrays(1, &this->vaoh);
}

GLuint Terrain::getVaoh()
{
	return this->vaoh;
}

TerrainInfo *Terrain::getTerrInfo()
{
	return &this->terrInf;
}