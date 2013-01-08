#include "Spritetext.h"

Spritetext::Spritetext()
{
	this->text="";
	this->x=0.0f;
	this->y=0.0f;
	this->size=1.0f;
	this->nrOfVerts=0;
	this->texHandle=0;
	this->vaoHandle=0;
}

Spritetext::~Spritetext()
{
	glDeleteTextures(1,&this->texHandle);
	glDeleteBuffers(2,this->vboH);
	glDeleteVertexArrays(1, &this->vaoHandle);
}

Spritetext::Spritetext(string text, int posX, int posY, float size,int screenResX, int screenResY,string spriteFile,int charWidth,int charHeight)
{
	this->text=text;
	this->posX=posX;
	this->posY=posY;
	this->size=size;	
	
	float charHeightNormalized, charWidthNormalized;
	float a,b,c,d;
	a = 2 * (float)screenResX/2 / screenResX - 1;
	b = 1 - 2 * (float)screenResY/2 / screenResY;
	c = 2 * ((float)screenResX/2+100) / screenResX - 1;
	d = 1 - 2 * ((float)screenResY/2+100) / screenResY;
	charHeightNormalized=b-d;
	charWidthNormalized=a-c;
	
	int nrOfLetters=this->spriteFile.GetWidth()/charWidth;
	
	//converts the position to homogenus clipspace
	this->x = 2 * (float)posX / screenResX - 1;
	this->y = 1 - 2 * (float)posY / screenResY;
	
	vector<float> verts;
	vector<float> uvs;
	float vert;
	float uv;
	
	//calcs the verts and uvs cords for a string
	float test;
	for(int i=0;i<this->text.length();i++)
	{
		if(int(this->text[i])>=65&&int(this->text[i])<=90 || int(this->text[i])>=97&&int(this->text[i])<=122)
		{
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			
			verts.push_back(0.0f);
			verts.push_back(0.5f);
			verts.push_back(0.0f);

			verts.push_back(0.5f);
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			
			verts.push_back(0.5f);
			verts.push_back(0.5f);
			verts.push_back(0.0f);
			
			verts.push_back(0.5f);
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			
			verts.push_back(0.0f);
			verts.push_back(0.5f);
			verts.push_back(0.0f);
			
			//if the user enters small letters
			int charCorr=0;
			if(int(this->text[i])>=97&&int(this->text[i])<=122)
			{
				charCorr=32;
			}
			/*
			uv=vec2(float((int(this->text[i])-65-charCorr))/float(nrOfLetters),0.0f);
			uvs.push_back(uv);
			uv=vec2(float((int(this->text[i])-65-charCorr))/float(nrOfLetters),1.0f);
			uvs.push_back(uv);
			uv=vec2(float((int(this->text[i])-65-charCorr)+1)/float(nrOfLetters),0.0f);
			uvs.push_back(uv);
			uv=vec2(float((int(this->text[i])-65-charCorr)+1)/float(nrOfLetters),1.0f);
			uvs.push_back(uv);
			uv=vec2(float((int(this->text[i])-65-charCorr)+1)/float(nrOfLetters),0.0f);
			uvs.push_back(uv);
			uv=vec2(float((int(this->text[i])-65-charCorr))/float(nrOfLetters),1.0f);
			uvs.push_back(uv);*/
			
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
			
			uvs.push_back(0.0f);
			uvs.push_back(1.0f);
			
			uvs.push_back(1.0f);
			uvs.push_back(0.0f);
			
			uvs.push_back(1.0f);
			uvs.push_back(1.0f);
			
			uvs.push_back(1.0f);
			uvs.push_back(0.0f);
			
			uvs.push_back(0.0f);
			uvs.push_back(1.0f);
		}
	}
	this->nrOfVerts=verts.size()/3;
	
	glGenVertexArrays(1,&this->vaoHandle);
	glGenBuffers(2,this->vboH);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboH[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(),&verts[0],GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboH[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*uvs.size(), &uvs[0], GL_STATIC_DRAW);
	
	glBindVertexArray(this->vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboH[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboH[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,NULL);
	
	this->spriteFile.LoadFromFile(spriteFile.c_str());
	glGenTextures(1,&this->texHandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,this->texHandle);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,this->spriteFile.GetWidth(),this->spriteFile.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,this->spriteFile.GetPixelsPtr());
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
}
string Spritetext::getText()
{
	return this->text;
}

int Spritetext::getXPos()
{
	return this->posX;
}
int Spritetext::getYPos()
{
	return this->posY;
}
float Spritetext::getSize()
{
	return this->size;
}

GLuint Spritetext::getVaoHandle()
{
	return this->vaoHandle;
}

int Spritetext::getNrOfVerts()
{
	return this->nrOfVerts;
}
GLuint Spritetext::getTexHandle()
{
	return this->texHandle;
}