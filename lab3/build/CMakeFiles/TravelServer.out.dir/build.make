# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kirill/Architecture/lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kirill/Architecture/lab3/build

# Include any dependencies generated for this target.
include CMakeFiles/TravelServer.out.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TravelServer.out.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TravelServer.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TravelServer.out.dir/flags.make

CMakeFiles/TravelServer.out.dir/main.cpp.o: CMakeFiles/TravelServer.out.dir/flags.make
CMakeFiles/TravelServer.out.dir/main.cpp.o: ../main.cpp
CMakeFiles/TravelServer.out.dir/main.cpp.o: CMakeFiles/TravelServer.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kirill/Architecture/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TravelServer.out.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TravelServer.out.dir/main.cpp.o -MF CMakeFiles/TravelServer.out.dir/main.cpp.o.d -o CMakeFiles/TravelServer.out.dir/main.cpp.o -c /home/kirill/Architecture/lab3/main.cpp

CMakeFiles/TravelServer.out.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TravelServer.out.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kirill/Architecture/lab3/main.cpp > CMakeFiles/TravelServer.out.dir/main.cpp.i

CMakeFiles/TravelServer.out.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TravelServer.out.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kirill/Architecture/lab3/main.cpp -o CMakeFiles/TravelServer.out.dir/main.cpp.s

CMakeFiles/TravelServer.out.dir/travel.cpp.o: CMakeFiles/TravelServer.out.dir/flags.make
CMakeFiles/TravelServer.out.dir/travel.cpp.o: ../travel.cpp
CMakeFiles/TravelServer.out.dir/travel.cpp.o: CMakeFiles/TravelServer.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kirill/Architecture/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TravelServer.out.dir/travel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TravelServer.out.dir/travel.cpp.o -MF CMakeFiles/TravelServer.out.dir/travel.cpp.o.d -o CMakeFiles/TravelServer.out.dir/travel.cpp.o -c /home/kirill/Architecture/lab3/travel.cpp

CMakeFiles/TravelServer.out.dir/travel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TravelServer.out.dir/travel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kirill/Architecture/lab3/travel.cpp > CMakeFiles/TravelServer.out.dir/travel.cpp.i

CMakeFiles/TravelServer.out.dir/travel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TravelServer.out.dir/travel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kirill/Architecture/lab3/travel.cpp -o CMakeFiles/TravelServer.out.dir/travel.cpp.s

CMakeFiles/TravelServer.out.dir/database.cpp.o: CMakeFiles/TravelServer.out.dir/flags.make
CMakeFiles/TravelServer.out.dir/database.cpp.o: ../database.cpp
CMakeFiles/TravelServer.out.dir/database.cpp.o: CMakeFiles/TravelServer.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kirill/Architecture/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TravelServer.out.dir/database.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TravelServer.out.dir/database.cpp.o -MF CMakeFiles/TravelServer.out.dir/database.cpp.o.d -o CMakeFiles/TravelServer.out.dir/database.cpp.o -c /home/kirill/Architecture/lab3/database.cpp

CMakeFiles/TravelServer.out.dir/database.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TravelServer.out.dir/database.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kirill/Architecture/lab3/database.cpp > CMakeFiles/TravelServer.out.dir/database.cpp.i

CMakeFiles/TravelServer.out.dir/database.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TravelServer.out.dir/database.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kirill/Architecture/lab3/database.cpp -o CMakeFiles/TravelServer.out.dir/database.cpp.s

# Object files for target TravelServer.out
TravelServer_out_OBJECTS = \
"CMakeFiles/TravelServer.out.dir/main.cpp.o" \
"CMakeFiles/TravelServer.out.dir/travel.cpp.o" \
"CMakeFiles/TravelServer.out.dir/database.cpp.o"

# External object files for target TravelServer.out
TravelServer_out_EXTERNAL_OBJECTS =

TravelServer.out: CMakeFiles/TravelServer.out.dir/main.cpp.o
TravelServer.out: CMakeFiles/TravelServer.out.dir/travel.cpp.o
TravelServer.out: CMakeFiles/TravelServer.out.dir/database.cpp.o
TravelServer.out: CMakeFiles/TravelServer.out.dir/build.make
TravelServer.out: /usr/local/lib/libPocoNetSSLd.so.103
TravelServer.out: /usr/local/lib/libPocoDatad.so.103
TravelServer.out: /usr/local/lib/libPocoMongoDBd.so.103
TravelServer.out: /usr/local/lib/libPocoUtild.so.103
TravelServer.out: /usr/local/lib/libPocoXMLd.so.103
TravelServer.out: /usr/local/lib/libPocoJSONd.so.103
TravelServer.out: /usr/local/lib/libPocoCryptod.so.103
TravelServer.out: /usr/lib/x86_64-linux-gnu/libssl.so
TravelServer.out: /usr/lib/x86_64-linux-gnu/libcrypto.so
TravelServer.out: /usr/local/lib/libPocoNetd.so.103
TravelServer.out: /usr/local/lib/libPocoFoundationd.so.103
TravelServer.out: CMakeFiles/TravelServer.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kirill/Architecture/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable TravelServer.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TravelServer.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TravelServer.out.dir/build: TravelServer.out
.PHONY : CMakeFiles/TravelServer.out.dir/build

CMakeFiles/TravelServer.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TravelServer.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TravelServer.out.dir/clean

CMakeFiles/TravelServer.out.dir/depend:
	cd /home/kirill/Architecture/lab3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kirill/Architecture/lab3 /home/kirill/Architecture/lab3 /home/kirill/Architecture/lab3/build /home/kirill/Architecture/lab3/build /home/kirill/Architecture/lab3/build/CMakeFiles/TravelServer.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TravelServer.out.dir/depend
