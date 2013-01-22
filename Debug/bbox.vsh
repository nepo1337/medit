 #version 400
 in vec3 bBoxCordsVertex;
 uniform mat4 MVP;
 void main()
 {
  gl_Position=MVP*vec4(bBoxCordsVertex,1.0);
 }