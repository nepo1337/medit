#ifndef MESH_H
#define MESH_H
#define GLEW_STATIC
#include <glew.h>
#include <OBJimporter.h>
#include <Object.h>
#include <iostream>
#include <vector>
#include <MeshInfo.h>
#include "SOIL/SOIL.h"

using namespace std;

class Mesh
{
private:
	OBJimporter objimp;
	string fileName;
	
	//holds VAOS,TEXHs for each object
	vector<MeshInfo> meshesInfo;
	vector<GLuint> VBOH;		//for freeing GFX
	bool debug;
	string path;
	void setFilename(string fn);
	
	//uploads vbos etc to gfx
	void uploadToGFX();
public:
	Mesh();
	~Mesh();
	bool import(string path,string filename);
	string getFilename();
	void freeGFX();
	
	//returns pointer to the meshinfo vector, one for each object
	vector<MeshInfo>* getMeshInfo();
};

#endif // MESH_H
