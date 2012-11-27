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
		0.0f,0.0f,this->height,
		this->width,0.0f,0.0f,
		this->width,0.0f,this->height,
		this->width,0.0f,0.0f,
		0.0f,0.0f,this->height,
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
		0.0f,1.0f*texScale,
		1.0f*texScale,0.0f,
		1.0f*texScale,1.0f*texScale,
		1.0f*texScale,0.0f,
		0.0f,1.0f*texScale
	};
	
	//creating blendmaps
	this->blendmap1.Create(512,512,sf::Color(255,0,0,0));
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
	
	this->terrInf.texA = SOIL_load_OGL_texture("terrain/grass.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.texB = SOIL_load_OGL_texture("terrain/dirt.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);

	
	/*if(debug)
	{
		if(texH==0)
			cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
	}*/

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

void Terrain::makeBlendMap(GLuint& handle, sf::Image img)
{
	glDeleteTextures(1,&handle);
	glGenTextures(1,&handle);
	glBindTexture(GL_TEXTURE_2D,handle);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.GetWidth(),img.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,img.GetPixelsPtr());
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

Terrain::~Terrain()
{
	glDeleteBuffers(3,&this->vbohs[3]);
	glDeleteVertexArrays(1, &this->vaoh);
	glDeleteTextures(1,&this->terrInf.blendmap1H);
	glDeleteTextures(1,&this->terrInf.texA);
}

GLuint Terrain::getVaoh()
{
	return this->vaoh;
}

TerrainInfo *Terrain::getTerrInfo()
{
	return &this->terrInf;
}

void Terrain::paint(int indexForblendMap,float radius,int whichTex,vec3 origin, vec3 ray)
{
	//one of the triangles
	vec3 v1= vec3(0.0f,0.0f,0.0f);
	vec3 v2= vec3(0.0f,0.0f,this->height);
	vec3 v3= vec3(this->width,0.0f,0.0f);
	vec3 hit = intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
	//the other triangle
	if(hit.x==1)
	{
		v1= vec3(this->width,0.0f,this->height);
		v2= vec3(this->width,0.0f,0.0f);
		v3= vec3(0.0f,0.0f,this->height);
		hit=intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
	}
	cout<<"X"<<hit.x<<endl;
	cout<<"Z"<<hit.z<<endl;
	float rad=40;
	int x=hit.x*this->blendmap1.GetWidth();
	int y=-hit.z*this->blendmap1.GetHeight();
	cout << x<<" " << y << endl;
	for(int i=y-rad;i<y+rad;i++)
	{
		for(int j=x-rad;j<x+rad;j++)
		{
			if(j>0&&i>0&&j<this->blendmap1.GetWidth()&&i<this->blendmap1.GetHeight())
			{
				sf::Color pixCol=this->blendmap1.GetPixel(j,i);
				float r,g,b,a;
				r=pixCol.r-5;
				g=pixCol.g+5;
				b=pixCol.b;
				a=pixCol.a;
				if(g<255)
					this->blendmap1.SetPixel(j,i,sf::Color(r,g,b,a));
			}
		}
	}
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
}