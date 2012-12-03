#include "Terrain.h"

Terrain::Terrain(int size)
{
	this->mapsize=size;
	if(this->mapsize>2)
		this->mapsize=2;
		
	this->blendsc=1;
	if(size==2)
	{
		this->width=256;
		this->height=256;
		this->blendsc=4;
	}
	else if(size==1)
	{
		this->width=128;
		this->height=128;
		this->blendsc=2;
	}
	else
	{
		this->width=64;
		this->height=64;
		this->blendsc=1;
	}
	this->debug=true;
	
	float planeVerts[]=
	{
		0.0f,0.0f,0.0f,
		0.0f,0.0f,-this->height,
		this->width,0.0f,0.0f,
		this->width,0.0f,-this->height,
		this->width,0.0f,0.0f,
		0.0f,0.0f,-this->height,
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
		0.0f,1.0f*width,
		1.0f*height,0.0f,
		1.0f*height,1.0f*width,
		1.0f*height,0.0f,
		0.0f,1.0f*width
	};
	
	float blendmapUvs[]=
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f
	};
	
	
	//creating blendmaps
	this->blendmap1.Create(256*this->blendsc,256*this->blendsc,sf::Color(255,0,0,0));
	this->blendmap2.Create(256*this->blendsc,256*this->blendsc,sf::Color(0,0,0,0));

	//uploads the blendmaps, filing the handle
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
	this->makeBlendMap(this->terrInf.blendmap2H,this->blendmap2);
	
	//for blendmap 1
	this->terrInf.texH[0] = SOIL_load_OGL_texture("terrain/textures/set1/grass.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.texH[1] = SOIL_load_OGL_texture("terrain/textures/set1/grass2.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.texH[2] = SOIL_load_OGL_texture("terrain/textures/set1/grass3.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.texH[3] = SOIL_load_OGL_texture("terrain/textures/set1/snow.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);

	//for blendmap 2
	this->terrInf.tex2H[0] = SOIL_load_OGL_texture("terrain/textures/set1/dirt.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.tex2H[1] = SOIL_load_OGL_texture("terrain/textures/set1/dirt2.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.tex2H[2] = SOIL_load_OGL_texture("terrain/textures/set1/sand.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	this->terrInf.tex2H[3] = SOIL_load_OGL_texture("terrain/textures/set1/stone.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
	
	/*if(debug)
>>>>>>> d9861b4941551f94d383b9eeda8221bae5399f4f
	{
		if(texH==0)
			cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
	}*/

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
	
	//blendmapUvs
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blendmapUvs), blendmapUvs, GL_STATIC_DRAW);
	
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
	//uv blendmap
	glBindBuffer(GL_ARRAY_BUFFER, vbohs[3]);
	glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	this->terrInf.vaoh=this->vaoh;
}

//uploads the blendmaps to the GFX, getting a handle to the GFX side tex
void Terrain::makeBlendMap(GLuint& handle, sf::Image img)
{
	glDeleteTextures(1,&handle);
	glGenTextures(1,&handle);
	glBindTexture(GL_TEXTURE_2D,handle);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.GetWidth(),img.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,img.GetPixelsPtr());
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Terrain::~Terrain()
{
	glDeleteBuffers(4,&this->vbohs[4]);
	glDeleteVertexArrays(1, &this->vaoh);
	glDeleteTextures(4,this->terrInf.texH);
	glDeleteTextures(4,this->terrInf.tex2H);
}

GLuint Terrain::getVaoh()
{
	return this->vaoh;
}

TerrainInfo *Terrain::getTerrInfo()
{
	return &this->terrInf;
}

void Terrain::paint(float radius,int whichTex,vec3 origin, vec3 ray)
{
	//one of the triangles
	vec3 v1= vec3(0.0f,0.0f,0.0f);
	vec3 v2= vec3(0.0f,0.0f,-this->height);
	vec3 v3= vec3(this->width,0.0f,0.0f);
	vec3 hit = intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
	//the other triangle
	int x=hit.x*256*this->blendsc;
	int y=(hit.z*256*this->blendsc);
	if(hit.x==1)
	{
		v1= vec3(this->width,0.0f,-this->height);
		v2= vec3(this->width,0.0f,0.0f);
		v3= vec3(0.0f,0.0f,-this->height);
		hit=intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
		x=256*this->blendsc-hit.x*256*this->blendsc;
		y=256*this->blendsc+(-hit.z*256*this->blendsc);
	}
	float rad=1;
	
	for(int i=y-rad;i<y+rad;i++)
	{
		
		for(int j=x-rad;j<x+rad;j++)
		{
			if(j>=0&&i>=0&&j<256*this->blendsc&&i<256*this->blendsc)
			{
				sf::Color pix1=this->blendmap1.GetPixel(j,i);
				sf::Color pix2=this->blendmap2.GetPixel(j,i);
				this->increasePixelPaint(pix1,pix2,whichTex,10);
				
				this->blendmap1.SetPixel(j,i,pix1);
				this->blendmap2.SetPixel(j,i,pix2);
			}
		}
	}
	
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
	this->makeBlendMap(this->terrInf.blendmap2H,this->blendmap2);
}

//gets two pixels, calculates if the  choosen blendindex can increase, and in that case, increases it and decreses another pixels blend value
void Terrain::increasePixelPaint(sf::Color &pix1, sf::Color &pix2,int blendIndex,float strength)
{
	int pixBlends[]=
	{
		pix1.r,pix1.g,pix1.b,pix1.a,
		pix2.r,pix2.g,pix2.b,pix2.a
	};
	
	//counts the number of blends, say, if red is above 1, and alpha is over one, there is 2 nrofblends
	int nrOfBlends=-1; //OBS, MY CAUSE ERROR
	
	for(int i=0;i<8;i++)
	{
		if(pixBlends[i]>0)
			nrOfBlends++;
	}

	//increase as long as ur not going over 255
	//cout<<pixBlends[blendIndex]+255/strength<<endl;
	if(pixBlends[blendIndex]+255/strength<=255)
		pixBlends[blendIndex]+=(255/strength);
	//if you are going beyond 255
	else
	{
		for(int i=0;i<8;i++)
			pixBlends[i]=0;
		pixBlends[blendIndex]=255;
	}
	
	float resultBlendFac=(255/strength)/nrOfBlends;
	for(int i=0;i<8;i++)
	{
		if(blendIndex!=i)
		{
			if(pixBlends[i]-resultBlendFac>0)
				pixBlends[i]-=resultBlendFac;
		}
	}
	
	pix1.r=pixBlends[0];
	pix1.g=pixBlends[1];
	pix1.b=pixBlends[2];
	pix1.a=pixBlends[3];
	
	pix2.r=pixBlends[4];
	pix2.g=pixBlends[5];
	pix2.b=pixBlends[6];
	pix2.a=pixBlends[7];
}