
include_directories("../3rdpart/glfw/include")
include_directories("../3rdpart/glad/include")

link_directories(../3rdpart/glfw/lib/)

# souce code add on this project
add_library (
XGL 
 "XGL.hpp"
 "XGLShader.cpp"
  "XGLProgram.cpp" "XGLVAO.cpp")


# TODO:if nessery add test file