#include(ExternalProject)
#
#set(GLFW_BUILD   ${PROJECT_SOURCE_DIR}/build) 
#MESSAGE( STATUS "this var key = ${CMAKE_BINARY_DIR}")
#
##set(GLFW_GIT_URL  https://github.com/glfw/glfw.git)  
#set(GLFW_CONFIGURE  cd ${PROJECT_SOURCE_DIR}/glfw/build && cmake -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=NO -DGLFW_BUILD_EXAMPLES=NO -DGLFW_BUILD_TESTS=NO ..)  
#set(GLFW_MAKE     cd ${PROJECT_SOURCE_DIR}/glfw/build && make) 
#
#ExternalProject_Add(
#  glfw
#  SOURCE_DIR        ${PROJECT_SOURCE_DIR}/glfw
#  BINARY_DIR        ${GLFW_BUILD}
#  CONFIGURE_COMMAND ${GLFW_CONFIGURE}
#  BUILD_COMMAND     ${GLFW_MAKE}
#  INSTALL_COMMAND   ""
#)

# 指定编译好的静态库文件的路径
#set(GLFW_LIB       ${PROJECT_SOURCE_DIR}/glfw/build/src/libglfw3.a)

# 指定头文件所在的目录
#set(GLFW_INCLUDE_DIR   ${PROJECT_SOURCE_DIR}/glfw/include)

# 添加第三方依赖包
include(FetchContent)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(add_FetchContent_MakeAvailable.cmake)
endif()

set(GLFW_TAG  3.3.2)  # 指定版本
set(GLFW_URL  https://github.com/glfw/glfw.git) 
#set(GLFW_CONFIGURE  cmake -DBUILD_SHARED_LIBS=NO -DGLFW_BUILD_EXAMPLES=NO -DGLFW_BUILD_TESTS=NO .)  

FetchContent_Declare(
  glfw
  GIT_REPOSITORY    ${GLFW_URL}
  GIT_TAG           ${GLFW_TAG}
  CONFIGURE_COMMAND ${GLFW_CONFIGURE}
)

FetchContent_MakeAvailable(glfw)
