# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Thread_Codes.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Thread_Codes.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Thread_Codes.dir/flags.make

CMakeFiles/Thread_Codes.dir/main.cpp.o: CMakeFiles/Thread_Codes.dir/flags.make
CMakeFiles/Thread_Codes.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Thread_Codes.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Thread_Codes.dir/main.cpp.o -c "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/main.cpp"

CMakeFiles/Thread_Codes.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Thread_Codes.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/main.cpp" > CMakeFiles/Thread_Codes.dir/main.cpp.i

CMakeFiles/Thread_Codes.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Thread_Codes.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/main.cpp" -o CMakeFiles/Thread_Codes.dir/main.cpp.s

# Object files for target Thread_Codes
Thread_Codes_OBJECTS = \
"CMakeFiles/Thread_Codes.dir/main.cpp.o"

# External object files for target Thread_Codes
Thread_Codes_EXTERNAL_OBJECTS =

Thread_Codes: CMakeFiles/Thread_Codes.dir/main.cpp.o
Thread_Codes: CMakeFiles/Thread_Codes.dir/build.make
Thread_Codes: CMakeFiles/Thread_Codes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Thread_Codes"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Thread_Codes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Thread_Codes.dir/build: Thread_Codes

.PHONY : CMakeFiles/Thread_Codes.dir/build

CMakeFiles/Thread_Codes.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Thread_Codes.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Thread_Codes.dir/clean

CMakeFiles/Thread_Codes.dir/depend:
	cd "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes" "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes" "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug" "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug" "/Users/zhangyongbin/Desktop/Github Projects/C++ Series/C++ Primer/C++ Thread Program/Thread_Codes/cmake-build-debug/CMakeFiles/Thread_Codes.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Thread_Codes.dir/depend
