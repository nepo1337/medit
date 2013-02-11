#include "Terrain.h"
#define PI 3.14159265

void Terrain::createNewMap(int size)
{
	this->mapsize=size;
	if(this->mapsize>2)
		this->mapsize=2;
		
	if(this->mapsize<0)
		this->mapsize=0;
		
	this->blendsc=1;
	if(size==2)
	{
		this->width=256;
		this->height=256;
		this->blendsc=16;
	}
	else if(size==1)
	{
		this->width=128;
		this->height=128;
		this->blendsc=8;
	}
	else
	{
		this->width=64;
		this->height=64;
		this->blendsc=4;
	}
	
	glDeleteBuffers(4,&this->vbohs[4]);
	glDeleteVertexArrays(1, &this->vaoh);
	
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
		0.0f,1.0f*width/8,
		1.0f*height/8,0.0f,
		1.0f*height/8,1.0f*width/8,
		1.0f*height/8,0.0f,
		0.0f,1.0f*width/8
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
	
	//creating blendmaps
	this->blendmap1.Create(256*this->blendsc,256*this->blendsc,sf::Color(255,0,0,0));
	this->blendmap2.Create(256*this->blendsc,256*this->blendsc,sf::Color(0,0,0,0));
	
	//creates a gridmap
	this->gridMap.Create(512*this->blendsc/4,512*this->blendsc/4,sf::Color(0,0,0,0));
	for(unsigned int i=1;i<this->gridMap.GetHeight();i+=2)
	{
		for(unsigned int j=1;j<this->gridMap.GetWidth();j+=2)
		{
			this->gridMap.SetPixel(j,i,sf::Color(0,255,0,0));
		}
	}
	
	//uploads the blendmaps, filing the handle
	glActiveTexture(GL_TEXTURE1);
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
	glActiveTexture(GL_TEXTURE2);
	this->makeBlendMap(this->terrInf.blendmap2H,this->blendmap2);

	glActiveTexture(GL_TEXTURE11);
	this->makeBlendMap(this->gridTexHandle,this->gridMap);
	for(int i=0;i<this->surfacesTextures.size();i++)
	{
		this->surfacesTextures[i].clear();
	}
}
Terrain::Terrain(int size)
{
	this->radiusMarker.init(80);
	this->surfaceScale=1.0f;
	this->roadSpacing=1.0f;
	this->opacity=0.5;
	this->dropoff=0.5;
	this->activeTex=0;
	this->mapsize=size;
	this->width=64;
	this->height=64;
	
	if(this->mapsize>2)
		this->mapsize=2;
		
	if(this->mapsize<0)
		this->mapsize=0;
		
	this->blendsc=1;
	if(size==2)
	{
		this->width=256;
		this->height=256;
		this->blendsc=16;
	}
	else if(size==1)
	{
		this->width=128;
		this->height=128;
		this->blendsc=8;
	}
	else
	{
		this->width=64;
		this->height=64;
		this->blendsc=4;
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
		0.0f,1.0f*width/8,
		1.0f*height/8,0.0f,
		1.0f*height/8,1.0f*width/8,
		1.0f*height/8,0.0f,
		0.0f,1.0f*width/8
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
	this->showGridMap=false;
	this->terrInf.blendmap1H=0;
	this->terrInf.blendmap2H=0;
	this->gridTexHandle=0;
	
	this->drawCircle=false;
	
	//creating blendmaps
	this->blendmap1.Create(256*this->blendsc,256*this->blendsc,sf::Color(255,0,0,0));
	this->blendmap2.Create(256*this->blendsc,256*this->blendsc,sf::Color(0,0,0,0));
	
	//creates a gridmap
	this->gridMap.Create(512*this->blendsc/4,512*this->blendsc/4,sf::Color(0,0,0,0));
	for(unsigned int i=1;i<this->gridMap.GetHeight();i+=2)
	{
		for(unsigned int j=1;j<this->gridMap.GetWidth();j+=2)
		{
			this->gridMap.SetPixel(j,i,sf::Color(0,255,0,0));
		}
	}
	
	//the minimap
	this->minimap.Create(256,256,sf::Color(0,0,0,0));
	
	//uploads the blendmaps, filing the handle
	glActiveTexture(GL_TEXTURE1);
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
	glActiveTexture(GL_TEXTURE2);
	this->makeBlendMap(this->terrInf.blendmap2H,this->blendmap2);

	glActiveTexture(GL_TEXTURE11);
	this->makeBlendMap(this->gridTexHandle,this->gridMap);
	
	//for blendmap 1
	this->tex1.LoadFromFile("terrain/textures/set1/1.png");
	this->tex2.LoadFromFile("terrain/textures/set1/2.png");
	this->tex3.LoadFromFile("terrain/textures/set1/3.png");
	this->tex4.LoadFromFile("terrain/textures/set1/4.png");
	this->tex5.LoadFromFile("terrain/textures/set1/5.png");
	this->tex6.LoadFromFile("terrain/textures/set1/6.png");
	this->tex7.LoadFromFile("terrain/textures/set1/7.png");
	this->tex8.LoadFromFile("terrain/textures/set1/8.png");
	glActiveTexture(GL_TEXTURE3);
	this->terrInf.texH[0] = this->uploadTextureGFX(this->tex1);
	glActiveTexture(GL_TEXTURE4);
	this->terrInf.texH[1] = this->uploadTextureGFX(this->tex2);
	glActiveTexture(GL_TEXTURE5);
	this->terrInf.texH[2] = this->uploadTextureGFX(this->tex3);
	glActiveTexture(GL_TEXTURE6);
	this->terrInf.texH[3] = this->uploadTextureGFX(this->tex4);

	//for blendmap 2
	glActiveTexture(GL_TEXTURE7);
	this->terrInf.tex2H[0] = this->uploadTextureGFX(this->tex5);
	glActiveTexture(GL_TEXTURE8);
	this->terrInf.tex2H[1] = this->uploadTextureGFX(this->tex6);
	glActiveTexture(GL_TEXTURE9);
	this->terrInf.tex2H[2] = this->uploadTextureGFX(this->tex7);
	glActiveTexture(GL_TEXTURE10);
	this->terrInf.tex2H[3] = this->uploadTextureGFX(this->tex8);
	
	//the shader for the terrain
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
	this->TerrainShader.setUniform("gridMap",11);
	glUseProgram(0);
	
	//shader for drawing surface textures
	this->surfaceTexShader.compileShaderFromFile("model.vsh",GLSLShader::VERTEX);
	this->surfaceTexShader.compileShaderFromFile("model.fsh",GLSLShader::FRAGMENT);
	this->surfaceTexShader.bindAttribLocation(0,"vertexPosition");
	this->surfaceTexShader.bindAttribLocation(1,"vertexNormal");
	this->surfaceTexShader.bindAttribLocation(2,"vertexUv");
	
	if(debug)
		cout<<this->surfaceTexShader.log();
		
	this->surfaceTexShader.link();
	
	if(debug)
		cout<<this->surfaceTexShader.log();
		
	this->surfaceTexShader.use();
	this->surfaceTexShader.setUniform("tex1",0);
	
	//bbox
	this->surfaceBboxShader.compileShaderFromFile("bbox.vsh",GLSLShader::VERTEX);
	this->surfaceBboxShader.compileShaderFromFile("bbox.fsh",GLSLShader::FRAGMENT);
	this->surfaceBboxShader.bindAttribLocation(3,"bBoxCordsVertex");
	

	if(debug)
		cout<<this->surfaceBboxShader.log();
		
	this->surfaceBboxShader.link();
	
	if(debug)
		cout<<this->surfaceBboxShader.log();
	glUseProgram(0);

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
	SurfaceTex tmp;
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures.push_back(tmp);
	this->surfacesTextures[0].init("terrain/textures/set1/","s1.png");
	this->surfacesTextures[1].init("terrain/textures/set1/","s2.png");
	this->surfacesTextures[2].init("terrain/textures/set1/","s3.png");
	this->surfacesTextures[3].init("terrain/textures/set1/","s4.png");
	this->surfacesTextures[4].init("terrain/textures/set1/","s5.png");
	this->surfacesTextures[5].init("terrain/textures/set1/","s6.png");
	this->surfacesTextures[6].init("terrain/textures/set1/","s7.png");
	this->surfacesTextures[7].init("terrain/textures/set1/","s8.png");
	this->surfacesTextures[8].init("terrain/textures/set1/","s9.png");
	this->surfacesTextures[9].init("terrain/textures/set1/","s10.png");
}

void Terrain::showCircle()
{
	this->drawCircle=true;
}

void Terrain::hideCircle()
{
	this->drawCircle=false;
}

void Terrain::draw()
{
	mat4 mvp=mat4(0.0f);
	//rendering terrain
	//add modelmatrix if needed(should not be needed)
	mvp=this->projMatrix*this->viewMatrix;
	//terrain doesnt support any scaling etc
	mat4 modelMatrix(1.0f);
	this->TerrainShader.use();
	this->TerrainShader.setUniform("modelMatrix",modelMatrix);
	this->TerrainShader.setUniform("MVP",mvp);
	
	//draws ground plane
	glBindVertexArray(this->vaoh);
	glDrawArrays(GL_TRIANGLES,0,6);
	
	
	this->surfaceTexShader.use();
	this->surfaceTexShader.setUniform("outAlpha",1.0f);
	glActiveTexture(GL_TEXTURE0);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for(unsigned int j=0;j<this->surfacesTextures.size();j++)
	{
		glBindVertexArray(this->surfacesTextures[j].getVaoH());
		for(unsigned int i=0; i<this->surfacesTextures[j].getModelMatrices()->size();i++)
		{			
			mvp=this->projMatrix*this->viewMatrix*this->surfacesTextures[j].getModelMatrices()->at(i);
			this->surfaceTexShader.setUniform("normalMatrix",mat3(this->surfacesTextures[j].getModelMatrices()->at(i)));
			this->surfaceTexShader.setUniform("MVP",mvp);
			glBindTexture(GL_TEXTURE_2D,this->surfacesTextures[j].getTexHandle());
			glDrawArrays(GL_TRIANGLES,0,6);
		}
	}
	
	this->surfaceBboxShader.use();
	this->surfaceBboxShader.setUniform("ro",0.0f);
	this->surfaceBboxShader.setUniform("go",1.0f);
	this->surfaceBboxShader.setUniform("bo",0.0f);
	glLineWidth(2);
	for(unsigned int j=0;j<this->surfacesTextures.size();j++)
	{		
		for(unsigned int i=0; i<this->surfacesTextures[j].getModelMatrices()->size();i++)
		{
			if(this->surfacesTextures[j].getDrawBbox()->at(i))
			{
				mvp=this->projMatrix*this->viewMatrix*this->surfacesTextures[j].getModelMatrices()->at(i);
				this->surfaceTexShader.setUniform("MVP",mvp);
				glDrawArrays(GL_LINE_STRIP,0,5);
			}
		}
	}
	
	glLineWidth(4);
	this->surfaceBboxShader.setUniform("ro",0.1f);
	this->surfaceBboxShader.setUniform("go",0.6f);
	this->surfaceBboxShader.setUniform("bo",0.8f);
	if(this->drawCircle)
	{
		this->radiusMarker.setPos(vec3(this->worldClickX,0,-this->worldClickZ));
		mat4 mvp = this->projMatrix*this->viewMatrix*translate(vec3(this->worldClickX,0,-this->worldClickZ))*scale(mat4(1.0f),vec3(this->radiusMarker.getScale()));
		this->surfaceTexShader.setUniform("MVP",mvp);
		glBindVertexArray(this->radiusMarker.getVaoh());
		glDrawArrays(GL_LINE_STRIP,0,this->radiusMarker.getNrOfLines());
	}
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glBindVertexArray(0);
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

void Terrain::replacePartTexture(GLint xoffset, GLint yoffset, sf::Image img,GLuint handle)
{
	glBindTexture(GL_TEXTURE_2D,handle);
	glTexSubImage2D(GL_TEXTURE_2D,0,xoffset,yoffset,img.GetWidth(),img.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,img.GetPixelsPtr());
}

Terrain::~Terrain()
{
	glDeleteBuffers(4,&this->vbohs[4]);
	glDeleteVertexArrays(1, &this->vaoh);
	glDeleteTextures(4,this->terrInf.texH);
	glDeleteTextures(4,this->terrInf.tex2H);
	glDeleteTextures(1,&this->terrInf.blendmap1H);
	glDeleteTextures(1,&this->terrInf.blendmap2H);
}

TerrainInfo *Terrain::getTerrInfo()
{
	return &this->terrInf;
}

void Terrain::rayIntersectTerrain(vec3 origin, vec3 ray, float &x, float &y)
{
	//one of the triangles
	vec3 v1= vec3(0.0f,0.0f,0.0f);
	vec3 v2= vec3(0.0f,0.0f,-this->height);
	vec3 v3= vec3(this->width,0.0f,0.0f);
	vec3 hit = intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
	//the other triangle
	x=hit.x*this->width;
	y=(hit.z*this->height);
	
	//if the first ray missed the first triangle, try the other one
	if(hit.x==1)
	{
		v1= vec3(this->width,0.0f,-this->height);
		v2= vec3(this->width,0.0f,0.0f);
		v3= vec3(0.0f,0.0f,-this->height);
		hit=intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
		x=this->width-hit.x*this->width;
		y=this->height+(-hit.z*this->height);
	}
}


void Terrain::paint(vec3 origin, vec3 ray)
{
	if(state == TerrState::PAINT)
	{
		int x=0;
		int y=0;
		//one of the triangles
		vec3 v1= vec3(0.0f,0.0f,0.0f);
		vec3 v2= vec3(0.0f,0.0f,-this->height);
		vec3 v3= vec3(this->width,0.0f,0.0f);
		vec3 hit = intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
		//the other triangle
		x=hit.x*256*this->blendsc;
		y=(hit.z*256*this->blendsc);
		
		//if the first ray missed the first triangle, try the other one
		if(hit.x==1)
		{
			v1= vec3(this->width,0.0f,-this->height);
			v2= vec3(this->width,0.0f,0.0f);
			v3= vec3(0.0f,0.0f,-this->height);
			hit=intersect.rayIntersectTriangle(origin,ray,v1,v2,v3);
			x=256*this->blendsc-hit.x*256*this->blendsc;
			y=256*this->blendsc+(-hit.z*256*this->blendsc);
		}
		

			//if the the ray hit any of the triangles
		//go through the blendmaps and update blend values
		if(x!=1)
		{
			//lowest x,y values, in case of drawing partially outside
			//used to create a new image which can replace parts of the texture on the GFX side
			int lowestY=y-this->radius;
			int lowestX=x-this->radius;
			int highestY=y+this->radius;
			int highestX=x+this->radius;
			
			if(lowestY<0)
				lowestY=0;
			if(lowestX<0)
				lowestX=0;
			if(highestX>256*this->blendsc)
				highestX=256*this->blendsc;
			if(highestY>256*this->blendsc)
				highestY=256*this->blendsc;
			
			sf::Image bmp1;
			sf::Image bmp2;
			
			bmp1.Create(highestX-lowestX,highestY-lowestY,sf::Color(0,0,0,0));
			bmp2.Create(highestX-lowestX,highestY-lowestY,sf::Color(0,0,0,0));
			
			//so you cant change the map with close to 0 radius
			if(bmp1.GetWidth()>0)
			{
				//counter for the sub image
				int subX=0;
				int subY=0;
				
					//creates a area to look through the blendmap to inrease speed
					for(int i=y-this->radius;i<y+this->radius;i++)
					{
						for(int j=x-this->radius;j<x+this->radius;j++)
						{
							//if the cordinats are inside the bounds
							if(j>=0&&i>=0&&j<256*this->blendsc&&i<256*this->blendsc)
							{
								//first updates the pixel in the part texture that will replace
								//the texture on the gfx, with the "global blendmap"
								sf::Color pix1=this->blendmap1.GetPixel(j,i);
								sf::Color pix2=this->blendmap2.GetPixel(j,i);
								bmp1.SetPixel(subX,subY,pix1);
								bmp2.SetPixel(subX,subY,pix2);
								if(this->inCircle(x,y,j,i))
								{
									this->increasePixelPaint(pix1,pix2,this->activeTex,sqrt((float)((j-x)*(j-x)+(i-y)*(i-y))));
									//updates the small texture thats going to replace pixels in the GFX tex
									bmp1.SetPixel(subX,subY,pix1);
									bmp2.SetPixel(subX,subY,pix2);
									//and updates the global blendmap
									this->blendmap1.SetPixel(j,i,pix1);
									this->blendmap2.SetPixel(j,i,pix2);
								}
								subX++;
							}
						}
						//if inside volume, counter y is increase so the sub img(bmp1/2) is correct
						if(i>=0&&i<=256*this->blendsc)
							subY++;
						subX=0;
					}

				//should be changes to upload just the data changed and not the whole blendmap
				//glActiveTexture(GL_TEXTURE1);
				this->replacePartTexture(lowestX,lowestY,bmp1,this->terrInf.blendmap1H);
				//this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
				//glActiveTexture(GL_TEXTURE2);
				this->replacePartTexture(lowestX,lowestY,bmp2,this->terrInf.blendmap2H);
				//this->makeBlendMap(this->terrInf.blendmap2H,this->blendmap2);
			}
		}
	}
}

//gets two pixels, calculates if the  choosen blendindex can increase, and in that case, increases it and decreses another pixels blend value
void Terrain::increasePixelPaint(sf::Color &pix1, sf::Color &pix2,int blendIndex,float distance)
{
	//translates pixels color data to array for convinience
	int pixBlends[]=
	{
		pix1.r,pix1.g,pix1.b,pix1.a,
		pix2.r,pix2.g,pix2.b,pix2.a
	};

		//if the distance was small(t), it resulten in a pixel that got more color then the others
		float dist=distance;
		//if(dist<0.1)
		//	dist=0.1;
		pixBlends[blendIndex]+=(255*this->opacity)+(((this->radius)-dist)*this->dropoff);
		
		float sum = 0;
		
		for(int i = 0; i < 8; i++)
		{
			sum += pixBlends[i];
		}
		
		//translates the array back to pixs
		pix1.r=(pixBlends[0] / sum) * 255;
		pix1.g=(pixBlends[1] / sum) * 255;
		pix1.b=(pixBlends[2] / sum) * 255;
		pix1.a=(pixBlends[3] / sum) * 255;
		
		pix2.r=(pixBlends[4] / sum) * 255;
		pix2.g=(pixBlends[5] / sum) * 255;
		pix2.b=(pixBlends[6] / sum) * 255;
		pix2.a=(pixBlends[7] / sum) * 255;
}

bool Terrain::inCircle(float cx, float cy, float x, float y)
{
	float dist;
	dist = (cx-x)*(cx-x)+(cy-y)*(cy-y);
	return dist<this->radius*this->radius;
}

void Terrain::setRadius(float rad)
{
	this->radius=rad*100;
	this->radiusMarker.setScale((rad*100)/(16));
}
void Terrain::setOpacity(float opa)
{
	this->opacity=opa;
}
void Terrain::setDropoff(float d)
{
	this->dropoff=d;
}

void Terrain::setActiveTex(int tex)
{
	this->activeTex=tex;
}

void Terrain::save(string path, string filename)
{
	string fullName=path+filename+".txt";
	ofstream out(fullName.c_str());
	out << "width: " << this->width << endl;
	out << "height: " <<  this->height << endl;
	out << "bmp1: " <<filename<<"bmp1.png"<<endl;
	out << "bmp2: " <<filename<<"bmp2.png"<<endl;
	out << "minimap: " << filename << "minimap.png"<<endl;
	
	
	string p1 = path+filename+"bmp1"+".png";
	string p2 = path+filename+"bmp2"+".png";
	string p3 = path+filename+"minimap"+".png";
	
	//swaps the images for exporting
	this->makeMiniMap();
	this->swapImg(this->minimap);
	this->swapImg(this->blendmap1);
	this->swapImg(this->blendmap2);

	this->blendmap1.SaveToFile(p1.c_str());
	this->blendmap2.SaveToFile(p2.c_str());
	this->minimap.SaveToFile(p3);
	
	//swaps back the blendmaps
	this->swapImg(this->blendmap1);
	this->swapImg(this->blendmap2);
	
	//starts saving the surface planes
	out << endl<<"Surfaceplanes: (format rotY, posX,posZ, scale ) "<<endl;
	for(unsigned int j=0;j<this->surfacesTextures.size();j++)
	{
		out << "SF: " << this->surfacesTextures[j].getName()<<endl;
		for(unsigned int i=0;i<this->surfacesTextures[j].getRotations()->size();i++)
		{
			out << this->surfacesTextures[j].getRotations()->at(i) << " " <<
			this->surfacesTextures[j].getPositions()->at(i).x << " " <<
			this->surfacesTextures[j].getPositions()->at(i).z << " " <<
			this->surfacesTextures[j].getScales()->at(i)<<endl;
		}
	}
	out << "end"<<endl;
	
	out << "GRID "<<endl;
	out << "width, height " << this->gridMap.GetWidth()/2<<" " << this->gridMap.GetHeight()/2<<endl;
	for(unsigned int j=1;j<this->gridMap.GetHeight()-1;j+=2)
	{
		for(unsigned int i=1;i<this->gridMap.GetWidth()-1;i+=2)
		{
			if(this->gridMap.GetPixel(i,j).r>0)
				out << 1;
			else
				out << 0;
		}
		out << endl;
	}
	out<<"end"<<endl;
	out.close();
	
}

void Terrain::makeMiniMap()
{
	float ratioX=0;
	float ratioY=0;
	
	
	ratioX=this->blendmap1.GetWidth()/this->minimap.GetWidth();
	ratioY=this->blendmap1.GetHeight()/this->minimap.GetHeight();
	
	float gmRatioX=this->gridMap.GetWidth()/this->minimap.GetWidth();
	float gmRatioY=this->gridMap.GetHeight()/this->minimap.GetHeight();
	
	tex1Avg = this->getAverageTexColor(tex1);
	tex2Avg = this->getAverageTexColor(tex2);
	tex3Avg = this->getAverageTexColor(tex3);
	tex4Avg = this->getAverageTexColor(tex4);
	tex5Avg = this->getAverageTexColor(tex5);
	tex6Avg = this->getAverageTexColor(tex6);
	tex7Avg = this->getAverageTexColor(tex7);
	tex8Avg = this->getAverageTexColor(tex8);
	

	for(unsigned int i=0;i<this->minimap.GetHeight();i++)
	{
		for(unsigned int j=0;j<this->minimap.GetWidth();j++)
		{
			this->minimap.SetPixel(j,i,this->getSample(j,i,ratioX,ratioY));
			int stepCorrection=0;
			if(int(j*gmRatioX)%2==0)
				stepCorrection=1;
			if(this->gridMap.GetPixel(j*gmRatioX+stepCorrection,i*gmRatioY+stepCorrection).a>0)
				this->minimap.SetPixel(j,i,this->gridMap.GetPixel(j*gmRatioX+stepCorrection,i*gmRatioY+stepCorrection));
		}
	}
}

GLuint Terrain::uploadTextureGFX(sf::Image img)
{
	GLuint handle;
	glGenTextures(1,&handle);
	glBindTexture(GL_TEXTURE_2D,handle);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.GetWidth(),img.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,img.GetPixelsPtr());
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST  );
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap( GL_TEXTURE_2D);
	
	return handle;
}

