# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/linux_share/redis-net3.0/logger

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/linux_share/redis-net3.0/logger

# Include any dependencies generated for this target.
include CMakeFiles/logger.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/logger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/logger.dir/flags.make

CMakeFiles/logger.dir/logger.c.o: CMakeFiles/logger.dir/flags.make
CMakeFiles/logger.dir/logger.c.o: logger.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net3.0/logger/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/logger.dir/logger.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/logger.dir/logger.c.o   -c /mnt/hgfs/linux_share/redis-net3.0/logger/logger.c

CMakeFiles/logger.dir/logger.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/logger.dir/logger.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net3.0/logger/logger.c > CMakeFiles/logger.dir/logger.c.i

CMakeFiles/logger.dir/logger.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/logger.dir/logger.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net3.0/logger/logger.c -o CMakeFiles/logger.dir/logger.c.s

CMakeFiles/logger.dir/logger.c.o.requires:
.PHONY : CMakeFiles/logger.dir/logger.c.o.requires

CMakeFiles/logger.dir/logger.c.o.provides: CMakeFiles/logger.dir/logger.c.o.requires
	$(MAKE) -f CMakeFiles/logger.dir/build.make CMakeFiles/logger.dir/logger.c.o.provides.build
.PHONY : CMakeFiles/logger.dir/logger.c.o.provides

CMakeFiles/logger.dir/logger.c.o.provides.build: CMakeFiles/logger.dir/logger.c.o

CMakeFiles/logger.dir/main.c.o: CMakeFiles/logger.dir/flags.make
CMakeFiles/logger.dir/main.c.o: main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net3.0/logger/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/logger.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/logger.dir/main.c.o   -c /mnt/hgfs/linux_share/redis-net3.0/logger/main.c

CMakeFiles/logger.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/logger.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net3.0/logger/main.c > CMakeFiles/logger.dir/main.c.i

CMakeFiles/logger.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/logger.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net3.0/logger/main.c -o CMakeFiles/logger.dir/main.c.s

CMakeFiles/logger.dir/main.c.o.requires:
.PHONY : CMakeFiles/logger.dir/main.c.o.requires

CMakeFiles/logger.dir/main.c.o.provides: CMakeFiles/logger.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/logger.dir/build.make CMakeFiles/logger.dir/main.c.o.provides.build
.PHONY : CMakeFiles/logger.dir/main.c.o.provides

CMakeFiles/logger.dir/main.c.o.provides.build: CMakeFiles/logger.dir/main.c.o

# Object files for target logger
logger_OBJECTS = \
"CMakeFiles/logger.dir/logger.c.o" \
"CMakeFiles/logger.dir/main.c.o"

# External object files for target logger
logger_EXTERNAL_OBJECTS =

bin/logger: CMakeFiles/logger.dir/logger.c.o
bin/logger: CMakeFiles/logger.dir/main.c.o
bin/logger: CMakeFiles/logger.dir/build.make
bin/logger: CMakeFiles/logger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin/logger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/logger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/logger.dir/build: bin/logger
.PHONY : CMakeFiles/logger.dir/build

CMakeFiles/logger.dir/requires: CMakeFiles/logger.dir/logger.c.o.requires
CMakeFiles/logger.dir/requires: CMakeFiles/logger.dir/main.c.o.requires
.PHONY : CMakeFiles/logger.dir/requires

CMakeFiles/logger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/logger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/logger.dir/clean

CMakeFiles/logger.dir/depend:
	cd /mnt/hgfs/linux_share/redis-net3.0/logger && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/linux_share/redis-net3.0/logger /mnt/hgfs/linux_share/redis-net3.0/logger /mnt/hgfs/linux_share/redis-net3.0/logger /mnt/hgfs/linux_share/redis-net3.0/logger /mnt/hgfs/linux_share/redis-net3.0/logger/CMakeFiles/logger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/logger.dir/depend

