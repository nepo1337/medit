#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

using namespace std;
class OBJimporter
{
private:
	void getStrings(const string line, string &str1, string &str2,string &str3);
	void getStringsSplitted(const string line, string &str1, string &str2,string &str3);
	int nrOfObjects;
	vector<Object> objects;
	string getStringAfterSpace(string s);
	string mtllibfile;
	string stripStartSpaces(string s);
public:
	OBJimporter();
	virtual ~OBJimporter();
	void import(string path,string filename);
	Object getObject(int index);
	int getNrOfObjects();
};

#endif // OBJIMPORTER_H