sf::Color Terrain::getSample(int x, int y,float xrat, float yrat)
{
	sf::Color outColor;
	outColor.r=int(this->tex1Avg.r)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).r))/255;
	outColor.g=int(this->tex1Avg.g)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).r))/255;
	outColor.b=int(this->tex1Avg.b)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).r)/255);
	
	outColor.r+=int(this->tex2Avg.r)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).g))/255;
	outColor.g+=int(this->tex2Avg.g)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).g))/255;
	outColor.b+=int(this->tex2Avg.b)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).g))/255;
	
	outColor.r+=int(this->tex3Avg.r)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).b))/255;
	outColor.g+=int(this->tex3Avg.g)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).b))/255;
	outColor.b+=int(this->tex3Avg.b)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).b))/255;
	
	outColor.r+=int(this->tex4Avg.r)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).a))/255;
	outColor.g+=int(this->tex4Avg.g)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).a))/255;
	outColor.b+=int(this->tex4Avg.b)*(int(this->blendmap1.GetPixel(x*xrat,y*yrat).a))/255;
	
	outColor.r+=int(this->tex5Avg.r)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).r))/255;
	outColor.g+=int(this->tex5Avg.g)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).r))/255;
	outColor.b+=int(this->tex5Avg.b)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).r)/255);
	
	outColor.r+=int(this->tex6Avg.r)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).g))/255;
	outColor.g+=int(this->tex6Avg.g)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).g))/255;
	outColor.b+=int(this->tex6Avg.b)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).g))/255;
	
	outColor.r+=int(this->tex7Avg.r)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).b))/255;
	outColor.g+=int(this->tex7Avg.g)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).b))/255;
	outColor.b+=int(this->tex7Avg.b)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).b))/255;
	
	outColor.r+=int(this->tex8Avg.r)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).a))/255;
	outColor.g+=int(this->tex8Avg.g)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).a))/255;
	outColor.b+=int(this->tex8Avg.b)*(int(this->blendmap2.GetPixel(x*xrat,y*yrat).a))/255;

	return outColor;
}

