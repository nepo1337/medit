#include "Mesh.h"

Mesh::Mesh()
{
	this->debug=true;
	path="";
}

Mesh::~Mesh()
{
}

void Mesh::import(string path,string filename)
{
	this->path=path;
	this->objimp.import(path,filename);
}

void Mesh::setFilename(string fn)
{
	this->fileName=fn;
}

string Mesh::getFilename()
{
	return this->fileName;
}

vector<MeshInfo>* Mesh::getMeshInfo()
{
	return &this->meshesInfo;
}

void Mesh::uploadToGFX()
{
	if(debug)
		cout << "ALLOCATE GFX MEM FOR: " << this->getFilename()<<endl;
	for(int i =0;i<this->objimp.getNrOfObjects();i++)
	{
		Object o = this->objimp.getObject(i);
		
		//textures
		string p = this->path+o.getTextureNameKd().c_str();
		GLuint texH = SOIL_load_OGL_texture(p.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
		GLuint vaoH=0;
		glGenVertexArrays(1,&vaoH);
		MeshInfo mf;
		mf.setTexh(texH);
		mf.setVaoh(vaoH);
		mf.setNrOfVerts(o.getNrOfVerts());
		this->meshesInfo.push_back(mf);
		GLuint vbohs[3]={0,0,0};
		glGenBuffers(3,vbohs);
		
		//UPLOAD DATA
		//vertex points
		glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*o.getNrOfVerts()*3,&o.getVertexArray()[0],GL_STATIC_DRAW);
		
		//normals
		glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*o.getNrOfVerts()*3, &o.getNormalArray()[0], GL_STATIC_DRAW);
		
		//uvs
		glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*o.getNrOfVerts()*2, &o.getUvArray()[0], GL_STATIC_DRAW);
		
		//SETTING UP VAO
		glBindVertexArray(vaoH);
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
		
		this->VBOH.push_back(vbohs[0]);
		this->VBOH.push_back(vbohs[1]);
		this->VBOH.push_back(vbohs[2]);
	}
	
}

void Mesh::freeGFX()
{
	if(debug)
		cout<<"FREE GFX MEMORY FOR: " << this->getFilename()<<endl;
	
	GLuint n= this->VBOH.size();
	glDeleteBuffers(n,&this->VBOH[0]);
	for(unsigned int i =0;i<this->meshesInfo.size();i++)
	{
		GLuint h=this->meshesInfo[i].getVaoh();
		glDeleteVertexArrays(1, &h);
		GLuint t=this->meshesInfo[i].getTexh();
		glDeleteTextures(1,&t);
	}
	
}