#include "Renderer.h"

Renderer::Renderer()
{
	this->debug=true;
	this->modelShader.compileShaderFromFile("basic.vsh",GLSLShader::VERTEX);
	this->modelShader.compileShaderFromFile("basic.fsh",GLSLShader::FRAGMENT);
	this->modelShader.bindAttribLocation(0,"vertexPosition");
	this->modelShader.bindAttribLocation(1,"vertexNormal");
	this->modelShader.bindAttribLocation(2,"vertexUv");
	
	if(debug)
		cout<<this->modelShader.log();
		
	this->modelShader.link();
	
	if(debug)
		cout<<this->modelShader.log();
	
	this->viewMatrix=mat4(0.0f);
	this->projMatrix=mat4(0.0f);
}

Renderer::~Renderer()
{
	this->modelShader.destroy();
}

void Renderer::draw()
{
	//draw models
	this->modelShader.use();

	
	mat4 mvp=mat4(0.0f);
	//glActiveTexture(GL_TEXTURE0);
	for(unsigned int i=0;i<this->models.size();i++)
	{
		mvp=this->projMatrix*this->viewMatrix*this->models[i]->getModelMatrix();
		this->modelShader.setUniform("modelMatrix",this->models[i]->getModelMatrix());
		this->modelShader.setUniform("MVP",mvp);
		
		mat3 normalMatrix=transpose(inverse(mat3(this->viewMatrix*this->models[i]->getModelMatrix())));
		this->modelShader.setUniform("normalMatrix",normalMatrix);
		
		for(int j=0;j<this->models[i]->getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->models[i]->getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->models[i]->getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->models[i]->getMeshInfo()->at(j).getNrOfVerts());
		}
	}
	
	glUseProgram(0);
	glBindVertexArray(0);
}

void Renderer::updateCamera(vec3 pos)
{
	vec3 eye=pos;
	vec3 lookat(0.0f,0.0f,0.0f);
	vec3 up(0.0f,1.0f,0.0f);
	this->viewMatrix=glm::lookAt(eye,lookat,up);
	
	this->modelShader.use();
	this->modelShader.setUniform("viewMatrix",this->viewMatrix);
	glUseProgram(0);
}
void Renderer::updateProjMatrix(float width, float height)
{
	float nearClip = 0.5f;
	float farClip  = 100.0f;
	float fov_deg = 45.0f;
	float aspect = (float)width/(float)height;
	this->projMatrix=perspective(fov_deg, aspect,nearClip,farClip);
	
	this->modelShader.use();
	this->modelShader.setUniform("projectionMatrix",this->projMatrix);
	glUseProgram(0);
}

void Renderer::addModel(Model* m)
{
	this->models.push_back(m);
}