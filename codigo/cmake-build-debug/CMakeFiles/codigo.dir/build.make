# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.9

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.3.4\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.3.4\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/codigo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/codigo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/codigo.dir/flags.make

CMakeFiles/codigo.dir/prog.cpp.obj: CMakeFiles/codigo.dir/flags.make
CMakeFiles/codigo.dir/prog.cpp.obj: ../prog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/codigo.dir/prog.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\codigo.dir\prog.cpp.obj -c "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\prog.cpp"

CMakeFiles/codigo.dir/prog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/codigo.dir/prog.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\prog.cpp" > CMakeFiles\codigo.dir\prog.cpp.i

CMakeFiles/codigo.dir/prog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/codigo.dir/prog.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\prog.cpp" -o CMakeFiles\codigo.dir\prog.cpp.s

CMakeFiles/codigo.dir/prog.cpp.obj.requires:

.PHONY : CMakeFiles/codigo.dir/prog.cpp.obj.requires

CMakeFiles/codigo.dir/prog.cpp.obj.provides: CMakeFiles/codigo.dir/prog.cpp.obj.requires
	$(MAKE) -f CMakeFiles\codigo.dir\build.make CMakeFiles/codigo.dir/prog.cpp.obj.provides.build
.PHONY : CMakeFiles/codigo.dir/prog.cpp.obj.provides

CMakeFiles/codigo.dir/prog.cpp.obj.provides.build: CMakeFiles/codigo.dir/prog.cpp.obj


# Object files for target codigo
codigo_OBJECTS = \
"CMakeFiles/codigo.dir/prog.cpp.obj"

# External object files for target codigo
codigo_EXTERNAL_OBJECTS =

codigo.exe: CMakeFiles/codigo.dir/prog.cpp.obj
codigo.exe: CMakeFiles/codigo.dir/build.make
codigo.exe: CMakeFiles/codigo.dir/linklibs.rsp
codigo.exe: CMakeFiles/codigo.dir/objects1.rsp
codigo.exe: CMakeFiles/codigo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable codigo.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\codigo.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/codigo.dir/build: codigo.exe

.PHONY : CMakeFiles/codigo.dir/build

CMakeFiles/codigo.dir/requires: CMakeFiles/codigo.dir/prog.cpp.obj.requires

.PHONY : CMakeFiles/codigo.dir/requires

CMakeFiles/codigo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\codigo.dir\cmake_clean.cmake
.PHONY : CMakeFiles/codigo.dir/clean

CMakeFiles/codigo.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo" "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo" "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\cmake-build-debug" "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\cmake-build-debug" "C:\Users\diego\Documents\Facultad\AED 3\Labor\TP3\codigo\cmake-build-debug\CMakeFiles\codigo.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/codigo.dir/depend

