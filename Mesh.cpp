#include "Mesh.h"

Mesh::Mesh()
{
	this->debug=true;
	path="";
}

Mesh::~Mesh()
{
}

bool Mesh::import(string path,string filename)
{
	bool imported=false;
	this->path=path;
	string k = path+filename;
	//this->objimp.import(path,filename);
	this->fishImp.Read(k);
	imported = this->fishImp.getFishFile()->FMeshes.size()>0;
	if(imported)
	{
		this->setFilename(filename);
		this->uploadToGFX();
	}
	return imported;
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
	
	if(this->fishImp.getFishFile()->boundingVol.type==box)
	{
		vec3 bBoxPos= vec3(this->fishImp.getFishFile()->boundingVol.pos.x,this->fishImp.getFishFile()->boundingVol.pos.y,this->fishImp.getFishFile()->boundingVol.pos.z);
		vec3 bBoxVec= vec3(this->fishImp.getFishFile()->boundingVol.size.x,this->fishImp.getFishFile()->boundingVol.size.y,this->fishImp.getFishFile()->boundingVol.size.z);

		//calculates the boundingbox
		vec3 luc,luf,ldc,ldf, ruc,ruf,rdc,rdf ;
		//left side
		luc.x=bBoxPos.x - bBoxVec.x;
		luc.y=bBoxPos.y + bBoxVec.y;
		luc.z=bBoxPos.z - bBoxVec.z;
		
		luf.x=bBoxPos.x - bBoxVec.x;
		luf.y=bBoxPos.y + bBoxVec.y;
		luf.z=bBoxPos.z + bBoxVec.z;
		
		ldc.x=bBoxPos.x - bBoxVec.x;
		ldc.y=bBoxPos.y - bBoxVec.y;
		ldc.z=bBoxPos.z + bBoxVec.z;
		
		ldf.x=bBoxPos.x - bBoxVec.x;
		ldf.y=bBoxPos.y - bBoxVec.y;
		ldf.z=bBoxPos.z - bBoxVec.z;
		
		//right side
		ruc.x=bBoxPos.x + bBoxVec.x;
		ruc.y=bBoxPos.y + bBoxVec.y;
		ruc.z=bBoxPos.z - bBoxVec.z;
		
		ruf.x=bBoxPos.x + bBoxVec.x;
		ruf.y=bBoxPos.y + bBoxVec.y;
		ruf.z=bBoxPos.z + bBoxVec.z;
		
		rdc.x=bBoxPos.x + bBoxVec.x;
		rdc.y=bBoxPos.y - bBoxVec.y;
		rdc.z=bBoxPos.z + bBoxVec.z;
		
		rdf.x=bBoxPos.x + bBoxVec.x;
		rdf.y=bBoxPos.y - bBoxVec.y;
		rdf.z=bBoxPos.z - bBoxVec.z;
		
		float bVerts[]=
		{
			luc.x,luc.y,luc.z,
			luf.x,luf.y,luf.z,
			ldc.x,ldc.y,ldc.z,
			ldf.x,ldf.y,ldf.z,
			luc.x,luc.y,luc.z,
			ruc.x,ruc.y,ruc.z,
			ruf.x,ruf.y,ruf.z,
			rdc.x,rdc.y,rdc.z,
			rdf.x,rdf.y,rdf.z,
			ruc.x,ruc.y,ruc.z,
			ruf.x,ruf.y,ruf.z,
			luf.x,luf.y,luf.z,
			ldc.x,ldc.y,ldc.z,
			rdc.x,rdc.y,rdc.z,
			rdf.x,rdf.y,rdf.z,
			ldf.x,ldf.y,ldf.z
		};
		
		glGenVertexArrays(1,&this->bBoxVaoh);
		glGenBuffers(1,&this->bBoxVboh);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->bBoxVboh);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bVerts),bVerts,GL_STATIC_DRAW);
		
		glBindVertexArray(this->bBoxVaoh);
		glEnableVertexAttribArray(3);
		
		//vertex
		glBindBuffer(GL_ARRAY_BUFFER, this->bBoxVboh);
		glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);
		
		this->bBox.setBboxPos(bBoxPos);
		this->bBox.setBboxSide(bBoxVec);
		this->bBox.setVaoh(this->bBoxVaoh);
		
		
	}
	for(unsigned int i =0;i<this->fishImp.getFishFile()->FMeshes.size();i++)
	{
		//Object o = this->objimp.getObject(i);
		FishFile *fishF=this->fishImp.getFishFile();
		FMesh subMesh = fishF->FMeshes[i];
		string p;
		//textures
		for(unsigned int b=0;b<fishF->fMaterials[subMesh.materialIndex].textures.size();b++)
		{
			string papa="";
			
			for(int t=0;t<fishF->fMaterials[subMesh.materialIndex].textures[b].typeSize;t++)
			{
				//cout << "LALA"<<
				//fishF.fMaterials[subMesh.materialIndex].textures[b].type[t];//<<endl;
				papa+=fishF->fMaterials[subMesh.materialIndex].textures[b].type[t];
			}
				
			if(papa=="color")
			{
				string dada="";
				for(int t=0;t<fishF->fMaterials[subMesh.materialIndex].textures[b].pathSize;t++)
					dada+=fishF->fMaterials[subMesh.materialIndex].textures[b].path[t];
				p=this->path+dada;
			}
		}
		glActiveTexture(GL_TEXTURE0);
		GLuint texH = SOIL_load_OGL_texture(p.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS  | SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_TEXTURE_REPEATS);
		if(debug)
		{
			if(texH==0)
				cout<<"SOIL loading error: "<< SOIL_last_result()<<endl;
		}
		GLuint vaoH=0;
		glGenVertexArrays(1,&vaoH);
		MeshInfo mf;
		mf.setTexh(texH);
		mf.setVaoh(vaoH);
		//mf.setNrOfVerts(o.getNrOfVerts());
		mf.setNrOfVerts(subMesh.numVertices);
		this->meshesInfo.push_back(mf);
		GLuint vbohs[3]={0,0,0};
		glGenBuffers(3,vbohs);
		
		//UPLOAD DATA
		//vertex points
		vector<float> verts;
		vector<float> normals;
		vector<float> uvs;
		for(unsigned int b=0;b<subMesh.vertices.size();b++)
		{
			verts.push_back(subMesh.vertices[b].position.x);
			verts.push_back(subMesh.vertices[b].position.y);
			verts.push_back(subMesh.vertices[b].position.z);
		}
		
		for(unsigned int b=0;b<subMesh.vertices.size();b++)
		{
			normals.push_back(subMesh.vertices[b].normal.x);
			normals.push_back(subMesh.vertices[b].normal.y);
			normals.push_back(subMesh.vertices[b].normal.z);
		}
		
		for(unsigned int b=0;b<subMesh.vertices.size();b++)
		{
			uvs.push_back(subMesh.vertices[b].uv.u);
			uvs.push_back(subMesh.vertices[b].uv.v);
		}
		
		//calculates the boundingbox
		
		glBindBuffer(GL_ARRAY_BUFFER, vbohs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(),&verts[0],GL_STATIC_DRAW);
		
		//normals
		glBindBuffer(GL_ARRAY_BUFFER, vbohs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW);
		
		//uvs
		glBindBuffer(GL_ARRAY_BUFFER, vbohs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*uvs.size(), &uvs[0], GL_STATIC_DRAW);
		
		
		
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
	glDeleteBuffers(n,&this->bBoxVboh);
	glDeleteVertexArrays(1, &this->bBoxVaoh);
	for(unsigned int i =0;i<this->meshesInfo.size();i++)
	{
		GLuint h=this->meshesInfo[i].getVaoh();
		glDeleteVertexArrays(1, &h);
		GLuint t=this->meshesInfo[i].getTexh();
		glDeleteTextures(1,&t);
	}
	
}

BoundingBox* Mesh::getBoundingBox()
{
	return &this->bBox;
}