sf::Color Terrain::getAverageTexColor(sf::Image &img)
{
	unsigned long r=0;
	unsigned long g=0;
	unsigned long b=0;
	for(unsigned int i=0;i<img.GetHeight();i++)
	{
		for(unsigned int j=0;j<img.GetWidth();j++)
		{
			r+=long(img.GetPixel(j,i).r);
			g+=long(img.GetPixel(j,i).g);
			b+=long(img.GetPixel(j,i).b);
		}
	}
	r=r/(img.GetHeight()*img.GetWidth());
	g=g/(img.GetHeight()*img.GetWidth());
	b=b/(img.GetHeight()*img.GetWidth());

	return sf::Color(r,g,b,0);
}

void Terrain::swapImg(sf::Image &img)
{
	for(unsigned int i=0;i<img.GetWidth();i++)
	{
		for(unsigned int j=0;j<img.GetHeight()/2;j++)
		{
			sf::Color tmp = img.GetPixel(i,img.GetHeight()-1-j);
			img.SetPixel(i,img.GetHeight()-1-j,img.GetPixel(i,j));
			img.SetPixel(i,j,tmp);
		}
	}
}

void Terrain::updateViewMatrix(mat4 viewMatrix)
{
	this->viewMatrix=viewMatrix;
	this->TerrainShader.use();
	this->TerrainShader.setUniform("viewMatrix",this->viewMatrix);
	
	this->surfaceTexShader.use();
	this->surfaceTexShader.setUniform("viewMatrix",this->viewMatrix);
	glUseProgram(0);
}

