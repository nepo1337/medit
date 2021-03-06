#include "MeshHandler.h"

MeshHandler::MeshHandler(string dir)
{
	vector<string> files;
	this->fileDirReader.getFilesInDir(dir,files);
	for(unsigned int i=0;i<files.size();i++)
	{
		if(this->getFileExtension(files[i])=="BFish")
		{
			Mesh m;
			if(m.import(dir,files[i]))
			{
				if(files[i][0]=='l'&&files[i][1]=='a'&&files[i][2]=='m'&&files[i][3]=='p')
					m.setType("light");
				this->meshes.push_back(m);
			}
		}
	}
}
void MeshHandler::load(string dir)
{
		vector<string> files;
	this->fileDirReader.getFilesInDir(dir,files);
	for(unsigned int i=0;i<files.size();i++)
	{
		if(this->getFileExtension(files[i])=="BFish")
		{
			Mesh m;
			if(m.import(dir,files[i]))
			{
				if(files[i][0]=='l'&&files[i][1]=='a'&&files[i][2]=='m'&&files[i][3]=='p')
					m.setType("light");
				this->meshes.push_back(m);
			}
		}
	}
}
MeshHandler::MeshHandler()
{
	
}
MeshHandler::~MeshHandler()
{
	for(unsigned int i=0;i<this->meshes.size();i++)
	{
		this->meshes[i].freeGFX();
	}
}

string MeshHandler::getFileExtension(string str)
{
	string ext="";
	bool save=false;
	for(unsigned int i=0;i<str.length();i++)
	{
		if(save)
				ext+=str[i];
		if(str[i]=='.')
			save=true;
	}
	return ext;
}

string MeshHandler::getFileName(string str)
{
	string fn;
	bool end=false;
	for(unsigned int i=0;i<str.length()&&!end;i++)
	{
		if(str[i]=='.')
			end=true;
		if(!end)
		fn+=str[i];
	}
	return fn;
}
vector<MeshInfo>* MeshHandler::getMeshInfo(int i)
{
	vector<MeshInfo>* mf=0;
	if(i>=0&&i<this->meshes.size())
	{
		mf = this->meshes[i].getMeshInfo();
	}
	return mf;
}

BoundingBox *MeshHandler::getBoundingBox(int i)
{
	return this->meshes[i].getBoundingBox();
}

int MeshHandler::getNrOfMeshes()
{
	return this->meshes.size();
}
string MeshHandler::getMeshName(int i)
{
	return this->meshes[i].getFilename();
}
string MeshHandler::getMeshType(int i)
{
	return this->meshes[i].getType();
}
int MeshHandler::getMeshByName(string name)
{
	int id=-1;
	for( int i=0;i<this->meshes.size();i++)
	{
		if(this->meshes[i].getFilename()==name)
			id=i;
	}
	return id;
}