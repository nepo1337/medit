#pragma once

#include "FishStructs.h"
#include <fstream>

using namespace std;

class ReadFish
{
private:
	void MapFishFile(FishFile* fishFile, char* memblock);
	FishFile ff;
public:
	ReadFish();
	~ReadFish();
	void Read(string filename);
	FishFile *getFishFile();
};