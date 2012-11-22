#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H
#define GLEW_STATIC
#include <glew.h>
#include <string>
#include "glm/glm.hpp"


using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using namespace std;

namespace GLSLShader
{
  enum GLSLShaderType
  {
    VERTEX,FRAGMENT,GEOMETRY,TESS_CONTROL,TESS_EVALUATION
  };
};
class GLSLProgram
{
private:
    int handle;
    bool linked;
    string logString;
    string shaderCode;
    int  getUniformLocation(const char * name );
    GLuint fragShadHandle;
    GLuint vertShadHandle;
    GLuint geoShadHandle;
    GLuint tessCShadHandle;
    GLuint tessEShadHandle;
    
public:
    GLSLProgram();
    bool compileShaderFromFile(const char* fileName,GLSLShader::GLSLShaderType type);
    bool link();
    void use();
    
    string log();
    
    int getHandle();
    bool isLinked();
    
    void bindAttribLocation(GLuint location, const char * name);
    void bindFragDataLocation(GLuint location, const char * name );
    
    void setUniform(const char *name, float x, float y, float z);
    void setUniform(const char *name, const vec3 & v);
    void setUniform(const char *name, const vec4 & v);
    void setUniform(const char *name, const mat4 & m);
    void setUniform(const char *name, const mat3 & m);
    void setUniform(const char *name, float val );
    void setUniform(const char *name, int val );
    void setUniform(const char *name, bool val );

    void printActiveUniforms();
    void printActiveAttribs();
    
    void printShaderCode();
    void destroy();
};

#endif // GLSLPROGRAM_H