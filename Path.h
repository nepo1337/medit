#ifndef PATH_H
#define PATH_H
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"

using namespace glm;
using namespace std;

class Path
{
private:
	vector<vec3> flagPositions;
public:
	Path();
	~Path();
	void addFlag(vec3 pos);
	vec3 getFlagPosition(int i);
	int getNrOfFlags();

};

#endif // PATH_H
