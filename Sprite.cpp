#include "Sprite.h"

Sprite::Sprite()
{
	this->vaoHandle=0;
	this->texHandle=0;
	this->position = vec3(0.0f);
	this->vboHandle[0]=0;
	this->vboHandle[1]=0;
	this->modelMatrix=mat4(1.0f);
}

Sprite::~Sprite()
{
	glDeleteTextures(1,&this->texHandle);
	glDeleteBuffers(2,this->vboHandle);
	glDeleteVertexArrays(1, &this->vaoHandle);
}

GLuint Sprite::getVaoHandle()
{
	return this->vaoHandle;
}

GLuint Sprite::getTextureHandle()
{
	return this->texHandle;
}

vec3 Sprite::getPosition()
{
	return this->position;
}

bool Sprite::init(vec3 pos, float width, float height, string textureName)
{
	this->position=pos;
	
	float uvs[]=
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f
	};
	
	float verts[]=
	{
		0.0f-width,0.0f-height,0.0f,
		0.0f-width,0.0f+height,0.0f,
		0.0f+width,0.0f-height,0.0f,
		0.0f+width,0.0f+height,0.0f,
		0.0f+width,0.0f-height,0.0f,
		0.0f-width,0.0f+height,0.0f
	};
	
	glGenVertexArrays(1,&this->vaoHandle);
	glGenBuffers(2,this->vboHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),verts,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	
	glBindVertexArray(this->vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	glActiveTexture(GL_TEXTURE0);
	this->texHandle = SOIL_load_OGL_texture(textureName.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	if(this->texHandle==0)
				cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
	
	this->modelMatrix*=translate(this->position);
	return this->vaoHandle>0&&this->texHandle>0&&this->vboHandle>0;
}

bool Sprite::init(vec3 pos, float width, float height)
{
	this->position=pos;
	
	float uvs[]=
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f
	};
	
	float verts[]=
	{
		-width,-height,0.0f,
		-width,height,0.0f,
		width,-height,0.0f,
		width,height,0.0f,
		width,-height,0.0f,
		-width,height,0.0f
	};
	
	glGenVertexArrays(1,&this->vaoHandle);
	glGenBuffers(2,this->vboHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),verts,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	
	glBindVertexArray(this->vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	this->modelMatrix*=translate(this->position);
	return this->vaoHandle>0&&this->vboHandle>0;
}

mat4 Sprite::getModelMatrix()
{
	return this->modelMatrix;
}

void Sprite::setPosition(vec3 pos)
{
	this->position=pos;
	this->modelMatrix=mat4(1.0f);
	this->modelMatrix*=translate(this->position);
}