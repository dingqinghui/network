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
CMAKE_SOURCE_DIR = /mnt/hgfs/linux_share/net_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/linux_share/net_server

# Include any dependencies generated for this target.
include client/CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include client/CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include client/CMakeFiles/client.dir/flags.make

client/CMakeFiles/client.dir/main.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/main.c.o: client/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/main.c.o"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/main.c.o   -c /mnt/hgfs/linux_share/net_server/client/main.c

client/CMakeFiles/client.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/main.c.i"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/client/main.c > CMakeFiles/client.dir/main.c.i

client/CMakeFiles/client.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/main.c.s"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/client/main.c -o CMakeFiles/client.dir/main.c.s

client/CMakeFiles/client.dir/main.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/main.c.o.requires

client/CMakeFiles/client.dir/main.c.o.provides: client/CMakeFiles/client.dir/main.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/main.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/main.c.o.provides

client/CMakeFiles/client.dir/main.c.o.provides.build: client/CMakeFiles/client.dir/main.c.o

client/CMakeFiles/client.dir/__/net/src/net_server.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/net/src/net_server.c.o: net/src/net_server.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/net/src/net_server.c.o"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/net/src/net_server.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/net_server.c

client/CMakeFiles/client.dir/__/net/src/net_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/net/src/net_server.c.i"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/net_server.c > CMakeFiles/client.dir/__/net/src/net_server.c.i

client/CMakeFiles/client.dir/__/net/src/net_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/net/src/net_server.c.s"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/net_server.c -o CMakeFiles/client.dir/__/net/src/net_server.c.s

client/CMakeFiles/client.dir/__/net/src/net_server.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/net/src/net_server.c.o.requires

client/CMakeFiles/client.dir/__/net/src/net_server.c.o.provides: client/CMakeFiles/client.dir/__/net/src/net_server.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/net/src/net_server.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/net/src/net_server.c.o.provides

client/CMakeFiles/client.dir/__/net/src/net_server.c.o.provides.build: client/CMakeFiles/client.dir/__/net/src/net_server.c.o

client/CMakeFiles/client.dir/__/net/src/socket.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/net/src/socket.c.o: net/src/socket.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/net/src/socket.c.o"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/net/src/socket.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/socket.c

client/CMakeFiles/client.dir/__/net/src/socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/net/src/socket.c.i"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/socket.c > CMakeFiles/client.dir/__/net/src/socket.c.i

client/CMakeFiles/client.dir/__/net/src/socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/net/src/socket.c.s"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/socket.c -o CMakeFiles/client.dir/__/net/src/socket.c.s

client/CMakeFiles/client.dir/__/net/src/socket.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/net/src/socket.c.o.requires

client/CMakeFiles/client.dir/__/net/src/socket.c.o.provides: client/CMakeFiles/client.dir/__/net/src/socket.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/net/src/socket.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/net/src/socket.c.o.provides

client/CMakeFiles/client.dir/__/net/src/socket.c.o.provides.build: client/CMakeFiles/client.dir/__/net/src/socket.c.o

client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o: net/src/socket_epoll.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/net/src/socket_epoll.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/socket_epoll.c

client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/net/src/socket_epoll.c.i"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/socket_epoll.c > CMakeFiles/client.dir/__/net/src/socket_epoll.c.i

client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/net/src/socket_epoll.c.s"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/socket_epoll.c -o CMakeFiles/client.dir/__/net/src/socket_epoll.c.s

client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.requires

client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.provides: client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.provides

client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.provides.build: client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o

client/CMakeFiles/client.dir/__/net/src/socket_event.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/net/src/socket_event.c.o: net/src/socket_event.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/net/src/socket_event.c.o"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/net/src/socket_event.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/socket_event.c

client/CMakeFiles/client.dir/__/net/src/socket_event.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/net/src/socket_event.c.i"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/socket_event.c > CMakeFiles/client.dir/__/net/src/socket_event.c.i

client/CMakeFiles/client.dir/__/net/src/socket_event.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/net/src/socket_event.c.s"
	cd /mnt/hgfs/linux_share/net_server/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/socket_event.c -o CMakeFiles/client.dir/__/net/src/socket_event.c.s

client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.requires

client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.provides: client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.provides

client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.provides.build: client/CMakeFiles/client.dir/__/net/src/socket_event.c.o

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/main.c.o" \
"CMakeFiles/client.dir/__/net/src/net_server.c.o" \
"CMakeFiles/client.dir/__/net/src/socket.c.o" \
"CMakeFiles/client.dir/__/net/src/socket_epoll.c.o" \
"CMakeFiles/client.dir/__/net/src/socket_event.c.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/main.c.o
/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/__/net/src/net_server.c.o
/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/__/net/src/socket.c.o
/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o
/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/__/net/src/socket_event.c.o
/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/build.make
/mnt/hgfs/linux_share/bin/client: client/CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable /mnt/hgfs/linux_share/bin/client"
	cd /mnt/hgfs/linux_share/net_server/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
client/CMakeFiles/client.dir/build: /mnt/hgfs/linux_share/bin/client
.PHONY : client/CMakeFiles/client.dir/build

client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/main.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/net/src/net_server.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/net/src/socket.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/net/src/socket_epoll.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/net/src/socket_event.c.o.requires
.PHONY : client/CMakeFiles/client.dir/requires

client/CMakeFiles/client.dir/clean:
	cd /mnt/hgfs/linux_share/net_server/client && $(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : client/CMakeFiles/client.dir/clean

client/CMakeFiles/client.dir/depend:
	cd /mnt/hgfs/linux_share/net_server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/linux_share/net_server /mnt/hgfs/linux_share/net_server/client /mnt/hgfs/linux_share/net_server /mnt/hgfs/linux_share/net_server/client /mnt/hgfs/linux_share/net_server/client/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : client/CMakeFiles/client.dir/depend

