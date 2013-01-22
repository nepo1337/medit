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

vec3 Renderer::rayIntersectModelBB(float normalizedX, float normalizedY,vec3 pos)
{
	
	
	for(unsigned int j=0;j<this->models.size();j++)
	{
		
		mat4 unview = inverse(this->projMatrix*this->viewMatrix*this->models[j]->getModelMatrix());
		vec4 near_point = unview * vec4(normalizedX, normalizedY, 0, 1);
		//the last vec3 is the pos of the camera
		vec3 t = vec3(near_point.x/near_point.w,near_point.y/near_point.w,near_point.z/near_point.w)-pos;
		vec3 rayd = normalize(t);
		
		vec3 normalizedSides[3] = 
		{
			normalize(vec3(this->models.at(j)->getBoundingBox()->getBboxSide().x,0.0f,0.0f)),
			normalize(vec3(0,this->models.at(j)->getBoundingBox()->getBboxSide().y,0)),
			normalize(vec3 (0,0,this->models.at(j)->getBoundingBox()->getBboxSide().z))
		};
		
		vec3 sides[3] =
		{
			vec3(this->models.at(j)->getBoundingBox()->getBboxSide().x,0.0f,0.0f),
			vec3(0,this->models.at(j)->getBoundingBox()->getBboxSide().y,0),
			vec3(0,0,this->models.at(j)->getBoundingBox()->getBboxSide().z)
		};

		bool found = false;
		float tmin = -9999999;
		float tmax = 9999999;
		vec3 p = this->models.at(j)->getBoundingBox()->getBboxPos()-pos;
		for(int i=0;i<3;i++)
		{
			float e = dot(normalizedSides[i],p);
			float f = dot(normalizedSides[i],rayd);
			float k=f;
			if(f<0)
				k=-f;
			if(k>0.000000000000000001)
			{
				float t1 = e+length(sides[i])/2/f;
				float t2 = e-length(sides[i])/2/f;
				if(t1>t2)
				{
					float tmp=0;
					tmp=t1;
					t1=t2;
					t2=tmp;
				}
				if(t1>tmin)
					tmin=t1;
				if(t2<tmax)
					tmax=t2;
				if(tmin>tmax)
					found = false;
				if(tmax<0)
					found=false;
			}
			else if(-e-length(sides[i])/2 >0 || -e+length(sides[i])/2<0)
				found=false;
		}
		if(tmin>0)
		{
			//hitData.t=tmin;
			//hitData.color=b.color;
			found=true;
			cout << "k"<<endl;
		}
			//hitData.t=tmax;
			//hitData.color=b.color;
			//found=true;
	}

}