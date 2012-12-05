#include "Renderer.h"

Renderer::Renderer()
{
	//the shader used for rendering models
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
		
	//the shader used for rendering the terrain
	this->debug=true;
	this->TerrainShader.compileShaderFromFile("terr.vsh",GLSLShader::VERTEX);
	this->TerrainShader.compileShaderFromFile("terr.fsh",GLSLShader::FRAGMENT);
	this->TerrainShader.bindAttribLocation(0,"vertexPosition");
	this->TerrainShader.bindAttribLocation(1,"vertexNormal");
	this->TerrainShader.bindAttribLocation(2,"vertexUv");
	this->TerrainShader.bindAttribLocation(3,"vertexUvBM");
	
	
	if(debug)
		cout<<this->TerrainShader.log();
		
	this->TerrainShader.link();
	
	if(debug)
		cout<<this->TerrainShader.log();
		
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
	
	//rendering terrain
	//add modelmatrix if needed(should not be needed)
	mvp=this->projMatrix*this->viewMatrix;
	//terrain doesnt support any scaling etc
	mat4 modelMatrix(1.0f);
	this->TerrainShader.use();
	this->TerrainShader.setUniform("modelMatrix",modelMatrix);
	this->TerrainShader.setUniform("MVP",mvp);
	
	//draws ground plane
	glBindVertexArray(this->terrInf->vaoh);
	glDrawArrays(GL_TRIANGLES,0,6);
	
	glUseProgram(0);
	glBindVertexArray(0);
}

void Renderer::updateViewMatrix(mat4 viewMatrix)
{
	this->viewMatrix=viewMatrix;
	
	this->modelShader.use();
	this->modelShader.setUniform("viewMatrix",this->viewMatrix);
	this->TerrainShader.use();
	this->TerrainShader.setUniform("viewMatrix",this->viewMatrix);
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
	this->TerrainShader.use();
	this->TerrainShader.setUniform("projectionMatrix",this->projMatrix);
	glUseProgram(0);
}

void Renderer::addModel(Model* m)
{
	this->models.push_back(m);
}

void Renderer::setTerrainInfo(TerrainInfo *t)
{
	this->terrInf=t;
	this->TerrainShader.use();
	this->TerrainShader.setUniform("blendmap1", 1);
	this->TerrainShader.setUniform("blendmap2", 2);
	this->TerrainShader.setUniform("tex1", 3);
	this->TerrainShader.setUniform("tex2", 4);
	this->TerrainShader.setUniform("tex3", 5);
	this->TerrainShader.setUniform("tex4", 6);
	this->TerrainShader.setUniform("tex5", 7);
	this->TerrainShader.setUniform("tex6", 8);
	this->TerrainShader.setUniform("tex7", 9);
	this->TerrainShader.setUniform("tex8", 10);
	glUseProgram(0);
}

mat4 Renderer::getProjMatrix()
{
	return this->projMatrix;
}