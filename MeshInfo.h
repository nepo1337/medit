#ifndef MESHINFO_H
#define MESHINFO_H
#include <GL/glew.h>

class MeshInfo
{
	GLuint vaoh;
	GLuint texH;
	int nrOfVerts;

public:
	MeshInfo();
	~MeshInfo();
	void setVaoh(GLuint h);
	void setTexh(GLuint h);
	void setNrOfVerts(int n);
	
	GLuint getVaoh();
	GLuint getTexh();
	int getNrOfVerts();
	

};

#endif // MESHINFO_H
