#ifndef PATHHANDLER_H
#define PATHHANDLER_H
#include <vector>
#include "Path.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"

using namespace glm;
using namespace std;

class PathHandler
{
private:
	vector<Path> Paths;
public:
	PathHandler();
	~PathHandler();

};

#endif // PATHHANDLER_H
