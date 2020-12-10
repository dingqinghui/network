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
CMAKE_SOURCE_DIR = /mnt/hgfs/linux_share/zookeeper/stclient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/linux_share/zookeeper/stclient

# Include any dependencies generated for this target.
include CMakeFiles/stclient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/stclient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stclient.dir/flags.make

CMakeFiles/stclient.dir/client.c.o: CMakeFiles/stclient.dir/flags.make
CMakeFiles/stclient.dir/client.c.o: client.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/zookeeper/stclient/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/stclient.dir/client.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/stclient.dir/client.c.o   -c /mnt/hgfs/linux_share/zookeeper/stclient/client.c

CMakeFiles/stclient.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stclient.dir/client.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/zookeeper/stclient/client.c > CMakeFiles/stclient.dir/client.c.i

CMakeFiles/stclient.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stclient.dir/client.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/zookeeper/stclient/client.c -o CMakeFiles/stclient.dir/client.c.s

CMakeFiles/stclient.dir/client.c.o.requires:
.PHONY : CMakeFiles/stclient.dir/client.c.o.requires

CMakeFiles/stclient.dir/client.c.o.provides: CMakeFiles/stclient.dir/client.c.o.requires
	$(MAKE) -f CMakeFiles/stclient.dir/build.make CMakeFiles/stclient.dir/client.c.o.provides.build
.PHONY : CMakeFiles/stclient.dir/client.c.o.provides

CMakeFiles/stclient.dir/client.c.o.provides.build: CMakeFiles/stclient.dir/client.c.o

CMakeFiles/stclient.dir/zkcli.c.o: CMakeFiles/stclient.dir/flags.make
CMakeFiles/stclient.dir/zkcli.c.o: zkcli.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/zookeeper/stclient/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/stclient.dir/zkcli.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/stclient.dir/zkcli.c.o   -c /mnt/hgfs/linux_share/zookeeper/stclient/zkcli.c

CMakeFiles/stclient.dir/zkcli.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stclient.dir/zkcli.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/zookeeper/stclient/zkcli.c > CMakeFiles/stclient.dir/zkcli.c.i

CMakeFiles/stclient.dir/zkcli.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stclient.dir/zkcli.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/zookeeper/stclient/zkcli.c -o CMakeFiles/stclient.dir/zkcli.c.s

CMakeFiles/stclient.dir/zkcli.c.o.requires:
.PHONY : CMakeFiles/stclient.dir/zkcli.c.o.requires

CMakeFiles/stclient.dir/zkcli.c.o.provides: CMakeFiles/stclient.dir/zkcli.c.o.requires
	$(MAKE) -f CMakeFiles/stclient.dir/build.make CMakeFiles/stclient.dir/zkcli.c.o.provides.build
.PHONY : CMakeFiles/stclient.dir/zkcli.c.o.provides

CMakeFiles/stclient.dir/zkcli.c.o.provides.build: CMakeFiles/stclient.dir/zkcli.c.o

# Object files for target stclient
stclient_OBJECTS = \
"CMakeFiles/stclient.dir/client.c.o" \
"CMakeFiles/stclient.dir/zkcli.c.o"

# External object files for target stclient
stclient_EXTERNAL_OBJECTS =

/mnt/hgfs/linux_share/zookeeper/bin/stclient: CMakeFiles/stclient.dir/client.c.o
/mnt/hgfs/linux_share/zookeeper/bin/stclient: CMakeFiles/stclient.dir/zkcli.c.o
/mnt/hgfs/linux_share/zookeeper/bin/stclient: CMakeFiles/stclient.dir/build.make
/mnt/hgfs/linux_share/zookeeper/bin/stclient: CMakeFiles/stclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable /mnt/hgfs/linux_share/zookeeper/bin/stclient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stclient.dir/build: /mnt/hgfs/linux_share/zookeeper/bin/stclient
.PHONY : CMakeFiles/stclient.dir/build

CMakeFiles/stclient.dir/requires: CMakeFiles/stclient.dir/client.c.o.requires
CMakeFiles/stclient.dir/requires: CMakeFiles/stclient.dir/zkcli.c.o.requires
.PHONY : CMakeFiles/stclient.dir/requires

CMakeFiles/stclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stclient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stclient.dir/clean

CMakeFiles/stclient.dir/depend:
	cd /mnt/hgfs/linux_share/zookeeper/stclient && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/linux_share/zookeeper/stclient /mnt/hgfs/linux_share/zookeeper/stclient /mnt/hgfs/linux_share/zookeeper/stclient /mnt/hgfs/linux_share/zookeeper/stclient /mnt/hgfs/linux_share/zookeeper/stclient/CMakeFiles/stclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stclient.dir/depend

