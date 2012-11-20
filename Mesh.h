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
	vector<MeshInfo> meshesInfo;
	vector<GLuint> VBOH;
	bool debug;
	string path;
public:
	Mesh();
	~Mesh();
	void import(string path,string filename);
	void setFilename(string fn);
	string getFilename();
	void uploadToGFX();
	void freeGFX();
	vector<MeshInfo>* getMeshInfo();
};

#endif // MESH_H
