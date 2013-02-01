#ifndef MESHHANDLER_H
#define MESHHANDLER_H
#include <FilesInDir.h>
#include <iostream>
#include <Mesh.h>
#include <MeshInfo.h>
#include <vector>

using namespace std;

class MeshHandler
{
private:
	FilesInDir fileDirReader;
	vector<Mesh> meshes;
	string getFileExtension(string str);
	string getFileName(string str);
public:
	MeshHandler(string dir);
	MeshHandler();
	~MeshHandler();
	vector<MeshInfo>* getMeshInfo(int i);
	string getMeshName(int i);
	BoundingBox *getBoundingBox(int i);
	int getNrOfMeshes();
	string getMeshType(int i);
	int getMeshByName(string name);
	void load(string dir);
};

#endif // MESHHANDLER_H
