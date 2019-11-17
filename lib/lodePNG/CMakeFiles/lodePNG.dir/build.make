# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer"

# Include any dependencies generated for this target.
include lib/lodePNG/CMakeFiles/lodePNG.dir/depend.make

# Include the progress variables for this target.
include lib/lodePNG/CMakeFiles/lodePNG.dir/progress.make

# Include the compile flags for this target's objects.
include lib/lodePNG/CMakeFiles/lodePNG.dir/flags.make

lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o: lib/lodePNG/CMakeFiles/lodePNG.dir/flags.make
lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o: lib/lodePNG/lodepng.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o"
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lodePNG.dir/lodepng.cpp.o -c "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG/lodepng.cpp"

lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lodePNG.dir/lodepng.cpp.i"
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG/lodepng.cpp" > CMakeFiles/lodePNG.dir/lodepng.cpp.i

lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lodePNG.dir/lodepng.cpp.s"
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG/lodepng.cpp" -o CMakeFiles/lodePNG.dir/lodepng.cpp.s

lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.requires:

.PHONY : lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.requires

lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.provides: lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.requires
	$(MAKE) -f lib/lodePNG/CMakeFiles/lodePNG.dir/build.make lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.provides.build
.PHONY : lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.provides

lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.provides.build: lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o


# Object files for target lodePNG
lodePNG_OBJECTS = \
"CMakeFiles/lodePNG.dir/lodepng.cpp.o"

# External object files for target lodePNG
lodePNG_EXTERNAL_OBJECTS =

lib/lodePNG/liblodePNG.a: lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o
lib/lodePNG/liblodePNG.a: lib/lodePNG/CMakeFiles/lodePNG.dir/build.make
lib/lodePNG/liblodePNG.a: lib/lodePNG/CMakeFiles/lodePNG.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblodePNG.a"
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" && $(CMAKE_COMMAND) -P CMakeFiles/lodePNG.dir/cmake_clean_target.cmake
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lodePNG.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/lodePNG/CMakeFiles/lodePNG.dir/build: lib/lodePNG/liblodePNG.a

.PHONY : lib/lodePNG/CMakeFiles/lodePNG.dir/build

lib/lodePNG/CMakeFiles/lodePNG.dir/requires: lib/lodePNG/CMakeFiles/lodePNG.dir/lodepng.cpp.o.requires

.PHONY : lib/lodePNG/CMakeFiles/lodePNG.dir/requires

lib/lodePNG/CMakeFiles/lodePNG.dir/clean:
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" && $(CMAKE_COMMAND) -P CMakeFiles/lodePNG.dir/cmake_clean.cmake
.PHONY : lib/lodePNG/CMakeFiles/lodePNG.dir/clean

lib/lodePNG/CMakeFiles/lodePNG.dir/depend:
	cd "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer" "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer" "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG" "/home/pascal/Dokumente/Uni/WiSe19/Computer Graphics/cg_raytracer/lib/lodePNG/CMakeFiles/lodePNG.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : lib/lodePNG/CMakeFiles/lodePNG.dir/depend

