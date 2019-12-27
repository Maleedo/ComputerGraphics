
# OpenGL & GLEW library
find_package(OpenGL)
ADD_DEFINITIONS(-DGLEW_STATIC)
include_directories(${CMAKE_SOURCE_DIR}/lib/glew)

# source files
file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)
file(GLOB SHADERS ./shader/*.vert ./shader/*.frag)

# executable
add_executable(SolarSystem ${HEADERS} ${SOURCES} ${SHADERS})
target_link_libraries(SolarSystem utils glew glfw lodePNG ${OPENGL_LIBRARIES})
