#include "Renderer.h"

Renderer::Renderer()
{
	//the shader used for rendering models
	this->debug=true;
	this->modelShader.compileShaderFromFile("model.vsh",GLSLShader::VERTEX);
	this->modelShader.compileShaderFromFile("model.fsh",GLSLShader::FRAGMENT);
	this->modelShader.bindAttribLocation(0,"vertexPosition");
	this->modelShader.bindAttribLocation(1,"vertexNormal");
	this->modelShader.bindAttribLocation(2,"vertexUv");
	
	if(debug)
		cout<<this->modelShader.log();
		
	this->modelShader.link();
	
	if(debug)
		cout<<this->modelShader.log();
		
	this->modelShader.setUniform("tex1",0);
		
	//the shader used for rendering the terrain
	this->debug=true;
	
	//bbox
	this->bBoxShader.compileShaderFromFile("bbox.vsh",GLSLShader::VERTEX);
	this->bBoxShader.compileShaderFromFile("bbox.fsh",GLSLShader::FRAGMENT);
	this->bBoxShader.bindAttribLocation(3,"bBoxCordsVertex");
	
	if(debug)
		cout<<this->bBoxShader.log();
		
	this->bBoxShader.link();
	
	if(debug)
		cout<<this->bBoxShader.log();
	glUseProgram(0);
		
	if(debug)
	{
		//info
		const GLubyte *vendor = glGetString(GL_VENDOR);
		const GLubyte *renderer = glGetString(GL_RENDERER);
		const GLubyte *version = glGetString(GL_VERSION);
		const GLubyte *glslver = glGetString(GL_SHADING_LANGUAGE_VERSION);
		GLint major,minor;
		glGetIntegerv(GL_MAJOR_VERSION,&major);
		glGetIntegerv(GL_MINOR_VERSION,&minor);

		cout <<"vendor: "<<vendor<<endl;
		cout <<"renderer: "<<renderer<<endl;
		cout <<"openGL version: "<<version<<endl;
		cout <<"GLSL version: "<<glslver<<endl;
		cout <<"GL version, maj min: "<<major<<minor<<endl;
	}
	
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // set depth-testing function type
	//glEnable(GL_CULL_FACE); // enable culling of back-faces
	//glCullFace(GL_BACK); // enable culling of back-faces
	//glFrontFace(GL_CCW);
	
	this->viewMatrix=mat4(0.0f);
	this->projMatrix=mat4(0.0f);
}

Renderer::~Renderer()
{
}

void Renderer::draw()
{
	//renderin models
	this->modelShader.use();
	mat4 mvp=mat4(0.0f);
	glActiveTexture(GL_TEXTURE0);
	for(unsigned int i=0;i<this->models.size();i++)
	{
		//set upp uniforms for rendering call
		mvp=this->projMatrix*this->viewMatrix*this->models[i]->getModelMatrix();
		this->modelShader.setUniform("modelMatrix",this->models[i]->getModelMatrix());
		this->modelShader.setUniform("MVP",mvp);
		mat3 normalMatrix=transpose(inverse(mat3(this->viewMatrix*this->models[i]->getModelMatrix())));
		this->modelShader.setUniform("normalMatrix",normalMatrix);
		
		//get a pointer to a vector with meshes info for each mesh
		for(unsigned int j=0;j<this->models[i]->getMeshInfo()->size();j++)
		{
			glBindTexture(GL_TEXTURE_2D, this->models[i]->getMeshInfo()->at(j).getTexh());
			glBindVertexArray(this->models[i]->getMeshInfo()->at(j).getVaoh());
			glDrawArrays(GL_TRIANGLES,0,this->models[i]->getMeshInfo()->at(j).getNrOfVerts());
		}
	}
	this->bBoxShader.use();
	
	//renders bounding box
	for(unsigned int i=0;i<this->models.size();i++)
	{
		//if the object is selected
		if(this->models[i]->isSelected())
		{
			//set upp uniforms for rendering call
			mvp=this->projMatrix*this->viewMatrix*this->models[i]->getModelMatrix();
			this->modelShader.setUniform("MVP",mvp);
			
			glBindVertexArray(this->models[i]->getBoundingBox()->getVaoh());
			glDrawArrays(GL_LINE_STRIP,0,16);
		}
	}
	
	glUseProgram(0);
	glBindVertexArray(0);
}

void Renderer::updateViewMatrix(mat4 viewMatrix)
{
	this->viewMatrix=viewMatrix;
	
	this->modelShader.use();
	this->modelShader.setUniform("viewMatrix",this->viewMatrix);
	glUseProgram(0);
}
void Renderer::updateProjMatrix(float width, float height)
{
	this->wheight=height;
	this->wwidth=width;
	float nearClip = 0.5f;
	float farClip  = 1000.0f;
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

mat4 Renderer::getProjMatrix()
{
	return this->projMatrix;
}