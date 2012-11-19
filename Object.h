#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace glm;
class Object
{
private:
	int nrVerts;
	vec3 kd,ka,ks;
	float ns,tr;
	string textrueKa,textureKd,usemtl;
	
	vector<float> vertexArray,normalArray,uvArray;
public:
    Object();
    virtual ~Object();
    void setKa(vec3 ka);
    void setKd(vec3 kd);
    void setKs(vec3 ks);
    void setNs(float ns);
    void setTr(float d);
    void setTextureNameKa(string s);
    void setTextureNameKd(string s);
    void setUsemtl(string s);
    void addVertex(vec3 v);
    void addNormal(vec3 n);
    void addUv(vec2 uv);
    
    string getUsemtl();
    int getNrOfVerts();
	int getNrOfNormals();
	int getNrOfUvs();
    vec3 getKa();
    vec3 getKs();
    vec3 getKd();
    float getNs();
    float getTr();
    string getTextureNameKa();
    string getTextureNameKd();
    vector<float> getVertexArray();
    vector<float> getNormalArray();
    vector<float> getUvArray();
    
    void print();
};

#endif // OBJECT_H
