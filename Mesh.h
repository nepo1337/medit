#ifndef MESH_H
#define MESH_H
#define GLEW_STATIC
#include <glew.h>
#include "ReadFish.h"
#include <Object.h>
#include <iostream>
#include <vector>
#include <MeshInfo.h>
#include "SOIL/SOIL.h"
#include "FishStructs.h"
#include "BoundingBox.h"

using namespace std;

class Mesh
{
private:
	BoundingBox bBox;
	ReadFish fishImp;
	string fileName;

	
	//holds VAOS,TEXHs for each object
	vector<MeshInfo> meshesInfo;
	vector<GLuint> VBOH;		//for freeing GFX
	bool debug;
	string path;
	void setFilename(string fn);
	GLuint bBoxVboh,bBoxVaoh;
	
	//uploads vbos etc to gfx
	void uploadToGFX();
	string type;
public:
	Mesh();
	~Mesh();
	bool import(string path,string filename);
	string getFilename();
	void freeGFX();
	string getType();
	void setType(string);
	
	//returns pointer to the meshinfo vector, one for each object
	vector<MeshInfo>* getMeshInfo();
	BoundingBox* getBoundingBox();
};

#endif // MESH_H
