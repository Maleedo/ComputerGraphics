# Install script for directory: /home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpmp.so.0.1.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpmp.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/build/libpmp.so.0.1.0"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/build/libpmp.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpmp.so.0.1.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpmp.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pmp" TYPE FILE FILES
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./BoundingBox.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./EdgeSet.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./GeometryObject.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./MatVec.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./PointSet.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./SurfaceMesh.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./Timer.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./properties.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./types.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pmp/algorithms" TYPE FILE FILES "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./algorithms/SurfaceNormals.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pmp/io" TYPE FILE FILES
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./io/EdgeSetIO.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./io/IOOptions.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./io/PointSetIO.h"
    "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/external/pmp/./io/SurfaceMeshIO.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/Ue3/cg_bezier-subdiv/build/external/pmp/gl/cmake_install.cmake")

endif()

