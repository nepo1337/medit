#include "GLSLProgram.h"
#include <fstream>
#include <iostream>

GLSLProgram::GLSLProgram()
{
  this->linked=false;
  this->handle=0;
  this->shaderCode="";
  fragShadHandle=0;
  vertShadHandle=0;
  geoShadHandle=0;
  tessCShadHandle=0;
  tessEShadHandle=0;
}

GLSLProgram::~GLSLProgram()
{
	glDetachShader(handle, vertShadHandle);
	glDetachShader(handle, fragShadHandle);
	glDetachShader(handle, geoShadHandle);
	glDetachShader(handle, tessCShadHandle);
	glDetachShader(handle, tessEShadHandle);
	glDeleteShader(fragShadHandle);
	glDeleteShader(vertShadHandle);
	glDeleteShader(geoShadHandle);
	glDeleteShader(tessCShadHandle);
	glDeleteShader(tessEShadHandle);
	glDeleteProgram(handle);
}

void GLSLProgram::printShaderCode()
{
  cout << this->shaderCode;
}


bool GLSLProgram::compileShaderFromFile(const char* fileName, GLSLShader::GLSLShaderType type)
{
  ifstream file;
  file.open(fileName);
  if(!file.is_open())
  {
    logString="File not found";
    return false;
  }
  string line;
  string shad="";
  while(file.good())
  {
    getline(file,line);
    shad+=line+"\n";
  }
  this->shaderCode=shad;
  file.close();
  
  if(handle<=0)
  {
    handle=glCreateProgram();
    if(handle==0)
    {
      logString="Unable to create shader program.";
      return false;
    }
  }
  
  GLuint shaderHandle=0;
  switch(type)
  {
    case GLSLShader::VERTEX:
      shaderHandle=glCreateShader(GL_VERTEX_SHADER);
      vertShadHandle=shaderHandle;
      break;
    case GLSLShader::FRAGMENT:
      shaderHandle=glCreateShader(GL_FRAGMENT_SHADER);
      fragShadHandle=shaderHandle;
      break;
    case GLSLShader::GEOMETRY:
      shaderHandle=glCreateShader(GL_GEOMETRY_SHADER);
      geoShadHandle=shaderHandle;
      break;
    case GLSLShader::TESS_CONTROL:
      shaderHandle=glCreateShader(GL_TESS_CONTROL_SHADER);
      tessCShadHandle=shaderHandle;
      break;
    case GLSLShader::TESS_EVALUATION:
      shaderHandle=glCreateShader(GL_TESS_EVALUATION_SHADER);
      tessEShadHandle=shaderHandle;
      break;
    default:
      return false;
  }
  const char* c_code = shad.c_str();
  glShaderSource(shaderHandle,1,&c_code,NULL);
  
  glCompileShader(shaderHandle);
  
  int result;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
        // Compile failed, store log and return false
        int length = 0;
        logString = "";
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
	    cout << logString;
            delete [] c_log;
        }
        return false;
    }
    else
    {
        // Compile succeeded, attach shader and return true
        glAttachShader(handle, shaderHandle);
        return true;
    }
}

bool GLSLProgram::link()
{
  if(this->linked)
    return true;
  if(handle<=0)
    return false;
  glLinkProgram(handle);
  
      int status = 0;
    glGetProgramiv( handle, GL_LINK_STATUS, &status);
    if( GL_FALSE == status )
    {
        // Store log and return false
        int length = 0;
        logString = "";

        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
	{
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }
        return false;
    }
    else 
    {
        linked = true;
        return linked;
    }
}
void GLSLProgram::use()
{
  if(handle<=0||!linked)
    return;
  glUseProgram(handle);
}

string GLSLProgram::log()
{
  return this->logString;
}

int GLSLProgram::getHandle()
{
  return this->handle;
}

bool GLSLProgram::isLinked()
{
  return this->linked;
}

void GLSLProgram::bindAttribLocation(GLuint location, const char* name)
{
  glBindAttribLocation(handle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char* name)
{
  glBindFragDataLocation(handle, location, name);
}

void GLSLProgram::setUniform(const char *name, float x, float y, float z)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniform3f(loc,x,y,z);
}

void GLSLProgram::setUniform(const char *name, const vec3 & v)
{
    this->setUniform(name,v.x,v.y,v.z);
}

void GLSLProgram::setUniform(const char *name, const vec4 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniform4f(loc,v.x,v.y,v.z,v.w);
}

void GLSLProgram::setUniform(const char *name, const mat4 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::setUniform(const char *name, const mat3 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::setUniform(const char *name, float val)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniform1f(loc, val);
}

void GLSLProgram::setUniform(const char *name, int val)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniform1i(loc, val);
}

void GLSLProgram::setUniform(const char *name, bool val)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
      glUniform1i(loc, val);
}

void GLSLProgram::printActiveUniforms()
{
    GLint nUniforms, size, location, maxLen;
    GLchar * name;
    GLsizei written;
    GLenum type;

    glGetProgramiv( handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv( handle, GL_ACTIVE_UNIFORMS, &nUniforms);

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i )
    {
        glGetActiveUniform(handle, i, maxLen, &written, &size, &type, name);
        location = glGetUniformLocation(handle, name);
        printf(" %-8d | %s\n",location, name);
    }

    free(name);
}

void GLSLProgram::printActiveAttribs()
{

    GLint written, size, location, maxLength, nAttribs;
    GLenum type;
    GLchar * name;

    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

    name = (GLchar *) malloc(maxLength);

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for(int i = 0; i < nAttribs; i++)
    {
        glGetActiveAttrib(handle, i, maxLength, &written, &size, &type, name);
        location = glGetAttribLocation(handle, name);
        printf(" %-5d | %s\n",location, name);
    }

    free(name);
}

int GLSLProgram::getUniformLocation(const char * name)
{
    return glGetUniformLocation(handle, name);
}

