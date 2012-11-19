#include "OBJimporter.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace glm;

OBJimporter::OBJimporter()
{
	this->nrOfObjects=0;
	mtllibfile="";
}

OBJimporter::~OBJimporter()
{

}

void OBJimporter::import(string path,string filename)
{
	ifstream file;
	string p=path+filename;
	file.open(p.c_str());
	string line,str1,str2,str3;
	
	//starts as true, so a new obj can be created as first vertex is found
	bool reachedEndObj=true;
	
	vector<vec3> vertex,normal,faces;
	vector<vec2> uv;
	
	while(file.good())
	{
		getline(file,line);
		line = this->stripStartSpaces(line);
		//mtllib file
		if(line[0]=='m')
		{
			mtllibfile = this->getStringAfterSpace(line);
		}
		
		//vertex points
		if(line[0]=='v'&&line[1]==' ')
		{
			if(reachedEndObj)
			{
				Object o;
				objects.push_back(o);
				reachedEndObj=false;
			}
			getStrings(line,str1,str2,str3);
			vertex.push_back(vec3(atof(str1.c_str()),atof(str2.c_str()),atof(str3.c_str())));
		}
		
		//vertex normals
		if(line[0]=='v'&&line[1]=='n')
		{
			getStrings(line,str1,str2,str3);
			normal.push_back(vec3(atof(str1.c_str()),atof(str2.c_str()),atof(str3.c_str())));
		}
		
		//vertex uvs
		if(line[0]=='v'&&line[1]=='t')
		{
			getStrings(line,str1,str2,str3);
			uv.push_back(vec2(atof(str1.c_str()),atof(str2.c_str())));
		}
		
		//usemtl for obj
		if(line[0]=='u')
		{
			string useMtl = this->getStringAfterSpace(line);
			objects[this->nrOfObjects].setUsemtl(useMtl);
		}
		
		//faces connected
		if(line[0]=='f'&&line[1]==' ')
		{
			if(!reachedEndObj)
			{
				reachedEndObj=true;
				nrOfObjects++;
			}
			getStrings(line,str1,str2,str3);
			string v,uvt,vn;
			//saves the points, gets the string as seperated strings(instead of /)
			this->getStringsSplitted(str1,v,uvt,vn);
			this->objects[nrOfObjects-1].addVertex(vertex[atof(v.c_str())-1]);
			if(vn.length()>0)
			this->objects[nrOfObjects-1].addNormal(normal[atof(vn.c_str())-1]);
			if(uvt.length()>0)
			this->objects[nrOfObjects-1].addUv(uv[atof(uvt.c_str())-1]);
			
			this->getStringsSplitted(str2,v,uvt,vn);
			this->objects[nrOfObjects-1].addVertex(vertex[atof(v.c_str())-1]);
			if(vn.length()>0)
			this->objects[nrOfObjects-1].addNormal(normal[atof(vn.c_str())-1]);
			if(uvt.length()>0)
			this->objects[nrOfObjects-1].addUv(uv[atof(uvt.c_str())-1]);
			
			this->getStringsSplitted(str3,v,uvt,vn);
			this->objects[nrOfObjects-1].addVertex(vertex[atof(v.c_str())-1]);
			if(vn.length()>0)
			this->objects[nrOfObjects-1].addNormal(normal[atof(vn.c_str())-1]);
			if(uvt.length()>0)
			this->objects[nrOfObjects-1].addUv(uv[atof(uvt.c_str())-1]);
			
		}
	}

	file.close();
	p= path+mtllibfile;
	file.open(p.c_str());
	int index=0;
	while(file.good())
	{
		getline(file,line);
		line = this->stripStartSpaces(line);
		//checks for the newmtl identifier
		if(line[0]=='n' &&line[1]=='e' &&line[2]=='w' &&line[3]=='m')
		{
			//saves the index of the object containing the
			//corresponding material
			string ident = this->getStringAfterSpace(line);
			for(int i=0;i<this->objects.size();i++)
			{
				if(ident==this->objects[i].getUsemtl())
					index=i;
			}
		}
		//saves the materials based on the correct index
		if(line[0]=='N' && line[1]=='s')
			this->objects[index].setNs(atof(getStringAfterSpace(line).c_str()));
		if(line[0]=='d')
			this->objects[index].setTr(atof(getStringAfterSpace(line).c_str()));
		if(line[0]=='T' && line[1]=='r')
			this->objects[index].setTr(atof(getStringAfterSpace(line).c_str()));
		
		if(line[0]=='K' && line[1]=='a')
		{
			string str1,str2,str3;
			getStrings(line,str1,str2,str3);
			this->objects[index].setKa(vec3(atof(str1.c_str()),atof(str2.c_str()),atof(str3.c_str())));
		}
		if(line[0]=='K' && line[1]=='d')
		{
			string str1,str2,str3;
			getStrings(line,str1,str2,str3);
			this->objects[index].setKd(vec3(atof(str1.c_str()),atof(str2.c_str()),atof(str3.c_str())));
		}
		if(line[0]=='K' && line[1]=='s')
		{
			string str1,str2,str3;
			getStrings(line,str1,str2,str3);
			this->objects[index].setKs(vec3(atof(str1.c_str()),atof(str2.c_str()),atof(str3.c_str())));
		}
		if(line[0]=='m' &&line[1]=='a' &&line[2]=='p' &&line[5]=='a')
		{
			this->objects[index].setTextureNameKa(getStringAfterSpace(line));
		}
		if(line[0]=='m' &&line[1]=='a' &&line[2]=='p' &&line[5]=='d')
		{
			this->objects[index].setTextureNameKd(getStringAfterSpace(line));
		}
	}
	file.close();
}

void OBJimporter::getStrings(const string line, string& str1, string& str2, string& str3)
{
	int counter=0;
	str1=str2=str3="";
	bool started=false;
	for(int i=0;i<line.length();i++)
	{
		if(isdigit(line[i])||ispunct(line[i]))
		{
			started=true;
			if(counter==0)
				str1+=line[i];
			if(counter==1)
			{
				str2+=line[i];
			}
			if(counter==2)
				str3+=line[i];
		}
		if(isspace(line[i])&&started)
				counter++;
	}
}
string OBJimporter::getStringAfterSpace(string s)
{
	string end;
	bool start=false;
	for(int i=0;i<s.length();i++)
	{
		if(isspace(s[i]))
		{
			start=true;
		}
		if(start&&!isspace(s[i]))
			end+=s[i];
	}
	return end;
}

void OBJimporter::getStringsSplitted(const string line, string& str1, string& str2, string& str3)
{
	str1=str2=str3="";
	int counter=0;
	
	for(int i=0;i<line.length();i++)
	{
		if(isdigit(line[i]))
		{
			if(counter==0)
				str1+=line[i];
			if(counter==1)
				str2+=line[i];
			if(counter==2)
				str3+=line[i];
		}
		if(ispunct(line[i]))
			counter++;
			
	}
}

string OBJimporter::stripStartSpaces(string s)
{
	bool start=false;
	string ret="";
	for(int i=0;i<s.length();i++)
	{
		if(isalpha(s[i]))
			start=true;
		if(start)
		{
			ret+=s[i];
		}
	}
	return ret;
}

Object OBJimporter::getObject(int index)
{
	if(index>-1&&index<this->nrOfObjects)
		return this->objects[index];
}
int OBJimporter::getNrOfObjects()
{
	return this->objects.size();
}