void Terrain::updateProjMatrix(float width, float height)
{
	float nearClip = 0.5f;
	float farClip  = 1000.0f;
	float fov_deg = 45.0f;
	float aspect = (float)width/(float)height;
	this->projMatrix=perspective(fov_deg, aspect,nearClip,farClip);

	this->TerrainShader.use();
	this->TerrainShader.setUniform("projectionMatrix",this->projMatrix);
	
	this->surfaceTexShader.use();
	this->surfaceTexShader.setUniform("projectionMatrix",this->projMatrix);
	
	glUseProgram(0);
}
void Terrain::drawSurface(vec3 origin, vec3 ray, int id,bool isMouseDown)
{
	if(id>=0&&id<this->surfacesTextures.size())
	{
		float x=0;
		float z=0;
		this->rayIntersectTerrain(origin,ray,x,z);
		
		if(x>0)
		{
			//calculates position and rotation for the surfaceplanes
			vec3 pos;
			pos.x=x;
			pos.y=0;
			pos.z=-z;

			float dx = pos.x-this->worldClickX;
			float dz = -pos.z-this->worldClickZ;//needs to turn z, since i turned it b4
			float a = atan2(dz,dx)*180/PI+90;

			SurfaceTex t;
			if(isMouseDown)
			{
				t.addSurface(a,vec3(this->worldClickX,0,-this->worldClickZ),this->surfaceScale);
				
				glLineWidth(4);
			}
			else
				t.addSurface(0,pos,this->surfaceScale);
				
			this->surfaceTexShader.use();
			this->surfaceTexShader.setUniform("outAlpha",1.0f);
			glActiveTexture(GL_TEXTURE0);
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);


			glBindVertexArray(this->surfacesTextures[id].getVaoH());

			mat4 mvp=this->projMatrix*this->viewMatrix*t.getModelMatrices()->at(0);
			this->surfaceTexShader.setUniform("normalMatrix",mat3(t.getModelMatrices()->at(0)));
			this->surfaceTexShader.setUniform("MVP",mvp);
			glBindTexture(GL_TEXTURE_2D,this->surfacesTextures[id].getTexHandle());
			glDrawArrays(GL_TRIANGLES,0,6);
			
			if(isMouseDown)
			{
				this->surfaceBboxShader.use();
				this->surfaceBboxShader.setUniform("ro",0.7f);
				this->surfaceBboxShader.setUniform("go",0.3f);
				this->surfaceBboxShader.setUniform("bo",0.4f);

				this->radiusMarker.setPos(vec3(this->worldClickX,0,-this->worldClickZ));
				mat4 mvp = this->projMatrix*this->viewMatrix*translate(vec3(this->worldClickX,0,-this->worldClickZ))*scale(mat4(1.0f),vec3(this->roadSpacing*this->surfaceScale));
				this->surfaceTexShader.setUniform("MVP",mvp);
				glBindVertexArray(this->radiusMarker.getVaoh());
				glDrawArrays(GL_LINE_STRIP,0,this->radiusMarker.getNrOfLines());
			}

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glUseProgram(0);
			glBindVertexArray(0);
		}
	}
}
void Terrain::addSurface(vec3 origin, vec3 ray, int id)
{
	if(id>=0&&id<this->surfacesTextures.size())
	{
		float x=0;
		float z=0;
		this->rayIntersectTerrain(origin,ray,x,z);
		
		if(!this->inCircle(this->worldClickX,this->worldClickZ,x,z,this->roadSpacing*this->surfaceScale))
		{
			if(x>0)
			{
				//calculates position and rotation for the surfaceplanes
				vec3 pos;
				pos.x=x;
				pos.y=0;
				pos.z=-z;

				float dx = pos.x-this->worldClickX;
				float dz = -pos.z-this->worldClickZ;//needs to turn z, since i turned it b4
				float a = atan2(dz,dx)*180/PI+90;

				this->surfacesTextures[id].addSurface(a,vec3(this->worldClickX,0,-this->worldClickZ),this->surfaceScale);
				this->worldClickX=x;
				this->worldClickZ=z;
			}
		}
	}
}
void Terrain::setWorldXY(vec3 origin, vec3 ray)
{
	this->rayIntersectTerrain(origin,ray,this->worldClickX,this->worldClickZ);
}

