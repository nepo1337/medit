#include "ReadFish.h"

ReadFish::ReadFish(){}

ReadFish::~ReadFish(){}

void ReadFish::Read(string filename)
{
	string filepath = filename;

	char* memblock;

	ifstream rb;
	rb.open(filepath.c_str(), ios::in | ios::binary | ios::ate);
	ifstream::pos_type size;
	size = rb.tellg();
	memblock = new char[size];
	rb.seekg(0, ios::beg);

	rb.read((char*)memblock, size);
	
	rb.close();

	MapFishFile(&ff, memblock);
	

	//DoStuff
}

void ReadFish::MapFishFile(FishFile* fishFile, char* memblock)
{
	//Map Random Header
	fishFile->fFileInfo = *(FFileInfo*)memblock;
	memblock += sizeof(FFileInfo);
	//Map boundingVol
	fishFile->boundingVol = *(FBoundingVolume*)memblock;
	memblock += sizeof(FBoundingVolume);
	//Map Materials
	fishFile->fMaterials.resize(fishFile->fFileInfo.numMaterials);
	for(int m = 0; m < fishFile->fFileInfo.numMaterials; m++)
	{
		fishFile->fMaterials[m].numTextures = *(int*)memblock;
		memblock += sizeof(int);
		fishFile->fMaterials[m].textures.resize(fishFile->fMaterials[m].numTextures);
		for(int t = 0; t < fishFile->fMaterials[m].numTextures; t++)
		{
			fishFile->fMaterials[m].textures[t] = *(FTexture*)memblock;
			memblock += sizeof(FTexture);
		}
	}
	//Map Meshes
	fishFile->FMeshes.resize(fishFile->fFileInfo.numGeometry);
	for(int g = 0; g < fishFile->fFileInfo.numGeometry; g++)
	{
		fishFile->FMeshes[g].skeletonId = *(int*)memblock;
		memblock += sizeof(int);
		fishFile->FMeshes[g].numInfluences = *(int*)memblock;
		memblock += sizeof(int);
		fishFile->FMeshes[g].numVertices = *(int*)memblock;
		memblock += sizeof(int);
		fishFile->FMeshes[g].materialIndex = *(int*)memblock;
		memblock += sizeof(int);

		fishFile->FMeshes[g].vertices.resize(fishFile->FMeshes[g].numVertices);
		for(int v = 0; v < fishFile->FMeshes[g].numVertices; v++)
		{
			fishFile->FMeshes[g].vertices[v].position = *(FFloat3*)memblock;
			memblock += sizeof(FFloat3);
			fishFile->FMeshes[g].vertices[v].normal = *(FFloat3*)memblock;
			memblock += sizeof(FFloat3);
			fishFile->FMeshes[g].vertices[v].tangent = *(FFloat3*)memblock;
			memblock += sizeof(FFloat3);
			fishFile->FMeshes[g].vertices[v].uv = *(FFloat2*)memblock;
			memblock += sizeof(FFloat2);

			//Write Influences
			fishFile->FMeshes[g].vertices[v].weights.resize(fishFile->FMeshes[g].numInfluences);
			fishFile->FMeshes[g].vertices[v].boneIndex.resize(fishFile->FMeshes[g].numInfluences);
			for(int i = 0; i < fishFile->FMeshes[g].numInfluences; i++)
			{
				fishFile->FMeshes[g].vertices[v].weights[i] = *(float*)memblock;
				memblock += sizeof(float);
				fishFile->FMeshes[g].vertices[v].boneIndex[i] = *(int*)memblock;
				memblock += sizeof(int);
			}
		}
	}
}

FishFile *ReadFish::getFishFile()
{
	return &this->ff;
}