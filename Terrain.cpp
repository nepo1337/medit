#include "Terrain.h"
#define PI 3.14159265

Terrain::Terrain(int size)
{
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
	
	this->terrInf.blendmap1H=0;
	this->terrInf.blendmap2H=0;
	this->gridTexHandle=0;
	
	//creating blendmaps
	this->blendmap1.Create(256*this->blendsc,256*this->blendsc,sf::Color(255,0,0,0));
	this->blendmap2.Create(256*this->blendsc,256*this->blendsc,sf::Color(0,0,0,0));
	
	//creates a gridmap
	this->gridMap.Create(512*this->blendsc/4,512*this->blendsc/4,sf::Color(0,0,0,0));
	
	//the minimap
	this->minimap.Create(256,256,sf::Color(0,0,0,0));

	this->gridMap.SetPixel(10,6,sf::Color(0,0,0,255));
	this->gridMap.SetPixel(0,0,sf::Color(0,0,0,255));
	this->gridMap.SetPixel(1,1,sf::Color(0,0,0,255));
	
	//uploads the blendmaps, filing the handle
	glActiveTexture(GL_TEXTURE1);
	this->makeBlendMap(this->terrInf.blendmap1H,this->blendmap1);
	glActiveTexture(GL_TEXTURE2);
	this->makeBlendMap(this->terrInf.blendmap2H,this->blendmap2);

	glActiveTexture(GL_TEXTURE11);
	this->makeBlendMap(this->gridTexHandle,this->gridMap);
	
	//for blendmap 1
	this->tex1.LoadFromFile("terrain/textures/set1/1.jpg");
	this->tex2.LoadFromFile("terrain/textures/set1/2.jpg");
	this->tex3.LoadFromFile("terrain/textures/set1/3.jpg");
	this->tex4.LoadFromFile("terrain/textures/set1/4.jpg");
	this->tex5.LoadFromFile("terrain/textures/set1/5.jpg");
	this->tex6.LoadFromFile("terrain/textures/set1/6.jpg");
	this->tex7.LoadFromFile("terrain/textures/set1/7.jpg");
	this->tex8.LoadFromFile("terrain/textures/set1/8.jpg");
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
	this->stoneSurface.init("terrain/textures/set1/stone.png");
	
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
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->stoneSurface.getVaoH());
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	this->surfaceTexShader.setUniform("outAlpha",1.0f);
	for(unsigned int i=0; i<this->stoneSurface.getRotations()->size();i++)
	{
		modelMatrix=mat4(1.0f);
		modelMatrix*=glm::translate(this->stoneSurface.getPositions()->at(i));
		modelMatrix*=glm::rotate(this->stoneSurface.getRotations()->at(i),glm::vec3(0.0f,1.0f,0.0f));
		
		mvp=this->projMatrix*this->viewMatrix*modelMatrix;
		this->surfaceTexShader.setUniform("normalMatrix",mat3(modelMatrix));
		this->surfaceTexShader.setUniform("MVP",mvp);
		glBindTexture(GL_TEXTURE_2D,this->stoneSurface.getTexHandle());
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	
	this->surfaceBboxShader.use();
	for(unsigned int i=0; i<this->stoneSurface.getRotations()->size();i++)
	{
		if(this->stoneSurface.getDrawBbox()->at(i))
		{
			modelMatrix=mat4(1.0f);
			modelMatrix*=glm::translate(this->stoneSurface.getPositions()->at(i));
			modelMatrix*=glm::rotate(this->stoneSurface.getRotations()->at(i),glm::vec3(0.0f,1.0f,0.0f));
			
			mvp=this->projMatrix*this->viewMatrix*modelMatrix;
			this->surfaceTexShader.setUniform("MVP",mvp);
			glDrawArrays(GL_LINE_STRIP,0,5);
		}
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
		if(dist<1)
			dist=1;
		pixBlends[blendIndex]+=(255*this->opacity/(dist*this->dropoff));
		
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
	out << endl<<"Surfaceplanes: (format rotY, posX,posZ) "<<endl;
	out << "SF: " << this->stoneSurface.getName()<<endl;
	for(int i=0;i<this->stoneSurface.getRotations()->size();i++)
	{
		out << this->stoneSurface.getRotations()->at(i) << " " <<
		this->stoneSurface.getPositions()->at(i).x << " " <<
		this->stoneSurface.getPositions()->at(i).z << " " <<endl;
	}
	out << "end"<<endl;
	out.close();
}

void Terrain::makeMiniMap()
{
	float ratioX=0;
	float ratioY=0;
	
	
	ratioX=this->blendmap1.GetWidth()/this->minimap.GetWidth();
	ratioY=this->blendmap1.GetWidth()/this->minimap.GetHeight();
	
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
			this->minimap.SetPixel(i,j,this->getSample(i,j,ratioX,ratioY));
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

	/*for(int i=x;i<x*xrat;i++)
	{
		for(int y=0;j<y*yrat;j++)
		{
			
		}
	}*/
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
void Terrain::addSurface(vec3 origin, vec3 ray, int id)
{
	float x=0;
	float z=0;
	this->rayIntersectTerrain(origin,ray,x,z);
	
	if(!this->inCircle(this->worldClickX,this->worldClickZ,x,z,0.9))
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

			this->stoneSurface.addSurface(a,pos);
			this->worldClickX=x;
			this->worldClickZ=z;
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
	for(unsigned int i=0;i<this->stoneSurface.getPositions()->size();i++)
	{
		if(this->inCircle(this->worldClickX,-this->worldClickZ, this->stoneSurface.getPositions()->at(i).x,this->stoneSurface.getPositions()->at(i).z,radius))
		{
			this->stoneSurface.select(i);
			found = true;
		}
	}
	return found;
}

void Terrain::deselectAllSurfaceTex()
{
	for(unsigned int i=0;i<this->stoneSurface.getPositions()->size();i++)
	{
		this->stoneSurface.deSelect(i);
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
	th.push_back(this->stoneSurface.getTexHandle());
	return th;
}

void Terrain::removeSelectedSurfaces()
{
	for(unsigned int i=0;i<this->stoneSurface.getPositions()->size();i++)
	{
		if(this->stoneSurface.isSelected(i))
		{
			this->stoneSurface.remove(i);
			//lowers the index, since the remove function removes an element
			//if not, it would cause to make a miss in the vector
			i--;
		}
	}
}