bool Terrain::selectTexSurfaces(float radius, vec3 origin, vec3 ray)
{
	this->setWorldXY(origin,ray);
	bool found=false;
	for(unsigned int j=0;j<this->surfacesTextures.size();j++)
	{
		for(unsigned int i=0;i<this->surfacesTextures[j].getPositions()->size();i++)
		{
			if(this->inCircle(this->worldClickX,-this->worldClickZ, this->surfacesTextures[j].getPositions()->at(i).x,this->surfacesTextures[j].getPositions()->at(i).z,radius*this->surfacesTextures[j].getScales()->at(i)))
			{
				this->surfacesTextures[j].select(i);
				found = true;
			}
		}
	}
	return found;
}

void Terrain::deselectAllSurfaceTex()
{
	for(unsigned int j=0;j<this->surfacesTextures.size();j++)
	{
		for(unsigned int i=0;i<this->surfacesTextures[j].getPositions()->size();i++)
		{
			this->surfacesTextures[j].deSelect(i);
		}
	}
}

bool Terrain::inCircle(float cx, float cy, float x, float y,float rad)
{
	float dist;
	dist = (cx-x)*(cx-x)+(cy-y)*(cy-y);
	return dist<rad*rad;
}

void Terrain::setTerState(TerrState::TerrStates state)
{
	this->state = state;
}

