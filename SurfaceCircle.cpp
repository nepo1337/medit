#include "SurfaceCircle.h"

SurfaceCircle::SurfaceCircle()
{
}

SurfaceCircle::~SurfaceCircle()
{
	glDeleteTextures(1,&this->texHandle);
	glDeleteBuffers(2,this->vbohs);
	glDeleteVertexArrays(1, &this->vaoh);
}

void SurfaceCircle::init()
{
	this->shader.compileShaderFromFile("noLightModel.vsh",GLSLShader::VERTEX);
	this->shader.compileShaderFromFile("noLightModel.fsh",GLSLShader::FRAGMENT);
	this->shader.bindAttribLocation(0,"vertexPosition");
	this->shader.bindAttribLocation(1,"vertexUv");
	this->shader.link();
	
	float planeVerts[]=
	{
		-0.1f,0.0f,-0.1f,
		-0.1f,0.0f,0.1f,
		0.1f,0.0f,-0.1f,
		0.1f,0.0f,0.1f,
		0.1f,0.0f,-0.1f,
		-0.1f,0.0f,0.1f
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
	
	glActiveTexture(GL_TEXTURE0);
	this->texHandle = SOIL_load_OGL_texture("./gui/ring.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_COMPRESS_TO_DXT);
	
	if(texHandle==0)
				cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
	
	glGenVertexArrays(1,&vaoh);
	glGenBuffers(2,vbohs);
	//vertex points
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts),planeVerts,GL_STATIC_DRAW);
	
	//uvs
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeUvs), planeUvs, GL_STATIC_DRAW);
	
	glBindVertexArray(vaoh);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	//vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	//uv
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
}

void SurfaceCircle::setPosition(vec3 pos)
{
	this->position=pos;
}
vec3 SurfaceCircle::getPosition()
{
	return this->position;
}
void SurfaceCircle::setScale(float s)
{
	this->scale=s;
}
float SurfaceCircle::getscale()
{
	return this->scale;
}

void SurfaceCircle::draw(mat4 projectionMatrix,mat4 viewMatrix)
{
	this->shader.use();
	mat4 mm = translate(this->position)*glm::scale(mat4(1.0f),vec3(this->scale));
	mat4 mvp=projectionMatrix*viewMatrix*mm;
	this->shader.setUniform("MVP",mvp);
		
	glBindTexture(GL_TEXTURE_2D, this->texHandle);
	glBindVertexArray(this->vaoh);
	glDrawArrays(GL_TRIANGLES,0,6);
	
	glUseProgram(0);
	glBindVertexArray(0);
}