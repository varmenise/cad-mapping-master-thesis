# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build

# Include any dependencies generated for this target.
include CMakeFiles/Test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Test.dir/flags.make

CMakeFiles/Test.dir/Test.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/Test.cpp.o: ../Test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Test.dir/Test.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Test.dir/Test.cpp.o -c /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/Test.cpp

CMakeFiles/Test.dir/Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.dir/Test.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/Test.cpp > CMakeFiles/Test.dir/Test.cpp.i

CMakeFiles/Test.dir/Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.dir/Test.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/Test.cpp -o CMakeFiles/Test.dir/Test.cpp.s

CMakeFiles/Test.dir/Test.cpp.o.requires:
.PHONY : CMakeFiles/Test.dir/Test.cpp.o.requires

CMakeFiles/Test.dir/Test.cpp.o.provides: CMakeFiles/Test.dir/Test.cpp.o.requires
	$(MAKE) -f CMakeFiles/Test.dir/build.make CMakeFiles/Test.dir/Test.cpp.o.provides.build
.PHONY : CMakeFiles/Test.dir/Test.cpp.o.provides

CMakeFiles/Test.dir/Test.cpp.o.provides.build: CMakeFiles/Test.dir/Test.cpp.o

CMakeFiles/Test.dir/Ply.cpp.o: CMakeFiles/Test.dir/flags.make
CMakeFiles/Test.dir/Ply.cpp.o: ../Ply.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Test.dir/Ply.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Test.dir/Ply.cpp.o -c /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/Ply.cpp

CMakeFiles/Test.dir/Ply.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.dir/Ply.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/Ply.cpp > CMakeFiles/Test.dir/Ply.cpp.i

CMakeFiles/Test.dir/Ply.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.dir/Ply.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/Ply.cpp -o CMakeFiles/Test.dir/Ply.cpp.s

CMakeFiles/Test.dir/Ply.cpp.o.requires:
.PHONY : CMakeFiles/Test.dir/Ply.cpp.o.requires

CMakeFiles/Test.dir/Ply.cpp.o.provides: CMakeFiles/Test.dir/Ply.cpp.o.requires
	$(MAKE) -f CMakeFiles/Test.dir/build.make CMakeFiles/Test.dir/Ply.cpp.o.provides.build
.PHONY : CMakeFiles/Test.dir/Ply.cpp.o.provides

CMakeFiles/Test.dir/Ply.cpp.o.provides.build: CMakeFiles/Test.dir/Ply.cpp.o

# Object files for target Test
Test_OBJECTS = \
"CMakeFiles/Test.dir/Test.cpp.o" \
"CMakeFiles/Test.dir/Ply.cpp.o"

# External object files for target Test
Test_EXTERNAL_OBJECTS =

Test: CMakeFiles/Test.dir/Test.cpp.o
Test: CMakeFiles/Test.dir/Ply.cpp.o
Test: CMakeFiles/Test.dir/build.make
Test: CMakeFiles/Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Test.dir/build: Test
.PHONY : CMakeFiles/Test.dir/build

CMakeFiles/Test.dir/requires: CMakeFiles/Test.dir/Test.cpp.o.requires
CMakeFiles/Test.dir/requires: CMakeFiles/Test.dir/Ply.cpp.o.requires
.PHONY : CMakeFiles/Test.dir/requires

CMakeFiles/Test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Test.dir/clean

CMakeFiles/Test.dir/depend:
	cd /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build /Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build/CMakeFiles/Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Test.dir/depend