vector<GLuint> Terrain::getSurfaceTexHandles()
{
	vector<GLuint> th;
	for(unsigned int j=0;j<this->surfacesTextures.size();j++)
	{
		th.push_back(this->surfacesTextures[j].getTexHandle());
	}
	return th;
}

void Terrain::removeSelectedSurfaces()
{
	for(unsigned int j=0; j<this->surfacesTextures.size();j++)
	{
		for(unsigned int i=0;i<this->surfacesTextures[j].getPositions()->size();i++)
		{
			if(this->surfacesTextures[j].isSelected(i))
			{
				this->surfacesTextures[j].remove(i);
				//lowers the index, since the remove function removes an element
				//if not, it would cause to make a miss in the vector
				i--;
			}
		}
	}
}

void Terrain::showHideGridMap()
{
	if(this->showGridMap)
	{
		this->showGridMap=false;
		this->TerrainShader.use();
		this->TerrainShader.setUniform("showGM",false);
	}
	else
	{
		this->showGridMap=true;
		this->TerrainShader.use();
		this->TerrainShader.setUniform("showGM",true);
	}
}

void Terrain::makeGridUnderModel(Model m)
{
	//4 is an offset, if the buldings doesnt fit , increase 4
	int minX = (this->worldClickX-4)*(this->gridMap.GetWidth()/this->width);
	int minY = (this->worldClickZ-4)*(this->gridMap.GetHeight()/this->height);
	int maxX=(this->worldClickX+4)*(this->gridMap.GetWidth()/this->width);
	int maxY=(this->worldClickZ+4)*(this->gridMap.GetHeight()/this->height);
	if(maxY>this->gridMap.GetHeight())
		maxY=this->gridMap.GetHeight();
	if(maxX>this->gridMap.GetWidth())
		maxX=this->gridMap.GetHeight();
	if(minY<2)
		minY=1;
	if(minY%2==0)
		minY+=1;
	if(maxY%2==0)
		maxY-=1;

	if(minX<2)
		minX=1;
	if(minX%2==0)
		minX+=1;
	if(maxX%2==0)
		maxX-=1;
	

	for(int j=minY;j<maxY;j+=2)
	{
		for(int i=minX;i<maxX;i+=2)
		{
			bool found=true;
			float t1=0;
			float t2=0;

			//grid propertis
			float wogmw = width/gridMap.GetWidth();
			float hogmh = height/gridMap.GetHeight();
			float gmwow = gridMap.GetWidth()/width;
			float gmhoh = gridMap.GetHeight()/height;
			float gmwowo2 = wogmw/2;
			float gmhoho2 = hogmh/2;

			vec4 mm = inverse(mat4(m.getModelMatrix()))*vec4(vec3(gmwowo2+(i+wogmw)/gmwow,0,-gmhoho2+(-j-hogmh)/gmhoh),1);
			vec3 t = vec3(gmwowo2+(i+wogmw)/gmwow,1,-gmhoho2+(-j-hogmh)/gmhoh)-vec3(gmwowo2+(i+wogmw)/gmwow,0,-gmhoho2+(-j-hogmh)/gmhoh);
			vec3 rayd=inverse(mat3(m.getModelMatrix()))*t;
			rayd = (rayd);


			float sides[3] =
			{
				m.getBoundingBox()->getBboxSide().x+0.1,
				m.getBoundingBox()->getBboxSide().y,
				m.getBoundingBox()->getBboxSide().z+0.1
			};
			
			vec3 normalizedSides[3] = 
			{
				vec3(1,0,0),
				vec3(0,1,0),
				vec3(0,0,1)
			};
			
			float tmin=-0.000001;
			float tmax=99999999;
			

			vec3 p = m.getBoundingBox()->getBboxPos()-vec3(mm.x/mm.w,mm.y/mm.w,mm.z/mm.w);
			
			for(int k=0;k<3;k++)
			{
				float e = dot(normalizedSides[k],p);
				float f = dot(normalizedSides[k],rayd);
				if(abs(f)>0.000001)
				{
					t1=(e+sides[k])/f;
					t2=(e-sides[k])/f;
					if(t1>t2)
					{
						float tmp=t1;
						t1=t2;
						t2=tmp;
					}
					if(t1>tmin)
						tmin=t1;
					if(t2<tmax)
						tmax=t2;
					if(tmin>tmax)
						found=false;
					if(tmax<0)
						found=false;
				}
				else if(-e-sides[k]>0 || -e+sides[k]<0)
					found=false;
			}
			if(found)
			{
				this->gridMap.SetPixel(i,j,sf::Color(120,120,255,255));
			}
		}
	}
	glActiveTexture(GL_TEXTURE11);
	this->makeBlendMap(this->gridTexHandle,this->gridMap);

}

