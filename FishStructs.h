#pragma once
#define box 1
#define sphere 0

#include <string>
#include <vector>

using namespace std;

struct FFloat3
{
	float x;
	float y;
	float z;
};

struct FFloat4
{
	float x;
	float y;
	float z;
	float w;
};

struct FJoint
{
	FFloat4 rotation;
	FFloat3 translation;
	FFloat3 scale;
};

struct FBoundingVolume
{
	FFloat3 pos;
	FFloat3 size;
	int type;
};

struct FFloat2
{
	float u;
	float v;
};

struct FFileInfo
{
	int numSkeleton;
	int numMaterials;
	int numGeometry;
};

struct FVertex
{
	FFloat3 position;
	FFloat3 normal;
	FFloat3 tangent;
	FFloat2 uv;
	vector<float> weights;
	vector<int> boneIndex;
};

struct FMesh
{
	int skeletonId;
	int numInfluences;
	int numVertices;
	int materialIndex;
	vector<FVertex> vertices;
};

struct FTexture
{
	int pathSize;
	int typeSize;
	char path[50];
	char type[30];
};

struct FMaterial
{
	int numTextures;
	vector<FTexture> textures;
};

struct FKeys
{
	float time;
	int numJoints;
	vector<FJoint> joints;
};

struct FSkeleton
{
	vector<FKeys> keys;
};


struct FishFile
{
	FFileInfo fFileInfo;
	FBoundingVolume boundingVol;
	vector<FMesh> FMeshes;
	vector<FMaterial> fMaterials;
};

struct AnimationFile
{
	int numKeys;
	vector<FSkeleton> skeletons;
};