void Terrain::recalcGridAroundModel(vector<Model> removedModels, vector<Model> models)
{
	for(unsigned int o=0;o<removedModels.size();o++)
	{
		
		float rad= removedModels[o].getBoundingBox()->getBboxSide().z+2;
		if(rad<removedModels[o].getBoundingBox()->getBboxSide().x)
			rad=rad<removedModels[o].getBoundingBox()->getBboxSide().x+2;
		
		rad*=1.5;
		int minX = (removedModels[o].getPos().x-rad)*(this->gridMap.GetWidth()/this->width)-1;
		int minY = (-removedModels[o].getPos().z-rad)*(this->gridMap.GetHeight()/this->height)-1;
		int maxX=(removedModels[o].getPos().x+rad)*(this->gridMap.GetWidth()/this->width)+1;
		int maxY=(-removedModels[o].getPos().z+rad)*(this->gridMap.GetHeight()/this->height)+1;
		if(maxY>this->gridMap.GetHeight())
			maxY=this->gridMap.GetHeight();
		if(maxX>this->gridMap.GetWidth())
			maxX=this->gridMap.GetHeight();
		if(minY<2)
			minY=1;
		if(minY%2==0)
			minY+=1;
		if(maxY%2==0)
			maxY-=1;

		if(minX<2)
			minX=1;
		if(minX%2==0)
			minX+=1;
		if(maxX%2==0)
			maxX-=1;
		
		for(int j=minY;j<maxY;j+=2)
		{
			for(int i=minX;i<maxX;i+=2)
			{
				this->gridMap.SetPixel(i,j,sf::Color(0,255,0,0));
			}
		}
		for(int z=0;z<models.size();z++)
		{
			if(this->inCircle(removedModels[o].getPos().x,-removedModels[o].getPos().z,models[z].getPos().x,-models[z].getPos().z,rad*2.5))
			{
					for(int j=1;j<maxY;j+=2)
					{
						for(int i=1;i<maxX;i+=2)
						{
							bool found=true;
							float t1=0;
							float t2=0;

							//grid propertis
							float wogmw = width/gridMap.GetWidth();
							float hogmh = height/gridMap.GetHeight();
							float gmwow = gridMap.GetWidth()/width;
							float gmhoh = gridMap.GetHeight()/height;
							float gmwowo2 = wogmw/2;
							float gmhoho2 = hogmh/2;

							vec4 mm = inverse(mat4(models[z].getModelMatrix()))*vec4(vec3(gmwowo2+(i+wogmw)/gmwow,0,-gmhoho2+(-j-hogmh)/gmhoh),1);
							vec3 t = vec3(gmwowo2+(i+wogmw)/gmwow,1,-gmhoho2+(-j-hogmh)/gmhoh)-vec3(gmwowo2+(i+wogmw)/gmwow,0,-gmhoho2+(-j-hogmh)/gmhoh);
							vec3 rayd=inverse(mat3(models[z].getModelMatrix()))*t;
							rayd = (rayd);


							float sides[3] =
							{
								models[z].getBoundingBox()->getBboxSide().x+0.1,
								models[z].getBoundingBox()->getBboxSide().y,
								models[z].getBoundingBox()->getBboxSide().z+0.1
							};
							
							vec3 normalizedSides[3] = 
							{
								vec3(1,0,0),
								vec3(0,1,0),
								vec3(0,0,1)
							};
							
							float tmin=-0.000001;
							float tmax=99999999;
							

							vec3 p = models[z].getBoundingBox()->getBboxPos()-vec3(mm.x/mm.w,mm.y/mm.w,mm.z/mm.w);
							
							for(int k=0;k<3;k++)
							{
								float e = dot(normalizedSides[k],p);
								float f = dot(normalizedSides[k],rayd);
								if(abs(f)>0.000001)
								{
									t1=(e+sides[k])/f;
									t2=(e-sides[k])/f;
									if(t1>t2)
									{
										float tmp=t1;
										t1=t2;
										t2=tmp;
									}
									if(t1>tmin)
										tmin=t1;
									if(t2<tmax)
										tmax=t2;
									if(tmin>tmax)
										found=false;
									if(tmax<0)
										found=false;
								}
								else if(-e-sides[k]>0 || -e+sides[k]<0)
									found=false;
							}
							if(found)
							{
								this->gridMap.SetPixel(i,j,sf::Color(120,120,255,255));
							}
						}
					}
			}
		}
		
	}
	glActiveTexture(GL_TEXTURE11);
	this->makeBlendMap(this->gridTexHandle,this->gridMap);

}

void Terrain::setRoadSpacing(float f)
{
	this->roadSpacing=f;
}
void Terrain::setRoadScale(float f)
{
	this->surfaceScale=f;
}